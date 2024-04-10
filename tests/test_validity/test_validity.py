from pathlib import Path

import pytest
from foamlib import AsyncFoamCase


# Cannot use a parametrized fixture for these: https://github.com/willemt/pytest-asyncio-cooperative/issues/46
@pytest.fixture(scope="module")
async def letxs_case():
    case = AsyncFoamCase(Path(__file__).parent / "LETxs")

    await case.clean()
    await case.run()

    return case


@pytest.fixture(scope="module")
async def letd_case():
    case = AsyncFoamCase(Path(__file__).parent / "LETd")

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_letxs(letxs_case):
    actual = letxs_case["100"]["theta"].internal_field
    expected = letxs_case["100"]["theta.expected"].internal_field

    assert actual == pytest.approx(expected, abs=5e-2)


@pytest.mark.asyncio_cooperative
def test_letd(letd_case):
    actual = letd_case["100"]["theta"].internal_field
    expected = letd_case["100"]["theta.expected"].internal_field

    assert actual == pytest.approx(expected, abs=5e-2)
