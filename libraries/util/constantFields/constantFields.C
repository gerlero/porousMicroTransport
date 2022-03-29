#include "constantFields.H"

#include <volFields.H>
#include <dimensionedScalar.H>
#include <scalar.H>
#include <error.H>

Foam::tmp<Foam::volScalarField> Foam::Pmt::constantFields::read
(
    const word& name,
    const fvMesh& mesh,
    const dimensionSet& dims,
    const dictionary& dict
)
{
    auto field = readIfPresent(name, mesh, dims, dict);

    if (!field)
    {
        if (dict == dictionary::null)
        {
            FatalErrorInFunction
                << "Missing constant field " << name << endl
                << exit(FatalError);
        }

        FatalErrorInFunction
            << name << " is neither the name of a constant field nor a dictionary entry in " << dict.name() << endl
            << exit(FatalError);
    }

    return field;
}

Foam::tmp<Foam::volScalarField> Foam::Pmt::constantFields::readIfPresent
(
    const word& name,
    const fvMesh& mesh,
    const dimensionSet& dims,
    const dictionary& dict
)
{
    auto field = tmp<volScalarField>::New
    (
        IOobject
        {
            name,
            mesh.time().constant(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        },
        mesh,
        dims
    );

    dimensionedScalar val(name, dims, 0);

    if (val.readIfPresent(dict))
    {
        if (field->headerOk())
        {
            FatalErrorInFunction
                << "Duplicate definition of " << name << nl
                << name << " is both a constant field and an entry in dictionary " << dict.name() << nl
                << "You must remove one of the conflicting definitions" << endl
                << exit(FatalError);
        }

        return volScalarField::New(name, mesh, val);
    }

    if (!field->headerOk())
    {
        return nullptr;
    }

    return field;
}

Foam::tmp<Foam::volScalarField> Foam::Pmt::constantFields::readOrDefault
(
    const word& name,
    const fvMesh& mesh,
    const dimensionSet& dims,
    scalar val,
    const dictionary& dict
)
{
    if (auto field = readIfPresent(name, mesh, dims, dict))
    {
        return field;
    }

    return volScalarField::New(name, mesh, dimensionedScalar(dims, val));
}

Foam::tmp<Foam::volScalarField> Foam::Pmt::constantFields::readOrDefault
(
    const word& name,
    const volScalarField& deflt,
    const dictionary& dict
)
{
    if (auto field = readIfPresent(name, deflt.mesh(), deflt.dimensions(), dict))
    {
        return field;
    }

    return volScalarField::New(name, deflt);
}
