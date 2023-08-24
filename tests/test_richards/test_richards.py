from pathlib import Path

import pytest
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def moisture_case():
    case = AsyncFoamCase(Path(__file__).parent / "moisture")

    await case.clean()
    await case.run()

    return case


@pytest.fixture(scope="module")
async def richards_case():
    case = AsyncFoamCase(Path(__file__).parent / "richards")

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
@pytest.mark.parametrize("field", ["theta", "U"])
def test_richards(moisture_case, richards_case, field):
    assert len(moisture_case) == len(richards_case) > 1

    for m, p in zip(moisture_case, richards_case):
        moisture = m[field].internal_field
        richards = p[field].internal_field

        assert richards == pytest.approx(moisture, abs=5e-2)
