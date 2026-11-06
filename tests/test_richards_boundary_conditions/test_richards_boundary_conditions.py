import itertools
from pathlib import Path

import pytest
from foamlib import AsyncFoamCase


@pytest.fixture(scope="module")
async def darcy_grad_pressure_case():
    case = AsyncFoamCase(Path(__file__).parent / "darcyGradPressure")

    await case.clean()
    await case.run()

    return case


@pytest.fixture(scope="module")
async def exhaustible_case():
    case = AsyncFoamCase(Path(__file__).parent / "exhaustible")

    await case.clean()
    await case.run()

    return case


@pytest.mark.asyncio_cooperative
def test_darcy_grad_pressure(darcy_grad_pressure_case):
    velocity = darcy_grad_pressure_case[0]["U"].boundary_field["left"]["value"][0]
    face_area = 10e-3 * 0.18e-3

    assert len(darcy_grad_pressure_case) > 1

    for time in darcy_grad_pressure_case[1:]:
        flux = time["phi"].boundary_field["left"]["value"]

        assert flux == pytest.approx(-velocity * face_area, rel=1e-6, abs=0)


@pytest.mark.asyncio_cooperative
def test_exhaustible(exhaustible_case):
    assert len(exhaustible_case) > 1

    initial = exhaustible_case[0]
    amount = initial["p"].boundary_field["left"]["remaining"]
    remaining = [amount]
    inflow = 0
    previous_time = initial.time

    for time in exhaustible_case[1:]:
        flux = time["phi"].boundary_field["left"]["value"]
        current_remaining = time["p"].boundary_field["left"]["remaining"]

        inflow -= flux * (time.time - previous_time)
        previous_time = time.time

        assert inflow + current_remaining == pytest.approx(amount, rel=5e-3, abs=1e-18)
        remaining.append(current_remaining)

    assert remaining[0] > 0
    assert remaining[-1] == 0
    assert all(
        previous >= current >= 0 for previous, current in itertools.pairwise(remaining)
    )

    log = Path(__file__).parent / "exhaustible" / "log.microRichardsTransportFoam"
    assert log.read_text().count("patch left has been exhausted") == 1
