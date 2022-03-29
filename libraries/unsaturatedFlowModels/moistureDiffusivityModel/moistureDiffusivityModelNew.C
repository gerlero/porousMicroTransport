#include "moistureDiffusivityModel.H"
#include "unsaturatedFlowModelNew.H"

Foam::autoPtr<Foam::Pmt::moistureDiffusivityModel> Foam::Pmt::moistureDiffusivityModel::New
(
    const porousMedium& medium,
    const fluidPhase& phase,
    const dictionary& transportProperties
)
{
    return Detail::unsaturatedFlowModelNew<moistureDiffusivityModel>(medium, phase, transportProperties);
}
