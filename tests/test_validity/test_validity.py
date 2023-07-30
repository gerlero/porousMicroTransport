import pytest

from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module", params=["LETxs", "LETd"])
async def validity_case(run_case, request):
    return await run_case(DIR / request.param)


@pytest.mark.asyncio_cooperative
async def test_validity(validity_case):
    actual = np.array(ParsedParameterFile(validity_case / "100" / "theta")["internalField"].value())
    expected = np.array(ParsedParameterFile(validity_case / "100" / "theta.expected")["internalField"].value())

    assert actual == pytest.approx(expected, abs=5e-2)
