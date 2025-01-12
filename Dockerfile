ARG OPENFOAM_VERSION=2412

FROM microfluidica/openfoam:${OPENFOAM_VERSION} AS dev

ARG PMT_DIR=/usr/local/src/porousMicroTransport

ENV PMT_SRC=${PMT_DIR}/libraries
ENV PMT_SOLVERS=${PMT_DIR}/solvers
ENV PMT_TUTORIALS=${PMT_DIR}/tutorials

COPY libraries ${PMT_SRC}
COPY solvers ${PMT_SOLVERS}
COPY Allwmake Allwclean LICENSE ${PMT_DIR}/

# build and install for all users 
RUN ${PMT_DIR}/Allwmake -j -prefix=group \
# clean up
 && ${PMT_DIR}/Allwclean \
# smoke test
 && moistureDiffusivityTransportFoam -help


FROM microfluidica/openfoam:${OPENFOAM_VERSION}-slim AS slim

ARG OPENFOAM_VERSION

COPY --from=dev /usr/lib/openfoam/openfoam${OPENFOAM_VERSION}/site/ /usr/lib/openfoam/openfoam${OPENFOAM_VERSION}/site/

# smoke test
RUN moistureDiffusivityTransportFoam -help


FROM dev

COPY tutorials ${PMT_TUTORIALS}
