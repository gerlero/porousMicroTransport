import pytest

from pathlib import Path

import numpy as np
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def velocity_case():
    case = AsyncFoamCase(Path(__file__).parent)

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_infiltration(velocity_case):
    theta0 = np.asarray(velocity_case[0]["theta"].internal_field)
    U = velocity_case[0]["U"].boundary_field["left"]["value"][0]
    h = 30e-3/5000

    assert len(velocity_case) > 1

    for time in velocity_case:
        theta = np.asarray(time["theta"].internal_field)

        assert np.sum(theta - theta0)*h == pytest.approx(U*time.time)
