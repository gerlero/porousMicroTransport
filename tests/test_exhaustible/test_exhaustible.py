import pytest

from pathlib import Path

import numpy as np

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def exhaustible_case(run_case):
    return await run_case(DIR)


@pytest.mark.asyncio_cooperative
async def test_exhaustion(exhaustible_case):
    assert len(exhaustible_case.times) >= 4

    remainings = []
    for t in exhaustible_case.times:

            theta = exhaustible_case[t]["theta"].getContent()

            remaining = theta["boundaryField"]["left"]["remaining"]

            assert remaining >= 0

            remainings.append(remaining)

    assert remainings[0] > 0
    assert remainings[1] > 0

    assert remainings[-2] == 0
    assert remainings[-1] == 0

    assert all(prev >= curr for prev, curr in zip(remainings[:-1], remainings[1:]))

@pytest.mark.asyncio_cooperative
async def test_infiltration(exhaustible_case):
    theta0 = np.asarray(exhaustible_case[0]["theta"].getContent()["internalField"].value())
    dV = 30e-3*10e-3*0.18e-3/5000
    amount = 2e-8

    assert len(exhaustible_case.times) > 1

    for t in exhaustible_case.times:

        theta = exhaustible_case[t]["theta"].getContent()

        remaining = theta["boundaryField"]["left"]["remaining"]

        theta = np.asarray(theta["internalField"].value())

        assert np.sum(theta - theta0)*dV + remaining == pytest.approx(amount)
