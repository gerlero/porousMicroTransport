import pytest

from pathlib import Path

from foamlib import AsyncFoamCase

@pytest.fixture(scope="module")
async def retardation_case():
    case = AsyncFoamCase(Path(__file__).parent)

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_retardation(retardation_case):
    a5 = retardation_case["5"]["A"].internal_field
    b10 = retardation_case["10"]["B"].internal_field
    
    assert b10 == pytest.approx(a5, abs=1e-3)
