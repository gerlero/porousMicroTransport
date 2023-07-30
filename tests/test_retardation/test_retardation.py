import pytest

from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def retardation_case(run_case):
    return await run_case(DIR)


@pytest.mark.asyncio_cooperative
async def test_retardation(retardation_case):
    a5 =  np.array(ParsedParameterFile(retardation_case / "5" / "A")["internalField"].value())
    b10 =  np.array(ParsedParameterFile(retardation_case / "10" / "B")["internalField"].value())
    
    assert b10 == pytest.approx(a5, abs=1e-3)
