from pathlib import Path

import fronts
import numpy as np
import pytest
from foamlib import AsyncFoamCase
from fronts.D import brooks_and_corey, letd, letxs, van_genuchten


def get_model(transport_properties):
    rho = 1e3
    g = 9.81

    with transport_properties:
        theta_range = (
            transport_properties["thetamin"].value,
            transport_properties["thetamax"].value,
        )

        model = transport_properties["unsaturatedFlowModel"]
        if model == "BrooksAndCorey":
            coeffs = transport_properties["BrooksAndCoreyCoeffs"]
            return brooks_and_corey(
                k=transport_properties["K"].value,
                alpha=rho * g / coeffs["pc0"].value,
                n=1 / coeffs["alpha"].value,
                l=coeffs["l"].value,
                theta_range=theta_range,
            )
        elif model == "LETd":
            coeffs = transport_properties["LETCoeffs"]
            return letd(
                L=coeffs["L"].value,
                E=coeffs["E"].value,
                T=coeffs["T"].value,
                Dwt=coeffs["Dwt"].value,
                theta_range=theta_range,
            )
        elif model == "LETxs":
            coeffs = transport_properties["LETCoeffs"]
            return letxs(
                k=transport_properties["K"].value,
                alpha=rho * g / coeffs["pc0"].value,
                Lw=coeffs["Lw"].value,
                Ew=coeffs["Ew"].value,
                Tw=coeffs["Tw"].value,
                Ls=coeffs["Ls"].value,
                Es=coeffs["Es"].value,
                Ts=coeffs["Ts"].value,
                theta_range=theta_range,
            )
        elif model == "VanGenuchten":
            coeffs = transport_properties["VanGenuchtenCoeffs"]
            return van_genuchten(
                k=transport_properties["K"].value,
                alpha=rho * g / coeffs["pc0"].value,
                n=coeffs["n"].value,
                l=coeffs["l"].value,
                theta_range=theta_range,
            )
        else:
            assert False


@pytest.mark.asyncio_cooperative
@pytest.mark.parametrize(
    "model_name", ["BrooksAndCorey", "LETd", "LETxs", "VanGenuchten"]
)
async def test_validity(model_name):
    case = AsyncFoamCase(Path(__file__).parent / model_name)

    await case.clean()
    assert case.transport_properties["unsaturatedFlowModel"] == model_name

    D = get_model(case.transport_properties)
    i = case[0]["theta"].internal_field
    b = case[0]["theta"].boundary_field["left"].value
    sol = fronts.solve(D, i=i, b=b, itol=1e-6)

    await case.run()

    t = case[-1].time
    assert t > 0
    theta = case[-1]["theta"].internal_field
    U, _, _ = case[-1]["U"].internal_field.T
    x, _, _ = (await case[-1].cell_centers()).internal_field.T

    assert fronts.sorptivity(fronts.o(x, t), theta, i=i, b=b) == pytest.approx(
        sol.sorptivity(), abs=1e-5
    )

    assert np.linalg.norm(theta - sol(x, t)) <= 0.25
    assert (
        np.linalg.norm(
            U
            - sol.flux(
                x,
                t,
            )
        )
        <= 2e-5
    )
