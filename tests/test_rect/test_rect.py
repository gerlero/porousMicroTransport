import pytest

import subprocess
import math
from pathlib import Path

import numpy as np

DIR = Path(__file__).parent

@pytest.fixture(scope="module")
def rect_case():
    subprocess.run(["./clean"],cwd=DIR)
    subprocess.run(["./run"], check=True, cwd=DIR)
    return DIR

def test_rect(rect_case):
    alphaT=5e-5

    with open(rect_case / '50/ampholyte.TARTRAZINE','r') as f:
        for _ in range(21):
            line=f.readline()
        line=f.readline()
        ndata=int(line)
        line=f.readline()
        field=np.fromfile(f,count=ndata,dtype='float',sep=' ')

    with open(rect_case / '0/Cx','r') as f:
        for _ in range(21):
            line=f.readline()
        line=f.readline()
        ndata=int(line)
        line=f.readline()
        xnod=np.fromfile(f,count=ndata,dtype='float',sep=' ')

    with open(rect_case / '0/Cy','r') as f:
        for _ in range(21):
            line=f.readline()
        line=f.readline()
        ndata=int(line)
        line=f.readline()
        ynod=np.fromfile(f,count=ndata,dtype='float',sep=' ')

    start=int(0.6*ndata)
    stop=int(0.8*ndata)
    ndata=stop-start
    e=np.zeros(ndata)
    for i in range(start,stop):
        e[i-start]=abs(field[i]-0.5*math.erfc(ynod[i]/(2*(alphaT*xnod[i])**0.5)))

    assert(np.linalg.norm(e)<1e-3*ndata)
