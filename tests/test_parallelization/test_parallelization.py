import pytest

import asyncio
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def parallelization_cases(run_case):
    # https://github.com/willemt/pytest-asyncio-cooperative/issues/36
    return await asyncio.gather(run_case(DIR / "serial"), run_case(DIR / "parallel", cpus=3))


@pytest.mark.asyncio_cooperative
@pytest.mark.parametrize("field", ["theta", "U", "C"])
async def test_parallelization(parallelization_cases, field):
    serial_case, parallel_case = parallelization_cases

    for d in serial_case.iterdir():
        try:
            float(d.name)
        except ValueError:
            continue
        
        if d.name == "0":
            continue

        serial = np.array(ParsedParameterFile(serial_case / d.name / field)["internalField"].value())
        parallel = np.array(ParsedParameterFile(parallel_case / d.name / field)["internalField"].value())

        assert parallel == pytest.approx(serial, abs=5e-3)
