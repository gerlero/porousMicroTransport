#include "reaction.H"

#include <word.H>
#include <IStringStream.H>
#include <DynamicList.H>
#include <error.H>
#include <className.H>
#include <addToRunTimeSelectionTable.H>

#include <cassert>
#include <utility>

void Foam::Pmt::reaction::speciesCoeffs::write
(
    Ostream& os,
    const speciesTable& species
) const
{
    if (mag(stoichCoeff - 1) > SMALL)
    {
        os << stoichCoeff;
    }
    os << species[index];
    if (mag(exponent - stoichCoeff) > SMALL)
    {
        os << "^" << exponent;
    }
}

Foam::Pmt::reaction::speciesCoeffs::speciesCoeffs
(
    const speciesTable& species,
    Istream& is
)
{
    token t{is};

    if (t.isNumber())
    {
        stoichCoeff = t.number();
        is >> t;
    }
    else
    {
        stoichCoeff = 1.0;
    }

    if (t.isWord())
    {
        auto name = t.wordToken();

        auto i = name.find('^');

        if (i != word::npos)
        {
            exponent = readScalar(name.substr(i + 1));
            name.resize(i);
        }
        else
        {
            exponent = stoichCoeff;
        }

        index = species[name];

        if (index < 0)
        {
            FatalIOErrorInFunction(is)
                << "Unknown species " << name << endl
                << exit(FatalIOError);
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "Expected a word but found " << t.info() << endl
            << exit(FatalIOError);
    }

}

namespace Foam
{
namespace Pmt
{
defineTypeNameAndDebug(reaction, 0);
defineRunTimeSelectionTable(reaction, dictionary)
}
}

Foam::Pmt::reaction::reaction
(
    const string& str,
    const speciesTable& species
)
:
    reaction
    {
        [&]
        {
            IStringStream is{str};

            return read(species, is);
        }()
    }
{}

Foam::List<Foam::Pmt::reaction::speciesCoeffs> Foam::Pmt::reaction::lhs() const
{
    return lhs_;
}

Foam::List<Foam::Pmt::reaction::speciesCoeffs> Foam::Pmt::reaction::rhs() const
{
    return rhs_;
}

Foam::dimensionSet Foam::Pmt::reaction::YDimensions
(
    const basicMultiComponentMixture& composition
) const
{
    dimensionSet dimY;

    if (lhs().empty())
    {
        dimY.reset(composition.Y(rhs().first().index).dimensions());
    }
    else if (rhs().empty())
    {
        dimY.reset(composition.Y(lhs().first().index).dimensions());
    }
    else if (!lhs().empty() && !rhs().empty())
    {
        dimY.reset(composition.Y(lhs().first().index).dimensions());
        
        if (composition.Y(rhs().first().index).dimensions() != dimY)
        {
            FatalErrorInFunction
                << "Incompatible dimensions of species concentrations in reaction" << nl
                << abort(FatalError);
        }
    }
    else
    {
        FatalErrorInFunction
            << "Empty reaction" << nl
            << abort(FatalError);
    }

    for (const auto& sc : lhs())
    {
        if (composition.Y(sc.index).dimensions() != dimY)
        {
            FatalErrorInFunction
                << "Incompatible dimensions of species concentrations in reaction" << nl
                << abort(FatalError);
        }
    }

    for (const auto& sc : rhs())
    {
        if (composition.Y(sc.index).dimensions() != dimY)
        {
            FatalErrorInFunction
                << "Incompatible dimensions of species concentrations in reaction" << nl
                << abort(FatalError);
        }
    }

    return dimY;
}   

Foam::dimensionSet Foam::Pmt::reaction::kfDimensions(const dimensionSet& dimY) const
{
    return dimless/(dimTime*pow(dimY, forwardOrder() - 1));
}

Foam::dimensionSet Foam::Pmt::reaction::krDimensions(const dimensionSet& dimY) const
{
    return dimless/(dimTime*pow(dimY, reverseOrder() - 1));
}

void Foam::Pmt::reaction::write
(
    Ostream& os,
    const speciesTable& species
)
{
    writeSide(os, lhs(), species);
    os << " = ";
    writeSide(os, rhs(), species);
}

Foam::Pmt::reaction::reaction
(
    Pair<List<reaction::speciesCoeffs>> lrhs
)
:
    lhs_{std::move(lrhs.first())},
    rhs_{std::move(lrhs.second())}
{}

Foam::scalar Foam::Pmt::reaction::forwardOrder() const noexcept
{
    scalar order = 0;

    for (const auto& sc : lhs())
    {
        order += sc.exponent;
    }

    return order;
}

Foam::scalar Foam::Pmt::reaction::reverseOrder() const noexcept
{
    scalar order = 0;

    for (const auto& sc : rhs())
    {
        order += sc.exponent;
    }

    return order;
}

Foam::Pair<Foam::List<Foam::Pmt::reaction::speciesCoeffs>> 
Foam::Pmt::reaction::read
(
    const speciesTable& species,
    Istream& is
)
{
    auto lhs = readSide(species, is);

    if (token{is} != token::ASSIGN)
    {
        FatalIOErrorInFunction(is)
            << "Expected '=' sign in reaction" << endl
            << exit(FatalIOError);
    }

    auto rhs = readSide(species, is);

    return {lhs, rhs};
}


Foam::List<Foam::Pmt::reaction::speciesCoeffs> Foam::Pmt::reaction::readSide
(
    const speciesTable& species,
    Istream& is
)
{
    DynamicList<speciesCoeffs> ret{};

    do
    {
        ret.append(speciesCoeffs{species, is});

        token t{is};
        if (t != token::ADD)
        {
            is.putBack(t);
            return ret;
        }
    } while(is);

    FatalIOErrorInFunction(is)
        << "Cannot continue reading reaction data from stream" << endl
        << exit(FatalIOError);

    assert(false);
}

void Foam::Pmt::reaction::writeSide
(
    Ostream& os,
    const List<reaction::speciesCoeffs>& s,
    const speciesTable& species
)
{
    forAll(s, i)
    {
        if (i > 0)
        {
            os << " + ";
        }
        s[i].write(os, species);
    }
}
