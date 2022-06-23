import numpy as np
import math
alphaT=5e-5
probes=np.arange(300,16000,400)

f=open('./50/ampholyte.TARTRAZINE','r')
for dummy_lines in range(21):
    line=f.readline()
line=f.readline()
ndata=int(line)
line=f.readline()
field=np.fromfile(f,count=ndata,dtype='float',sep=' ')
f.close()
f=open('./0/Cx','r')
for dummy_lines in range(21):
    line=f.readline()
line=f.readline()
ndata=int(line)
line=f.readline()
xnod=np.fromfile(f,count=ndata,dtype='float',sep=' ')
f.close()
f=open('./0/Cy','r')
for dummy_lines in range(21):
    line=f.readline()
line=f.readline()
ndata=int(line)
line=f.readline()
ynod=np.fromfile(f,count=ndata,dtype='float',sep=' ')
f.close()

start=int(0.6*ndata)
stop=int(0.8*ndata)
ndata=stop-start
e=np.zeros(ndata)
for i in range(start,stop):
    e[i-start]=abs(field[i]-0.5*math.erfc(ynod[i]/(2*(alphaT*xnod[i])**0.5)))

test_pass=np.linalg.norm(e)<1e-3*ndata

if test_pass:
    print('Rect test passed')
else:
    print('Rect test not passed')

