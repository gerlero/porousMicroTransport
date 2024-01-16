#include "reaction.H"
#include "runTimeSelectionHelpers.H"

Foam::autoPtr<Foam::Pmt::reaction> Foam::Pmt::reaction::New
(
    const basicMultiComponentMixture& composition,
    const dictionary& reactionDict
)
{
    auto reactionType = reactionDict.getOrDefault<word>("type", "default");

    return NewOfSelectedType<reaction>
        (
            reactionType,
            composition,
            reactionDict
        );
}
