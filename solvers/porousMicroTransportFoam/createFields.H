Info<< "Reading velocity field U\n" << endl;

volVectorField U
{
    IOobject
    {
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    },
    mesh,
    dimVelocity
};

#include "createPhi.H"

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

Info<< "Reading moisture content field theta (if present)\n" << endl;

Pmt::phaseFractionField theta
{
    transportProperties,
    IOobject
    {
        "theta",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    },
    medium.eps()
};

#include "createFvOptions.H"

Info<< "\nReading species\n" << endl;

Pmt::porousMixture composition{medium, phase, transportProperties};

Info<< "\nReading reactions\n" << endl;

Pmt::reactions reactions{composition, transportProperties};
