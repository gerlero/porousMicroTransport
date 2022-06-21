#ifndef PMT_LETd_H
#define PMT_LETd_H

#include "moistureDiffusivityModel.H"

#include <volFields.H>
#include <dimensionedScalar.H>

namespace Foam
{
namespace Pmt
{
namespace unsaturatedFlowModels
{

class LETd 
:
    public moistureDiffusivityModel
{

public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("LETd");
    #pragma clang diagnostic pop

    LETd
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const dictionary& coeffs
    );

    tmp<volScalarField> D(const phaseFractionField& frac) override;

private:
    const volScalarField Dwt_;
    const dimensionedScalar L_;
    const dimensionedScalar E_;
    const dimensionedScalar T_;
};

}
}
}

#endif
