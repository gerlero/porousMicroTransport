import pytest

from pathlib import Path

import numpy as np
from scipy.special import erfc

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def dispersion_case(run_case):
    return await run_case(DIR)


@pytest.mark.asyncio_cooperative
async def test_dispersion(dispersion_case):
    alphaT = 5e-5

    field = np.asarray(dispersion_case["50"]["ampholyte.TARTRAZINE"].getContent()["internalField"].value())

    xnod = np.asarray(dispersion_case["0"]["Cx"].getContent()["internalField"].value())
    ynod = np.asarray(dispersion_case["0"]["Cy"].getContent()["internalField"].value())

    assert len(field) == len(xnod) == len(ynod)

    start = int(0.6*len(field))
    stop = int(0.8*len(field))

    assert np.linalg.norm(field[start:stop] - 0.5*erfc(ynod[start:stop]/(2*(alphaT*xnod[start:stop])**0.5))) <= 1e-3*(stop-start)
