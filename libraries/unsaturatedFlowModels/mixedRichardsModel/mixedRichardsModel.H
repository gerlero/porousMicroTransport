#ifndef PMT_mixedRichardsModel_H
#define PMT_mixedRichardsModel_H

#include "RichardsModel.H"
#include "capillarityModel.H"
#include "relativePermeabilityModel.H"

#include <autoPtr.H>

namespace Foam
{
namespace Pmt
{

class mixedRichardsModel final
:
    public RichardsModel
{
public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("mixedRichardsModel");
    #pragma clang diagnostic pop

    mixedRichardsModel
    (
        autoPtr<capillarityModel> capillarity,
        autoPtr<relativePermeabilityModel> relativePermeability
    );

    tmp<volScalarField> C(const phaseFractionField& frac) override;

    tmp<volScalarField> M(const phaseFractionField& frac) override;

private:
    autoPtr<capillarityModel> capillarityModel_;
    autoPtr<relativePermeabilityModel> relativePermeabilityModel_;
};

}
}

#endif
