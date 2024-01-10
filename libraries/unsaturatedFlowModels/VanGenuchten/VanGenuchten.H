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
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("VanGenuchten");
    #pragma clang diagnostic pop

    VanGenuchten
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const phaseFractionField& frac,
        const dictionary& transportProperties
    );

    tmp<volScalarField> C() override;

    tmp<volScalarField> M() override;

private:
    const porousMedium& medium_;
    const fluidPhase& phase_;
    const phaseFractionField& frac_;
    const volScalarField pc0_;
    const volScalarField m_;
    const volScalarField l_;
};

}
}
}

#endif
