import pytest

from pathlib import Path

import numpy as np
from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def reaction_case(run_case):
    return await run_case(DIR)


@pytest.mark.asyncio_cooperative
async def test_reaction(reaction_case):
    a0 = np.array(ParsedParameterFile(reaction_case / "0" / "A")["internalField"].value())
    b0 = np.array(ParsedParameterFile(reaction_case / "0" / "B")["internalField"].value())
    c0 = np.array(ParsedParameterFile(reaction_case / "0" / "C")["internalField"].value())

    kf = 100
    kr = 2

    a = np.array(ParsedParameterFile(reaction_case / "30" / "A")["internalField"].value())
    b = np.array(ParsedParameterFile(reaction_case / "30" / "B")["internalField"].value())
    c = np.array(ParsedParameterFile(reaction_case / "30" / "C")["internalField"].value())

    # Test mass conservation
    assert a + b + c == pytest.approx(a0 + b0 + c0)

    # Test equilibrium
    assert c**2/(a*b)== pytest.approx(kf/kr)
