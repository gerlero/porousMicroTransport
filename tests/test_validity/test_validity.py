import pytest

from pathlib import Path

DIR = Path(__file__).parent

# Cannot use a parametrized fixture for these: https://github.com/willemt/pytest-asyncio-cooperative/issues/46
@pytest.fixture(scope="module")
async def letxs_case(run_case):
    return await run_case(DIR / "LETxs")

@pytest.fixture(scope="module")
async def letd_case(run_case):
    return await run_case(DIR / "LETd")


@pytest.mark.asyncio_cooperative
async def test_letxs(letxs_case):
    actual = letxs_case["100"]["theta"].getContent()["internalField"].value()
    expected = letxs_case["100"]["theta.expected"].getContent()["internalField"].value()

    assert actual == pytest.approx(expected, abs=5e-2)

@pytest.mark.asyncio_cooperative
async def test_letd(letd_case):
    actual = letd_case["100"]["theta"].getContent()["internalField"].value()
    expected = letd_case["100"]["theta.expected"].getContent()["internalField"].value()

    assert actual == pytest.approx(expected, abs=5e-2)
