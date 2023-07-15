import pytest

import subprocess
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module", params=["LETxs", "LETd"])
def validity_case(request):
    case = DIR / request.param
    subprocess.run(["./clean"], cwd=case)
    subprocess.run(["./run"], cwd=case, check=True)
    return case


def test_validity(validity_case):
    actual = np.array(ParsedParameterFile(validity_case / "100" / "theta")["internalField"].value())
    expected = np.array(ParsedParameterFile(validity_case / "100" / "theta.expected")["internalField"].value())

    assert actual == pytest.approx(expected, abs=5e-2)
