ARG OPENFOAM_IMAGE=opencfd/openfoam-dev
ARG OPENFOAM_VERSION=latest

FROM ${OPENFOAM_IMAGE}:${OPENFOAM_VERSION}

ARG PMT_DIR=/usr/local/porousMicroTransport

ENV PMT_SRC=${PMT_DIR}/libraries
ENV PMT_SOLVERS=${PMT_DIR}/solvers
ENV PMT_TUTORIALS=${PMT_DIR}/tutorials

COPY libraries ${PMT_SRC}
COPY solvers ${PMT_SOLVERS}
COPY Allwmake ${PMT_DIR}/

RUN . /openfoam/profile.rc \
# install for all users
 && export FOAM_MODULE_APPBIN=$FOAM_SITE_APPBIN \
 && export FOAM_MODULE_LIBBIN=$FOAM_SITE_LIBBIN \
# build
 && ${PMT_DIR}/Allwmake -j \
# clean up
 && wclean all \
# smoke test
 && moistureDiffusivityTransportFoam -help

COPY tutorials ${PMT_TUTORIALS}
COPY LICENSE ${PMT_DIR}/
