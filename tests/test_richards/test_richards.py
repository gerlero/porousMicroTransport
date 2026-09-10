from pathlib import Path

import fronts
import numpy as np
import pytest
from foamlib import AsyncFoamCase
from fronts.D import van_genuchten

RHO_WATER = 1e3
GRAVITY = 9.81
SATURATION_OFFSET = 1e-8


@pytest.fixture(scope="module")
async def imbibition_case():
    case = AsyncFoamCase(Path(__file__).parent / "case")

    await case.clean()
    await case.run()

    return case


@pytest.fixture(scope="module")
async def forced_flow_case():
    case = AsyncFoamCase(Path(__file__).parent / "forcedFlow")

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
async def test_imbibition(imbibition_case):
    assert (
        imbibition_case.transport_properties["unsaturatedFlowModel"] == "VanGenuchten"
    )

    theta_i = imbibition_case[0]["theta"].internal_field
    theta_b = imbibition_case[0]["theta"].boundary_field["left"].value

    with imbibition_case.transport_properties as props:
        theta_range = (props["thetamin"].value, props["thetamax"].value)
        permeability = props["K"].value
        coeffs = props["VanGenuchtenCoeffs"]
        pc0 = coeffs["pc0"].value
        n = coeffs["n"].value
        l = coeffs["l"].value

    diffusivity = van_genuchten(
        k=permeability,
        alpha=RHO_WATER * GRAVITY / pc0,
        n=n,
        l=l,
        theta_range=theta_range,
    )

    # fronts evaluates the diffusivity at the boundary value, where the
    # Van Genuchten model is singular at exact saturation.
    solution = fronts.solve(
        diffusivity,
        i=theta_i,
        b=theta_b - SATURATION_OFFSET,
        itol=1e-6,
    )

    final = imbibition_case[-1]
    t = final.time
    assert t > 0

    theta = np.asarray(final["theta"].internal_field)
    velocity, _, _ = np.asarray(final["U"].internal_field).T
    x, _, _ = (await final.cell_centers()).internal_field.T

    theta_reference = solution(x, t)
    velocity_reference = solution.flux(x, t)
    theta_rmse = np.sqrt(np.mean(np.square(theta - theta_reference)))
    velocity_rmse = np.sqrt(np.mean(np.square(velocity - velocity_reference)))

    sorptivity = fronts.sorptivity(fronts.o(x, t), theta, i=theta_i, b=theta_b)

    assert sorptivity == pytest.approx(solution.sorptivity(), abs=1e-4)
    assert theta_rmse <= 6e-2
    assert velocity_rmse <= 5e-5


@pytest.mark.asyncio_cooperative
async def test_forced_flow(forced_flow_case):
    final = forced_flow_case[-1]
    p = np.asarray(final["p"].internal_field)
    theta = np.asarray(final["theta"].internal_field)
    velocity = np.asarray(final["U"].internal_field)
    x, _, _ = (await final.cell_centers()).internal_field.T

    with forced_flow_case.transport_properties as props:
        permeability = props["K"].value
        viscosity = props["phase.theta"]["mu"].value
        theta_max = props["thetamax"].value

    length = 30e-3
    inlet_pressure = 1e3
    pressure_reference = inlet_pressure * (1 - x / length)
    velocity_reference = permeability / viscosity * inlet_pressure / length

    assert p == pytest.approx(pressure_reference, abs=1e-8)
    assert theta == pytest.approx(theta_max, abs=1e-12)
    assert velocity[:, 0] == pytest.approx(velocity_reference, rel=1e-10)
    assert velocity[:, 1:] == pytest.approx(0, abs=1e-15)
