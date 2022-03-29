#ifndef PMT_RichardsModel_H
#define PMT_RichardsModel_H

#include "moistureDiffusivityModel.H"
#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"

#include <runTimeSelectionTables.H>
#include <autoPtr.H>
#include <dictionary.H>

namespace Foam
{
namespace Pmt
{

class RichardsModel
:
    public moistureDiffusivityModel
{
public:
    TypeName("RichardsModel");

    declareRunTimeSelectionTable
    (
        autoPtr,
        RichardsModel,
        dictionary,
        (
            const porousMedium& medium,
            const fluidPhase& phase,
            const dictionary& coeffs
        ),
        (medium, phase, coeffs)
    );

    static autoPtr<RichardsModel> New
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const dictionary& transportProperties
    );

    virtual tmp<volScalarField> C(const phaseFractionField& frac) = 0;

    virtual tmp<volScalarField> M(const phaseFractionField& frac) = 0;

    tmp<volScalarField> D(const phaseFractionField& frac) override;
};

}
}

#endif
