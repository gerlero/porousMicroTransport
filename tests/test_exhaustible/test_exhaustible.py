import itertools
from pathlib import Path

import numpy as np
import pytest
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def exhaustible_case():
    case = AsyncFoamCase(Path(__file__).parent)

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_exhaustion(exhaustible_case):
    assert len(exhaustible_case) > 1

    remainings = []
    for time in exhaustible_case:
        remaining = time["theta"].boundary_field["left"]["remaining"]

        assert remaining >= 0

        remainings.append(remaining)

    assert remainings[0] > 0
    assert remainings[-1] == 0

    assert all(prev >= curr for prev, curr in itertools.pairwise(remainings))


@pytest.mark.asyncio_cooperative
def test_infiltration(exhaustible_case):
    theta0 = np.asarray(exhaustible_case[0]["theta"].internal_field)
    dV = 30e-3 * 10e-3 * 0.18e-3 / 5000
    amount = exhaustible_case[0]["theta"].boundary_field["left"]["remaining"]

    assert len(exhaustible_case) > 1

    for time in exhaustible_case:
        theta = time["theta"]

        remaining = theta.boundary_field["left"]["remaining"]

        theta = np.asarray(theta.internal_field)

        assert np.sum(theta - theta0) * dV + remaining == pytest.approx(
            amount, rel=5e-3, abs=1e-22
        )


@pytest.mark.asyncio_cooperative
def test_limited_flux(exhaustible_case):
    initial = exhaustible_case[0]
    amount = initial["theta"].boundary_field["left"]["remaining"]
    inflow = 0
    previous_time = initial.time

    for time in exhaustible_case[1:]:
        flux = np.sum(
            np.broadcast_to(time["phi"].boundary_field["left"]["value"], (2,))
        )
        remaining = time["theta"].boundary_field["left"]["remaining"]

        inflow -= flux * (time.time - previous_time)
        previous_time = time.time

        assert inflow + remaining == pytest.approx(amount, rel=5e-3, abs=1e-22)


@pytest.mark.asyncio_cooperative
def test_exhausted_log(exhaustible_case):
    log = (Path(exhaustible_case.name) / "log.moistureDiffusivityFoam").read_text()

    assert log.count("patch left has been exhausted") == 1
