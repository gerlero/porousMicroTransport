import pytest

import subprocess
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def parallelization_cases():
    subprocess.run(["./Allclean"],cwd=DIR)
    subprocess.run(["./Allrun"], check=True, cwd=DIR)

def test_parallelization(parallelization_cases):
    for d in (DIR / "serial").iterdir():
        try:
            float(d.name)
        except ValueError:
            continue

        serial = np.array(ParsedParameterFile(DIR / "serial" / d.name / "theta")["internalField"].value())
        parallel = np.array(ParsedParameterFile(DIR / "parallel" / d.name / "theta")["internalField"].value())

        assert parallel == pytest.approx(serial, abs=5e-3)
