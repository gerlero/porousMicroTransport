#include "reactions.H"
#include "dictionaries.H"

#include <volFields.H>
#include <DynamicList.H>
#include <messageStream.H>
#include <error.H>

Foam::Pmt::reactions::reactions
(
    const basicMultiComponentMixture& composition,
    const dictionary& transportProperties
)
:
    composition_{composition},
    reactionTerms_
    {
        [&composition]
        {
            PtrList<volScalarField> terms(composition.Y().size());

            forAll(composition.Y(), speciesi)
            {
                const auto& Y = composition.Y(speciesi);

                terms.set
                (
                    speciesi,
                    volScalarField::New
                    (
                        "RR",
                        Y.mesh(),
                        dimensionedScalar{Y.dimensions()/dimTime, Zero}
                    )
                );
            }

            return terms;
        }()
    },
    reactions_
    {
        [&]
        {
            PtrList<reaction> list{};

            const auto& reactionsDict =
                dictionaries::subOrNullDictRef(transportProperties, "reactions");

            Info<< reactionsDict.size() << " reactions found" << nl
                << endl;

            for (const auto& entry : reactionsDict)
            {
                const auto& dict = entry.dict();

                list.append(reaction::New(composition_, dict));
            }

            return list;
        }()
    }
{}

void Foam::Pmt::reactions::correct()
{
    clearTerms();

    for (auto& reaction : reactions_)
    {
        setReactionRate(reaction.lhs(), reaction.rhs(), reaction.kf());
        setReactionRate(reaction.rhs(), reaction.lhs(), reaction.kr());
    }
}

void Foam::Pmt::reactions::clearTerms()
{
    for (auto& reactionTerm: reactionTerms_)
    {
        reactionTerm = dimensionedScalar{reactionTerm.dimensions(), Zero};
    }
}

void Foam::Pmt::reactions::setReactionRate
(
    const List<reaction::speciesCoeffs>& lhs,
    const List<reaction::speciesCoeffs>& rhs,
    tmp<volScalarField> k
)
{
    auto& reactionRate = k.ref();

    for (const auto& sc : lhs)
    {
        const auto& Y = composition_.Y(sc.index);
        const auto& dimY = Y.dimensions();

        reactionRate *= pow(max(Y, dimensionedScalar{dimY, Zero}), sc.exponent);
    }

    for (const auto& sc : lhs)
    {
        auto& term = reactionTerms_[sc.index];

        term -= sc.stoichCoeff*reactionRate;
    }

    for (const auto& sc : rhs)
    {
        auto& term = reactionTerms_[sc.index];

        term += sc.stoichCoeff*reactionRate;
    }
}
