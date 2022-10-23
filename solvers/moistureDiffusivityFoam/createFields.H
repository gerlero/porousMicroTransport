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

Info<< "Reading velocity field (U | U" << theta.name() << ")\n" << endl;

volVectorField U
{
    [&]
    {
        IOobject IOU
        {
            "U",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        };

        IOobject IOUtheta
        {
            "U" + theta.name(),
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        };

        if
        (
            IOU.typeHeaderOk<volVectorField>() 
         && IOUtheta.typeHeaderOk<volVectorField>()
        )
        {
            FatalErrorInFunction
                << "Found both " << IOU.name() << " and " << IOUtheta.name() << " fields" << nl
                << "Please remove one of them" << endl
                << exit(FatalError);
        }

        if (IOUtheta.typeHeaderOk<volVectorField>())
        {
            return IOUtheta;
        }

        return IOU;
    }(),
    mesh,
    dimensionedVector{dimVelocity, Zero}
};

#include <createPhi.H>

mesh.setFluxRequired(theta.name());

auto flowModel = Pmt::moistureDiffusivityModel::New(medium, phase, transportProperties);

#include <createFvOptions.H>

volScalarField D
{
    {
        "D_",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    },
    mesh,
    dimViscosity
};
