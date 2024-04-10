from pathlib import Path

import numpy as np
import pytest
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def serial_case():
    case = AsyncFoamCase(Path(__file__).parent / "serial")

    await case.clean()
    await case.run()

    return case


@pytest.fixture(scope="module")
async def parallel_case():
    case = AsyncFoamCase(Path(__file__).parent / "parallel")

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
@pytest.mark.parametrize("field", ["theta", "U", "C"])
def test_parallelization(serial_case, parallel_case, field):
    assert len(serial_case) == len(parallel_case) > 1

    for s, p in zip(serial_case[1:], parallel_case[1:]):
        serial = np.asarray(s[field].internal_field)
        parallel = np.asarray(p[field].internal_field)

        assert parallel == pytest.approx(serial, abs=5e-3)
