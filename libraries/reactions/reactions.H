#ifndef PMT_reactions_H
#define PMT_reactions_H

#include "reaction.H"

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
    const basicMultiComponentMixture& composition_;
    PtrList<volScalarField> reactionTerms_;
    PtrList<reaction> reactions_;

    void clearTerms();

    void setReactionRate
    (
        const List<reaction::speciesCoeffs>& lhs,
        const List<reaction::speciesCoeffs>& rhs, 
        tmp<volScalarField> k
    );
};

}
}

#include "reactionsI.H"

#endif
