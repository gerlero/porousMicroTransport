#ifndef PMT_moistureDiffusivityModel_H
#define PMT_moistureDiffusivityModel_H

#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"
#include "phaseFractionFieldFwd.H"

#include <typeInfo.H>
#include <runTimeSelectionTables.H>
#include <autoPtr.H>
#include <dictionary.H>
#include <volFieldsFwd.H>
#include <tmp.H>

namespace Foam
{
namespace Pmt
{

class moistureDiffusivityModel
{
public:
    TypeName("moistureDiffusivityModel");

    declareRunTimeSelectionTable
    (
        autoPtr,
        moistureDiffusivityModel,
        dictionary,
        (
            const porousMedium& medium,
            const fluidPhase& phase,
            const phaseFractionField& frac,
            const dictionary& transportProperties
        ),
        (medium, phase, frac, transportProperties)
    );

    static autoPtr<moistureDiffusivityModel> New
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const phaseFractionField& frac,
        const dictionary& transportProperties
    );

    moistureDiffusivityModel() = default;

    virtual tmp<volScalarField> D() = 0;

    virtual ~moistureDiffusivityModel() = default;

    moistureDiffusivityModel(const moistureDiffusivityModel&) = delete;
    void operator=(const moistureDiffusivityModel&) = delete;
};

}
}

#endif
