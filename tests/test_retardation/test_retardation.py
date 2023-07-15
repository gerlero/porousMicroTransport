import pytest

import subprocess
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def retardation_case():
    subprocess.run(["./clean"], cwd=DIR)
    subprocess.run(["./run"], cwd=DIR, check=True)
    return DIR


def test_retardation(retardation_case):
    a5 =  np.array(ParsedParameterFile(retardation_case / "5" / "A")["internalField"].value())
    b10 =  np.array(ParsedParameterFile(retardation_case / "10" / "B")["internalField"].value())
    
    assert b10 == pytest.approx(a5, abs=1e-3)
