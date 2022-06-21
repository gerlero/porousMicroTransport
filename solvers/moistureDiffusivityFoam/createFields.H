Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
{
    IOobject
    {
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    }
};

Info<< "Reading porous medium properties\n" << endl;

Pmt::porousMedium medium
{
    mesh,
    transportProperties
};

Info<< "Reading phase properties\n" << endl;

Pmt::fluidPhase phase
{
    transportProperties,
    "theta"
};

Info<< "Reading moisture content field (theta)\n" << endl;

Pmt::phaseFractionField theta
{
    transportProperties,
    IOobject
    {
        "theta",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    },
    mesh
};

Info<< "Reading velocity field Utheta\n" << endl;

volVectorField U
{
    IOobject
    {
        "Utheta",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    },
    mesh,
    dimensionedVector{dimVelocity, Zero}
};

#include "createPhi.H"

mesh.setFluxRequired(theta.name());

auto flowModel = Pmt::moistureDiffusivityModel::New(medium, phase, transportProperties);

#include "createFvOptions.H"
