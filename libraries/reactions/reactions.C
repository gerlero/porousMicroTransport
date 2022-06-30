#include "reactions.H"
#include "dictionaries.H"

#include <volFields.H>
#include <DynamicList.H>
#include <messageStream.H>
#include <error.H>

#include <cassert>

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
            DynamicList<reaction> list{};
            
            const auto& reactionsDict =
                dictionaries::subOrNullDict(transportProperties, "reactions");
            
            Info<< reactionsDict.size() << " reactions found" << nl
                << endl;

            for (const auto& entry : reactionsDict)
            {
                const auto& dict = entry.dict();

                Info<< "Reaction " << dict.dictName() << nl
                    << "{" << endl;

                auto lrhs =
                    speciesCoeffs::readReactionEqn
                    (
                        composition.species(),
                        dict.get<string>("reaction")
                    );

                Info<< "    ";
                speciesCoeffs::writeReactionEqn
                (
                    Info,
                    lrhs.first(),
                    lrhs.second(),
                    composition.species()
                );
                Info<< nl
                    << endl;

                auto kf = 
                    dimensionedScalar
                    {
                        "kf",
                        kDimensions(lrhs.first(), lrhs.second(), composition),
                        dict
                    }.value();
                
                Info<< "    kf (->) = " << kf << endl;

                auto kr =
                    dimensionedScalar::getOrDefault
                    (
                        "kr",
                        dict,
                        kDimensions(lrhs.second(), lrhs.first(), composition),
                        Zero
                    ).value();

                if (kr != 0)
                {
                    Info<< "    kr (<-) = " << kr << nl;
                }
                Info<< "}" << nl
                    << endl;

                list.append
                (
                    reaction
                    {
                        std::move(lrhs.first()),
                        std::move(lrhs.second()),
                        kf,
                        kr
                    }
                );
            }

            return list;
        }()
    }
{}

void Foam::Pmt::reactions::correct()
{
    clearTerms();

    for (const auto& reaction : reactions_)
    {
        setReactionRate(reaction.lhs, reaction.rhs, reaction.kf);

        if (reaction.kr != 0)
        {
            setReactionRate(reaction.rhs, reaction.lhs, reaction.kr);
        }
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
    const List<speciesCoeffs>& lhs,
    const List<speciesCoeffs>& rhs,
    scalar k
)
{
    auto tReactionRate = volScalarField::New
        (
            "k",
            composition_.Y(0).mesh(),
            k
        );

    auto& reactionRate = tReactionRate.ref();

    for (const auto& sc : lhs)
    {
        const auto& Y = composition_.Y(sc.index);
        const auto& dimY = Y.dimensions();

        reactionRate *= pow(Y/dimensionedScalar{dimY, One}, sc.exponent);
    }

    for (const auto& sc : lhs)
    {
        auto& term = reactionTerms_[sc.index];
        const auto& dimTerm = term.dimensions();

        term -= sc.stoichCoeff*reactionRate*dimensionedScalar{dimTerm, One};
    }

    for (const auto& sc : rhs)
    {
        auto& term = reactionTerms_[sc.index];
        const auto& dimTerm = term.dimensions();

        term += sc.stoichCoeff*reactionRate*dimensionedScalar{dimTerm, One};
    }
}

Foam::dimensionSet Foam::Pmt::reactions::kDimensions
(
    const List<speciesCoeffs>& lhs,
    const List<speciesCoeffs>& rhs,
    const basicMultiComponentMixture& composition
)
{
    assert(!lhs.empty());

    auto dims = dimless;

    for (const auto& sc : lhs)
    {
        dims *= pow(composition.Y(sc.index).dimensions(), sc.exponent);
    }

    const auto& Ydims = composition.Y(lhs.first().index).dimensions();
    for (const auto& sc : lhs)
    {
        if (composition.Y(sc.index).dimensions() != Ydims)
        {
            FatalErrorInFunction
                << "Concentration fields involved in reaction have different dimensions" << nl
                << "Expected: " << Ydims
                << ". Actual (species " << composition.Y(sc.index).name()
                << ": " << composition.Y(sc.index).dimensions() << ")"
                << endl
                << exit(FatalError);
        }
    }

    dims /= Ydims*dimTime;

    return dims;
}
