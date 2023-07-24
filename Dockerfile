FROM opencfd/openfoam-dev:2306

ARG PMT_DIR=/usr/local/porousMicroTransport

ENV PMT_SRC=${PMT_DIR}/libraries
ENV PMT_SOLVERS=${PMT_DIR}/solvers
ENV PMT_TUTORIALS=${PMT_DIR}/tutorials

COPY libraries ${PMT_SRC}
COPY solvers ${PMT_SOLVERS}
COPY Allwmake ${PMT_DIR}/
COPY Allwclean ${PMT_DIR}/

RUN . /openfoam/profile.rc \
# build and install for all users
 && ${PMT_DIR}/Allwmake -j -prefix=group \
# clean up
 && ${PMT_DIR}/Allwclean \
# smoke test
 && moistureDiffusivityTransportFoam -help

COPY tutorials ${PMT_TUTORIALS}
COPY LICENSE ${PMT_DIR}/
