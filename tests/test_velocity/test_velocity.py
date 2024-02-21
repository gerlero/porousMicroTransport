import pytest

from pathlib import Path

import numpy as np
import aiofoam
from PyFoam.RunDictionary.SolutionDirectory import SolutionDirectory


@pytest.fixture(scope="module")
async def velocity_case():
    case = aiofoam.Case(Path(__file__).parent)

    await case.clean()
    await case.run()

    return SolutionDirectory(case.path)


@pytest.mark.asyncio_cooperative
def test_infiltration(velocity_case):
    theta0 = np.asarray(velocity_case[0]["theta"].getContent()["internalField"].value())
    U = velocity_case[0]["U"].getContent()["boundaryField"]["left"]["value"].value()[0]
    h = 30e-3/5000

    assert len(velocity_case.times) > 1

    for t in velocity_case.times:
        theta = np.asarray(velocity_case[t]["theta"].getContent()["internalField"].value())

        assert np.sum(theta - theta0)*h == pytest.approx(U*float(t))
