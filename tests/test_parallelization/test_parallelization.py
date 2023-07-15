import pytest

import subprocess
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def parallelization_cases():
    subprocess.run(["./Allclean"], cwd=DIR)
    subprocess.run(["./Allrun"], cwd=DIR, check=True)
    return DIR


@pytest.mark.parametrize("field", ["theta", "U", "C"])
def test_parallelization(parallelization_cases, field):
    for d in (parallelization_cases / "serial").iterdir():
        try:
            float(d.name)
        except ValueError:
            continue
        
        if d.name == "0":
            continue

        serial = np.array(ParsedParameterFile(parallelization_cases / "serial" / d.name / field)["internalField"].value())
        parallel = np.array(ParsedParameterFile(parallelization_cases / "parallel" / d.name / field)["internalField"].value())

        assert parallel == pytest.approx(serial, abs=5e-3)
