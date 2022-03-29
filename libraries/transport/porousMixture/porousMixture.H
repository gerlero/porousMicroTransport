#ifndef PMT_porousMixture_H
#define PMT_porousMixture_H

#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"
#include "dispersionModel.H"
#include "phaseFractionFieldFwd.H"

#include <basicMultiComponentMixture.H>
#include <dictionary.H>
#include <tmp.H>
#include <volFields.H>
#include <labelFwd.H>
#include <PtrList.H>

namespace Foam
{
namespace Pmt
{

class porousMixture
:
    public basicMultiComponentMixture
{
public:
    TypeName("porousMixture");

    porousMixture
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const dictionary& transportProperties
    );

    tmp<volTensorField> Deff
    (
        label speciesi,
        const volVectorField& U,
        const phaseFractionField& frac
    );

private:
    PtrList<dispersionModel> dispersionModels_;
};

}
}

#endif
