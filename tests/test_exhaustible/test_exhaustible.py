import pytest

from pathlib import Path

import numpy as np

from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
async def exhaustible_case(run_case):
    return await run_case(DIR)


@pytest.mark.asyncio_cooperative
async def test_infiltration(exhaustible_case):
    theta0 = np.array(ParsedParameterFile(DIR / "0" / "theta")["internalField"].value())
    dV = 30e-3*10e-3*0.18e-3/5000
    amount = 2e-8

    for d in exhaustible_case.iterdir():
        try:
            float(d.name)
        except ValueError:
            continue

        theta = ParsedParameterFile(exhaustible_case / d.name / "theta")

        remaining = theta["boundaryField"]["left"]["remaining"]

        theta = theta["internalField"].value()

        assert np.sum(theta - theta0)*dV + remaining == pytest.approx(amount)
