FROM opencfd/openfoam-dev:latest

COPY libraries /usr/local/porousMicroTransport/libraries
COPY solvers /usr/local/porousMicroTransport/solvers
COPY Allwmake /usr/local/porousMicroTransport/Allwmake

RUN . /openfoam/profile.rc \
# install for all users
 && export FOAM_MODULE_APPBIN=$FOAM_SITE_APPBIN \
 && export FOAM_MODULE_LIBBIN=$FOAM_SITE_LIBBIN \
# build
 && /usr/local/porousMicroTransport/Allwmake -j \
# clean up
 && wclean all \
# smoke test
 && moistureDiffusivityTransportFoam -help

COPY tutorials /usr/local/porousMicroTransport/tutorials
COPY LICENSE /usr/local/porousMicroTransport/LICENSE
