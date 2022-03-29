import pytest

import subprocess
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def validity_cases():
    subprocess.run(["./Allclean"],cwd=DIR)
    subprocess.run(["./Allrun"], check=True, cwd=DIR)

@pytest.mark.parametrize("model", ["LETxs", "LETd"])
def test_validity(validity_cases, model):
    actual = np.array(ParsedParameterFile(DIR / model / "100" / "theta")["internalField"].value())
    expected = np.array(ParsedParameterFile(DIR / model / "100" / "theta.expected")["internalField"].value())

    assert actual == pytest.approx(expected, abs=5e-2)
