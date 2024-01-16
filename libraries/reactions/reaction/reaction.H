#ifndef PMT_reaction_H
#define PMT_reaction_H

#include <speciesTable.H>
#include <basicMultiComponentMixture.H>
#include <volFieldsFwd.H>
#include <dimensionSet.H>
#include <scalar.H>


namespace Foam
{
namespace Pmt
{

class reaction
{
public:
    struct speciesCoeffs
    {
        label index;
        scalar stoichCoeff;
        scalar exponent;

        speciesCoeffs() = default;

        speciesCoeffs(const speciesTable& species, Istream& is);

        void write(Ostream& os, const speciesTable& species) const;
    };

    TypeName("reaction");

    declareRunTimeSelectionTable
    (
        autoPtr,
        reaction,
        dictionary,
        (
            const basicMultiComponentMixture& composition,
            const dictionary& reactionDict
        ),
        (composition, reactionDict)
    );

    static autoPtr<reaction> New
    (
        const basicMultiComponentMixture& composition,
        const dictionary& reactionDict
    );

    reaction(List<reaction::speciesCoeffs> lhs, List<reaction::speciesCoeffs> rhs);
    reaction(const string& str, const speciesTable& species);

    List<reaction::speciesCoeffs> lhs() const;
    List<reaction::speciesCoeffs> rhs() const;

    virtual tmp<volScalarField> kf() const = 0;
    virtual tmp<volScalarField> kr() const = 0;

    virtual ~reaction() = default;

    reaction(const reaction&) = delete;
    void operator=(const reaction&) = delete;

protected:
    List<reaction::speciesCoeffs> lhs_;
    List<reaction::speciesCoeffs> rhs_;

    dimensionSet YDimensions(const basicMultiComponentMixture& composition) const;
    dimensionSet kfDimensions(const dimensionSet& dimY) const;
    dimensionSet krDimensions(const dimensionSet& dimY) const;

    void write
    (
        Ostream& os,
        const speciesTable& species
    );

private:
    reaction(Pair<List<reaction::speciesCoeffs>> lrhs);

    scalar forwardOrder() const noexcept;
    scalar reverseOrder() const noexcept;

    static Pair<List<reaction::speciesCoeffs>> read
    (
        const speciesTable& species,
        Istream& is
    );

    static List<reaction::speciesCoeffs> readSide
    (
        const speciesTable& species,
        Istream& is
    );

    static void writeSide
    (
        Ostream& os,
        const List<reaction::speciesCoeffs>& s,
        const speciesTable& species
    );
};

}
}

#endif
