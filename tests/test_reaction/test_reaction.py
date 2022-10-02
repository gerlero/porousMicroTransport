import pytest

import subprocess
from pathlib import Path

import numpy as np
from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def reaction_case():
    subprocess.run(["./clean"], cwd=DIR)
    subprocess.run(["./run"], check=True, cwd=DIR)
    return DIR

def test_rect(reaction_case):
    a0 = np.array(ParsedParameterFile(DIR / "0" / "A")["internalField"].value())
    b0 = np.array(ParsedParameterFile(DIR / "0" / "B")["internalField"].value())
    c0 = np.array(ParsedParameterFile(DIR / "0" / "C")["internalField"].value())

    kf = 100
    kr = 2

    a = np.array(ParsedParameterFile(DIR / "100" / "A")["internalField"].value())
    b = np.array(ParsedParameterFile(DIR / "100" / "B")["internalField"].value())
    c = np.array(ParsedParameterFile(DIR / "100" / "C")["internalField"].value())

    # Test mass conservation
    assert a + b + c == pytest.approx(a0 + b0 + c0)

    # Test equilibrium
    assert c**2/(a*b)== pytest.approx(kf/kr, rel=1e-4)
