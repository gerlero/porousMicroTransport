#ifndef PMT_reactions_H
#define PMT_reactions_H

#include "speciesCoeffs.H"

#include <basicMultiComponentMixture.H>
#include <volFieldsFwd.H>
#include <label.H>
#include <PtrList.H>
#include <Pair.H>
#include <List.H>
#include <scalar.H>
#include <dimensionSet.H>

namespace Foam
{
namespace Pmt
{

class reactions
{
public:
    reactions
    (
        const basicMultiComponentMixture& composition,
        const dictionary& transportProperties
    );

    void correct();

    const volScalarField& reactionTerm(label speciesi) const;

    reactions(const reactions&) = delete;
    void operator=(const reactions&) = delete;

private:
    struct reaction
    {
        List<speciesCoeffs> lhs;
        List<speciesCoeffs> rhs;
        scalar kf;
        scalar kr;
    };

    const basicMultiComponentMixture& composition_;
    PtrList<volScalarField> reactionTerms_;
    List<reaction> reactions_;

    void clearTerms();
    void setReactionRate
    (
        const List<speciesCoeffs>& lhs,
        const List<speciesCoeffs>& rhs, 
        scalar k
    );

    static dimensionSet kDimensions
    (
        const List<speciesCoeffs>& lhs,
        const List<speciesCoeffs>& rhs,
        const basicMultiComponentMixture& composition
    );
};

}
}

#include "reactionsI.H"

#endif
