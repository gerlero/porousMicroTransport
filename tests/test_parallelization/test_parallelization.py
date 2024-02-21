import pytest

from pathlib import Path

import numpy as np
import aiofoam
from PyFoam.RunDictionary.SolutionDirectory import SolutionDirectory


@pytest.fixture(scope="module")
async def serial_case():
    case = aiofoam.Case(Path(__file__).parent / "serial")

    await case.clean()
    await case.run()

    return SolutionDirectory(case.path)


@pytest.fixture(scope="module")
async def parallel_case():
    case = aiofoam.Case(Path(__file__).parent / "parallel")

    await case.clean()
    await case.run()

    return SolutionDirectory(case.path)


@pytest.mark.asyncio_cooperative
@pytest.mark.parametrize("field", ["theta", "U", "C"])
def test_parallelization(serial_case, parallel_case, field):
    assert len(serial_case.times) == len(parallel_case.times) > 1

    for t in serial_case.times[1:]:
        serial = np.asarray(serial_case[t][field].getContent()["internalField"].value())
        parallel = np.asarray(parallel_case[t][field].getContent()["internalField"].value())

        assert parallel == pytest.approx(serial, abs=5e-3)
