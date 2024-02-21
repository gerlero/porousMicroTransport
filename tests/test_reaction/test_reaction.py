import pytest

from pathlib import Path

import numpy as np
import aiofoam
from PyFoam.RunDictionary.SolutionDirectory import SolutionDirectory


@pytest.fixture(scope="module")
async def reaction_case():
    case = aiofoam.Case(Path(__file__).parent)

    await case.clean()
    await case.run()

    return SolutionDirectory(case.path)


@pytest.mark.asyncio_cooperative
def test_reaction(reaction_case):
    a0 = np.asarray(reaction_case["0"]["A"].getContent()["internalField"].value())
    b0 = np.asarray(reaction_case["0"]["B"].getContent()["internalField"].value())
    c0 = np.asarray(reaction_case["0"]["C"].getContent()["internalField"].value())

    kf = 100
    kr = 2

    a = np.asarray(reaction_case["30"]["A"].getContent()["internalField"].value())
    b = np.asarray(reaction_case["30"]["B"].getContent()["internalField"].value())
    c = np.asarray(reaction_case["30"]["C"].getContent()["internalField"].value())

    # Test mass conservation
    assert a + b + c == pytest.approx(a0 + b0 + c0)

    # Test equilibrium
    assert c**2/(a*b)== pytest.approx(kf/kr)
