#ifndef PMT_speciesCoeffs_H
#define PMT_speciesCoeffs_H

#include <label.H>
#include <scalar.H>
#include <speciesTable.H>
#include <Istream.H>
#include <Ostream.H>
#include <Pair.H>
#include <List.H>

namespace Foam
{
namespace Pmt
{

struct speciesCoeffs
{
    label index;
    scalar stoichCoeff;
    scalar exponent;

    speciesCoeffs() = default;

    static Pair<List<speciesCoeffs>> readReactionEqn
    (
        const speciesTable& species,
        const string& str
    );

    static void writeReactionEqn
    (
        Ostream& os,
        const List<speciesCoeffs>& lhs,
        const List<speciesCoeffs>& rhs,
        const speciesTable& species
    );

private:
    speciesCoeffs(const speciesTable& species, Istream& is);

    void write(Ostream& os, const speciesTable& species) const;

    static Pair<List<speciesCoeffs>> readReactionEqn
    (
        const speciesTable& species,
        Istream& is
    );

    static List<speciesCoeffs> readReactionSide
    (
        const speciesTable& species,
        Istream& is
    );

    static void writeReactionSide
    (
        Ostream& os,
        const List<speciesCoeffs>& s,
        const speciesTable& species
    );
};

}
}

#endif
