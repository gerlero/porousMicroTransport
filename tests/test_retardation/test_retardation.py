import pytest

import subprocess
from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def retardation_case():
    subprocess.run(["./clean"],cwd=DIR)
    subprocess.run(["./run"], check=True, cwd=DIR)

def test_retardation(retardation_case):
    A_5 =  np.array(ParsedParameterFile(DIR / "5" / "A")["internalField"].value())
    B_10 =  np.array(ParsedParameterFile(DIR / "10" / "B")["internalField"].value())
    
    assert B_10 == pytest.approx(A_5, abs=1e-3)
