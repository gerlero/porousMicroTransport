#ifndef PMT_BrooksAndCorey_H
#define PMT_BrooksAndCorey_H

#include "RichardsModel.H"
#include "capillarityModel.H"
#include "relativePermeabilityModel.H"
#include "porousMedium.H"

#include <volFields.H>

namespace Foam
{
namespace Pmt
{
namespace unsaturatedFlowModels
{

class BrooksAndCorey 
:
    public RichardsModel,
    public capillarityModel,
    public relativePermeabilityModel
{

public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("BrooksAndCorey");
    #pragma clang diagnostic pop

    BrooksAndCorey
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
    const volScalarField alpha_;
    const volScalarField n_;
    const volScalarField l_;
};

}
}
}

#endif
