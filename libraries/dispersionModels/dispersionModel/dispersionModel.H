#ifndef PMT_dispersionModel_H
#define PMT_dispersionModel_H

#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"
#include "phaseFractionFieldFwd.H"

#include <typeInfo.H>
#include <runTimeSelectionTables.H>
#include <autoPtr.H>
#include <word.H>
#include <dimensionedScalar.H>
#include <dictionary.H>
#include <tmp.H>
#include <volFieldsFwd.H>

namespace Foam
{
namespace Pmt
{

class dispersionModel
{
public:
    TypeName("dispersionModel");

    declareRunTimeSelectionTable
    (
        autoPtr,
        dispersionModel,
        dictionary,
        (
            const porousMedium& medium,
            const fluidPhase& phase,
            const word& speciesName,
            dimensionedScalar Dm,
            const dictionary& coeffs
        ),
        (medium, phase, speciesName, Dm, coeffs)
    );

    static autoPtr<dispersionModel> New
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const word& speciesName,
        dimensionedScalar Dm,
        const dictionary& speciesTransport
    );

    dispersionModel() = default;

    virtual tmp<volTensorField> Deff
    (
        const volVectorField& U,
        const phaseFractionField& frac
    ) = 0;

    virtual ~dispersionModel() = default;

    dispersionModel(const dispersionModel&) = delete;
    void operator=(const dispersionModel&) = delete;
};

}
}

#endif
