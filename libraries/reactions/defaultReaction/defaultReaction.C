#include "defaultReaction.H"

#include <className.H>
#include <addToRunTimeSelectionTable.H>
#include <messageStream.H>

namespace Foam
{
namespace Pmt
{
defineTypeNameAndDebug(defaultReaction, 0);
addToRunTimeSelectionTable(reaction, defaultReaction, dictionary);
}
}

Foam::Pmt::defaultReaction::defaultReaction
(
    const basicMultiComponentMixture& composition,
    const dictionary& reactionDict
)
:
    reaction{reactionDict.get<string>("reaction"), composition.species()},
    mesh_{composition.Y().first().mesh()},
    kf_{"kf", kfDimensions(YDimensions(composition)), reactionDict},
    kr_{dimensionedScalar::getOrDefault("kr", reactionDict, krDimensions(YDimensions(composition)), Zero)}
{
    Info<< "Reaction " << reactionDict.name() << nl
        << "    "; write(Info, composition.species()); Info<< nl
        << "    kf = " << kf_.value() << endl;
    if (kr_.value() != 0)
    {
        Info<< "    kr = " << kr_.value() << endl;
    }
}

Foam::tmp<Foam::volScalarField> Foam::Pmt::defaultReaction::kf() const
{
    return volScalarField::New("kf", mesh_, kf_);
}

Foam::tmp<Foam::volScalarField> Foam::Pmt::defaultReaction::kr() const
{
    return volScalarField::New("kr", mesh_, kr_);
}
