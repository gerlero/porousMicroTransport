#ifndef PMT_porousMixture_H
#define PMT_porousMixture_H

#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"
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
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("porousMixture");
    #pragma clang diagnostic pop

    porousMixture
    (
        const porousMedium& medium,
        const fluidPhase& phase,
        const dictionary& transportProperties
    );

    void correct(const volVectorField& U, const phaseFractionField& frac);

    tmp<volTensorField> Deff(label speciesi) const;

    const volScalarField& retardation(label speciesi) const;

private:
    const porousMedium& medium_;
    volTensorField Disp_;
    List<dimensionedScalar> Dm_;
    List<dimensionedScalar> Kd_;
    PtrList<volScalarField> Rd_;
};

}
}

#include "porousMixtureI.H"

#endif
