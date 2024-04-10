from pathlib import Path

import numpy as np
import pytest
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def reaction_case():
    case = AsyncFoamCase(Path(__file__).parent)

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_reaction(reaction_case):
    a0 = np.asarray(reaction_case["0"]["A"].internal_field)
    b0 = np.asarray(reaction_case["0"]["B"].internal_field)
    c0 = np.asarray(reaction_case["0"]["C"].internal_field)

    kf = 100
    kr = 2

    a = np.asarray(reaction_case["30"]["A"].internal_field)
    b = np.asarray(reaction_case["30"]["B"].internal_field)
    c = np.asarray(reaction_case["30"]["C"].internal_field)

    # Test mass conservation
    assert a + b + c == pytest.approx(a0 + b0 + c0)

    # Test equilibrium
    assert c**2 / (a * b) == pytest.approx(kf / kr)
