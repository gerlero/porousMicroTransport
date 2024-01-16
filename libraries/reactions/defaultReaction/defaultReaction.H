#ifndef PMT_defaultReaction_H
#define PMT_defaultReaction_H

#include "reaction.H"

namespace Foam
{
namespace Pmt
{

class defaultReaction
:
    public reaction
{
public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("default");
    #pragma clang diagnostic pop

    defaultReaction
    (
        const basicMultiComponentMixture& composition,
        const dictionary& reactionDict
    );

    virtual tmp<volScalarField> kf() const override;
    virtual tmp<volScalarField> kr() const override;

private:
    const fvMesh& mesh_;
    const dimensionedScalar kf_;
    const dimensionedScalar kr_;
};

}
}

#endif
