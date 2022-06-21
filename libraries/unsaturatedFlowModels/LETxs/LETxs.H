#ifndef PMT_LETxs_H
#define PMT_LETxs_H

#include "moistureDiffusivityModel.H"

#include <volFields.H>
#include <dimensionedScalar.H>

namespace Foam
{
namespace Pmt
{
namespace unsaturatedFlowModels
{

class LETxs 
:
    public moistureDiffusivityModel
{

public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("LETxs");
    #pragma clang diagnostic pop

    LETxs
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const dictionary& coeffs
    );

    tmp<volScalarField> D(const phaseFractionField& frac) override;

private:
    const porousMedium& medium_;
    const fluidPhase& phase_;
    const volScalarField pc0_;
    const dimensionedScalar Lw_;
    const dimensionedScalar Ew_;
    const dimensionedScalar Tw_;
    const dimensionedScalar Ls_;
    const dimensionedScalar Es_;
    const dimensionedScalar Ts_;
};

}
}
}

#endif
