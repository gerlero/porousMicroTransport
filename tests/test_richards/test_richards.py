from pathlib import Path

import fronts
import numpy as np
import pytest
from foamlib import AsyncFoamCase
from fronts.D import van_genuchten


@pytest.fixture(scope="module")
async def richards_case():
    case = AsyncFoamCase(Path(__file__).parent / "case")

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
async def test_richards(richards_case):
    assert richards_case.transport_properties["unsaturatedFlowModel"] == "VanGenuchten"

    theta_i = richards_case[0]["theta"].internal_field
    theta_b = richards_case[0]["theta"].boundary_field["left"].value

    with richards_case.transport_properties as props:
        theta_range = (props["thetamin"].value, props["thetamax"].value)
        K = props["K"].value
        coeffs = props["VanGenuchtenCoeffs"]
        pc0 = coeffs["pc0"].value
        n = coeffs["n"].value
        l = coeffs["l"].value

    D = van_genuchten(
        k=K,
        alpha=1e3 * 9.81 / pc0,
        n=n,
        l=l,
        theta_range=theta_range
    )

    sol = fronts.solve(D, i=theta_i, b=theta_b - 1e-8, itol=1e-6)

    t = richards_case[-1].time
    assert t > 0

    theta = np.asarray(richards_case[-1]["theta"].internal_field)
    U, _, _ = np.asarray(richards_case[-1]["U"].internal_field).T
    x, _, _ = (await richards_case[-1].cell_centers()).internal_field.T

    assert fronts.sorptivity(fronts.o(x, t), theta, i=theta_i, b=theta_b) == pytest.approx(
        sol.sorptivity(), abs=1e-4
    )
    assert np.linalg.norm(theta - sol(x, t)) <= 10
    assert np.linalg.norm(U - sol.flux(x, t)) <= 1e-3
