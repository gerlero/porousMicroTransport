#ifndef PMT_relativePermeabilityModel_H
#define PMT_relativePermeabilityModel_H

#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"
#include "phaseFractionFieldFwd.H"

#include <typeInfo.H>
#include <runTimeSelectionTables.H>
#include <autoPtr.H>
#include <dictionary.H>
#include <tmp.H>
#include <volFieldsFwd.H>

namespace Foam
{
namespace Pmt
{

class relativePermeabilityModel
{
public:
    TypeName("relativePermeabilityModel");

    declareRunTimeSelectionTable
    (
        autoPtr,
        relativePermeabilityModel,
        dictionary,
        (
            const porousMedium& medium,
            const fluidPhase& phase,
            const dictionary& coeffs
        ),
        (medium, phase, coeffs)
    );

    relativePermeabilityModel() = default;

    virtual tmp<volScalarField> M(const phaseFractionField& frac) = 0;

    virtual ~relativePermeabilityModel() = default;

    relativePermeabilityModel(const relativePermeabilityModel&) = delete;
    void operator=(const relativePermeabilityModel&) = delete;
};

}
}

#endif
