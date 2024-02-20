import pytest

from pathlib import Path

import numpy as np
import aiofoam
from scipy.special import erfc
from PyFoam.RunDictionary.SolutionDirectory import SolutionDirectory


@pytest.fixture(scope="module")
async def dispersion_case():
    case = aiofoam.Case(Path(__file__).parent)

    await case.clean()
    await case.run()

    return SolutionDirectory(case.path)


@pytest.mark.asyncio_cooperative
def test_dispersion(dispersion_case):
    alphaT = 30e-6

    field = np.asarray(dispersion_case["50"]["ampholyte.TARTRAZINE"].getContent()["internalField"].value())

    xnod = np.asarray(dispersion_case["0"]["Cx"].getContent()["internalField"].value())
    ynod = np.asarray(dispersion_case["0"]["Cy"].getContent()["internalField"].value())

    assert len(field) == len(xnod) == len(ynod)

    start = int(0.6*len(field))
    stop = int(0.8*len(field))

    assert np.linalg.norm(field[start:stop] - 0.5*erfc(ynod[start:stop]/(2*(alphaT*xnod[start:stop])**0.5))) <= 1e-3*(stop-start)
