#include "mixedRichardsModel.H"

#include <className.H>
#include <volFields.H>

#include <utility>
#include <cassert>

namespace Foam
{
namespace Pmt
{
defineTypeNameAndDebug(mixedRichardsModel, 0);
}
}

Foam::Pmt::mixedRichardsModel::mixedRichardsModel
(
    autoPtr<capillarityModel> capillarityModel,
    autoPtr<relativePermeabilityModel> relativePermeabilityModel
)
:
    capillarityModel_{std::move(capillarityModel)},
    relativePermeabilityModel_{std::move(relativePermeabilityModel)}
{
    assert(capillarityModel);
    assert(relativePermeabilityModel);
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::mixedRichardsModel::C(const phaseFractionField& frac)
{
    return capillarityModel_->C(frac);
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::mixedRichardsModel::M(const phaseFractionField& frac)
{
    return relativePermeabilityModel_->M(frac);
}
