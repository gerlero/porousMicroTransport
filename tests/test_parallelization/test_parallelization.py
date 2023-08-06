import pytest

from pathlib import Path

import numpy as np

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def serial_case(run_case):
    return await run_case(DIR / "serial")

@pytest.fixture(scope="module")
async def parallel_case(run_case):
    return await run_case(DIR / "parallel", cpus=3)


@pytest.mark.asyncio_cooperative
@pytest.mark.parametrize("field", ["theta", "U", "C"])
async def test_parallelization(serial_case, parallel_case, field):
    assert len(serial_case.times) == len(parallel_case.times) > 1

    for t in serial_case.times[1:]:
        serial = np.asarray(serial_case[t][field].getContent()["internalField"].value())
        parallel = np.asarray(parallel_case[t][field].getContent()["internalField"].value())

        assert parallel == pytest.approx(serial, abs=5e-3)
