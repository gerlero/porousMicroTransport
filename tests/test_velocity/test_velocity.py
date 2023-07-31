import pytest

from pathlib import Path

import numpy as np

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def velocity_case(run_case):
    return await run_case(DIR)


@pytest.mark.asyncio_cooperative
async def test_infiltration(velocity_case):
    theta0 = np.asarray(velocity_case[0]["theta"].getContent()["internalField"].value())
    U = velocity_case[0]["U"].getContent()["boundaryField"]["left"]["value"].value()[0]
    h = 30e-3/5000

    assert len(velocity_case.times) > 1

    for t in velocity_case.times:
        theta = np.asarray(velocity_case[t]["theta"].getContent()["internalField"].value())

        assert np.sum(theta - theta0)*h == pytest.approx(U*float(t))
