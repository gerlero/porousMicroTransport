#ifndef PMT_alphaDispersion_H
#define PMT_alphaDispersion_H

#include "dispersionModel.H"

#include <volFields.H>

namespace Foam
{
namespace Pmt
{
namespace dispersionModels
{

class alphaDispersion
:
    public dispersionModel
{
public:
    TypeName("alphaDispersion");

    alphaDispersion
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const word& speciesName,
        dimensionedScalar Dm,
        const dictionary& coeffs
    );

    tmp<volTensorField> Deff
    (
        const volVectorField& U,
        const phaseFractionField& frac
    ) override;

private:
    const porousMedium& medium_;
    const dimensionedScalar Dm_;
    const volScalarField tau_;
    const volScalarField alphaT_;
    const volScalarField alphaL_;
};

}
}
}

#endif
