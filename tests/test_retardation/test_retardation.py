import pytest

from pathlib import Path

import aiofoam
from PyFoam.RunDictionary.SolutionDirectory import SolutionDirectory

@pytest.fixture(scope="module")
async def retardation_case():
    case = aiofoam.Case(Path(__file__).parent)

    await case.clean()
    await case.run()

    return SolutionDirectory(case.path)


@pytest.mark.asyncio_cooperative
def test_retardation(retardation_case):
    a5 = retardation_case["5"]["A"].getContent()["internalField"].value()
    b10 = retardation_case["10"]["B"].getContent()["internalField"].value()
    
    assert b10 == pytest.approx(a5, abs=1e-3)
