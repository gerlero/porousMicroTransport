#ifndef PMT_capillarityModel_H
#define PMT_capillarityModel_H

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

class capillarityModel
{
public:
    TypeName("capillarityModel");

    declareRunTimeSelectionTable
    (
        autoPtr,
        capillarityModel,
        dictionary,
        (
            const porousMedium& medium,
            const fluidPhase& phase,
            const dictionary& coeffs
        ),
        (medium, phase, coeffs)
    );

    capillarityModel() = default;

    virtual tmp<volScalarField> C(const phaseFractionField& frac) = 0;

    virtual ~capillarityModel() = default;

    capillarityModel(const capillarityModel&) = delete;
    void operator=(const capillarityModel&) = delete;
};

}
}

#endif
