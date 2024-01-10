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
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("RichardsModel");
    #pragma clang diagnostic pop

    declareRunTimeSelectionTable
    (
        autoPtr,
        RichardsModel,
        dictionary,
        (
            const porousMedium& medium,
            const fluidPhase& phase,
            const phaseFractionField& frac,
            const dictionary& transportProperties
        ),
        (medium, phase, frac, transportProperties)
    );

    static autoPtr<RichardsModel> New
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const phaseFractionField& frac,
        const dictionary& transportProperties
    );

    virtual tmp<volScalarField> C() = 0;

    virtual tmp<volScalarField> M() = 0;

    tmp<volScalarField> D() override;
};

}
}

#endif
