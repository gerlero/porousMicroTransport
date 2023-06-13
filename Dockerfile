FROM opencfd/openfoam-dev:latest

COPY libraries /usr/local/porousMicroTransport/libraries
COPY solvers /usr/local/porousMicroTransport/solvers
COPY Allwmake /usr/local/porousMicroTransport/Allwmake

RUN . /openfoam/profile.rc \
 && /usr/local/porousMicroTransport/Allwmake -j \
 && wclean all

COPY tutorials /usr/local/porousMicroTransport/tutorials
COPY LICENSE /usr/local/porousMicroTransport/LICENSE
