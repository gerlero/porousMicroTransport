ARG FOAM_VERSION=2306

FROM opencfd/openfoam-dev:${FOAM_VERSION} as dev

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

COPY LICENSE ${PMT_DIR}/


FROM opencfd/openfoam-run:${FOAM_VERSION} as run

ARG FOAM_VERSION

COPY --from=dev /usr/lib/openfoam/openfoam${FOAM_VERSION}/site/ /usr/lib/openfoam/openfoam${FOAM_VERSION}/site/

# smoke test
RUN . /openfoam/profile.rc \
 && moistureDiffusivityTransportFoam -help


FROM dev

COPY tutorials ${PMT_TUTORIALS}
