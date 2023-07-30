import pytest

import asyncio
import multiprocessing

from contextlib import asynccontextmanager
from pathlib import Path

class _NSemaphore:
    def __init__(self, value=1):
        assert value >= 0
        self._value = value
        self._cond = asyncio.Condition()

    async def acquire(self, n=1):
        assert n >= 1
        async with self._cond:
            await self._cond.wait_for(lambda: self._value >= n)
            self._value -= n

    async def release(self, n=1):
        assert n >= 1
        async with self._cond:
            self._value += n
            self._cond.notify(n)

    @asynccontextmanager
    async def __call__(self, n=1):
        await self.acquire(n)
        try:
            yield
        finally:
            await self.release(n)


@pytest.fixture(scope="session")
def max_cpus():
    return multiprocessing.cpu_count()

@pytest.fixture(scope="session")
def reserve_cpus(max_cpus):
    return _NSemaphore(max_cpus)

@pytest.fixture(scope="session")
def run_case(reserve_cpus, max_cpus):
    async def _run_case(case, *, cpus=1):
        case = Path(case)
        assert cpus >= 1
        cpus = min(cpus, max_cpus) # Oversubscribe if necessary

        clean = await asyncio.create_subprocess_exec("./clean", cwd=case, stdout=asyncio.subprocess.DEVNULL)
        await clean.wait()

        async with reserve_cpus(cpus):
            run = await asyncio.create_subprocess_exec("./run", cwd=case, stdout=asyncio.subprocess.DEVNULL)
            await run.wait()

        assert run.returncode == 0, f"Case {case.name}: run failed"

        return case
    return _run_case
