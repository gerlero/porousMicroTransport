FROM opencfd/openfoam-dev:latest

COPY Allwmake /usr/local/porousMicroTransport/Allwmake
COPY Allwclean /usr/local/porousMicroTransport/Allwclean
COPY libraries /usr/local/porousMicroTransport/libraries
COPY solvers /usr/local/porousMicroTransport/solvers

RUN . /openfoam/profile.rc \
 && /usr/local/porousMicroTransport/Allwmake -j

COPY tutorials /usr/local/porousMicroTransport/tutorials
COPY LICENSE /usr/local/porousMicroTransport/LICENSE
