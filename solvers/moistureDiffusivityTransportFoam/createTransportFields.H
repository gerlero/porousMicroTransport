Info<< "\nReading species\n" << endl;

Pmt::porousMixture composition{medium, phase, transportProperties};

Info<< "\nReading reactions\n" << endl;

Pmt::reactions reactions{composition, transportProperties};

volScalarField Rd
{
    IOobject
    {
        "Rd",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    },
    mesh,
    dimensionedScalar{dimless, One}
};
