#ifndef PMT_VanGenuchten_H
#define PMT_VanGenuchten_H

#include "RichardsModel.H"
#include "capillarityModel.H"
#include "relativePermeabilityModel.H"

#include <volFields.H>

namespace Foam
{
namespace Pmt
{
namespace unsaturatedFlowModels
{

class VanGenuchten 
:
    public RichardsModel,
    public capillarityModel,
    public relativePermeabilityModel
{

public:
    TypeName("VanGenuchten");

    VanGenuchten
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const dictionary& coeffs
    );

    tmp<volScalarField> C(const phaseFractionField& frac) override;

    tmp<volScalarField> M(const phaseFractionField& frac) override;

private:
    const porousMedium& medium_;
    const fluidPhase& phase_;
    const volScalarField pc0_;
    const volScalarField m_;
    const volScalarField l_;
};

}
}
}

#endif
