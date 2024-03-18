import pytest

from pathlib import Path

import numpy as np
from scipy.special import erfc
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def dispersion_case():
    case = AsyncFoamCase(Path(__file__).parent)

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_dispersion(dispersion_case):
    alphaT = 30e-6

    field = np.asarray(dispersion_case[-1]["ampholyte.TARTRAZINE"].internal_field)

    xnod = np.asarray(dispersion_case["0"]["Cx"].internal_field)
    ynod = np.asarray(dispersion_case["0"]["Cy"].internal_field)

    assert len(field) == len(xnod) == len(ynod)

    start = int(0.6*len(field))
    stop = int(0.8*len(field))

    assert np.linalg.norm(field[start:stop] - 0.5*erfc(ynod[start:stop]/(2*(alphaT*xnod[start:stop])**0.5))) <= 1e-3*(stop-start)
