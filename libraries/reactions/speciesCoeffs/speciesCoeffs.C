#include "speciesCoeffs.H"

#include <word.H>
#include <IStringStream.H>
#include <DynamicList.H>
#include <error.H>

#include <cassert>

Foam::List<Foam::Pmt::speciesCoeffs> Foam::Pmt::speciesCoeffs::readReactionSide
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

Foam::Pair<Foam::List<Foam::Pmt::speciesCoeffs>> 
Foam::Pmt::speciesCoeffs::readReactionEqn
(
    const speciesTable& species,
    Istream& is
)
{
    auto lhs = readReactionSide(species, is);

    if (token{is} != token::ASSIGN)
    {
        FatalIOErrorInFunction(is)
            << "Expected '=' sign in reaction" << endl
            << exit(FatalIOError);
    }

    auto rhs = readReactionSide(species, is);

    return {lhs, rhs};
}

Foam::Pair<Foam::List<Foam::Pmt::speciesCoeffs>> 
Foam::Pmt::speciesCoeffs::readReactionEqn
(
    const speciesTable& species,
    const string& str
)
{
    IStringStream is{str};

    return readReactionEqn(species, is);
}


void Foam::Pmt::speciesCoeffs::write
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

void Foam::Pmt::speciesCoeffs::writeReactionSide
(
    Ostream& os,
    const List<speciesCoeffs>& s,
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

void Foam::Pmt::speciesCoeffs::writeReactionEqn
(
    Ostream& os,
    const List<speciesCoeffs>& lhs,
    const List<speciesCoeffs>& rhs,
    const speciesTable& species
)
{
    writeReactionSide(os, lhs, species);
    os << " = ";
    writeReactionSide(os, rhs, species);
}


Foam::Pmt::speciesCoeffs::speciesCoeffs
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
