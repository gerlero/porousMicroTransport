ARG OPENFOAM_IMAGE=opencfd/openfoam-dev
ARG OPENFOAM_VERSION=latest

ARG OPENFOAM_ARM_IMAGE=gerlero/openfoam-dev

FROM ${OPENFOAM_IMAGE}:${OPENFOAM_VERSION} as openfoam-amd64
FROM ${OPENFOAM_ARM_IMAGE}:${OPENFOAM_VERSION} as openfoam-arm64

FROM openfoam-${TARGETARCH}

ARG PMT_DIR=/usr/local/porousMicroTransport

ENV PMT_SRC=${PMT_DIR}/libraries
ENV PMT_SOLVERS=${PMT_DIR}/solvers
ENV PMT_TUTORIALS=${PMT_DIR}/tutorials

COPY libraries ${PMT_SRC}
COPY solvers ${PMT_SOLVERS}
COPY Allwmake ${PMT_DIR}/

RUN . /openfoam/profile.rc \
# build and install for all users
 && ${PMT_DIR}/Allwmake -j -prefix=group \
# clean up
 && wclean all ${PMT_DIR} \
# smoke test
 && moistureDiffusivityTransportFoam -help

COPY tutorials ${PMT_TUTORIALS}
COPY LICENSE ${PMT_DIR}/
