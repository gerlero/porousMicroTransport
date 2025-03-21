#include "porousMixture.H"
#include "porousMedium.H"

#include <className.H>
#include <messageStream.H>
#include <dimensionedScalar.H>
#include <label.H>

namespace Foam
{
namespace Pmt
{
defineTypeNameAndDebug(porousMixture, 0);
}
}

Foam::Pmt::porousMixture::porousMixture
(
    const porousMedium& medium,
    const fluidPhase& phase,
    const dictionary& transportProperties
)
:
    basicMultiComponentMixture
    {
        dictionary::null,
        transportProperties.getOrDefault("species", wordList(1, "C")),
        medium.mesh(),
        word::null
    },
    medium_{medium},
    Disp_
    {
        volTensorField::New
        (
            "Disp_",
            medium.mesh(),
            dimensionedTensor{dimViscosity, Zero}
        )
    },
    Dm_(Y().size()),
    Kd_(Y().size()),
    Rd_(Y().size())
{
    forAll(species(), speciesi)
    {
        const auto& speciesName = species()[speciesi];

        Info<< "Species " << speciesName << endl;

        dimensionedScalar Dm{"Dm", dimViscosity, transportProperties.optionalSubDict(speciesName)};

        Info<< "Dm = " << Dm.value() << endl;

        Dm_[speciesi].dimensions().reset(Dm.dimensions());
        Dm_[speciesi] = Dm;


        auto Kd = dimensionedScalar::getOrDefault
            (
                "Kd", 
                transportProperties.optionalSubDict(speciesName).optionalSubDict("porousTransport"),
                dimless/dimDensity
            );

        Info<< "Kd = " << Kd.value() << nl
            << endl;

        Kd_[speciesi].dimensions().reset(Kd.dimensions());
        Kd_[speciesi] = Kd;

        Rd_.set
            (
                speciesi,
                volScalarField::New
                (
                    "Rd_",
                    medium.mesh(),
                    dimensionedScalar{dimless, One}
                )
            );


        if (transportProperties.subOrEmptyDict(speciesName).optionalSubDict("porousTransport").found("rs"))
        {
            FatalErrorInFunction
                << "Per-species density (rs) is not supported by porousMicroTransport" << nl
                << "Define rs only in the top-level transportProperties dictionary" << nl
                << exit(FatalError);
        }

        if (transportProperties.optionalSubDict(speciesName).optionalSubDict("porousTransport").found("epsTotal"))
        {
            FatalErrorInFunction
                << "Per-species total porosity (epsTotal) is not supported by porousMicroTransport" << nl
                << "Define epsTotal only in the top-level transportProperties dictionary" << nl
                << exit(FatalError);
        }

        auto lambda = dimensionedScalar::getOrDefault
            (
                "lambda",
                transportProperties.optionalSubDict(speciesName).optionalSubDict("porousTransport"),
                dimless/dimTime
            );
        if (lambda.value() != 0)
        {
            FatalErrorInFunction
                << "decay rate lambda is not supported by porousMicroTransport" << nl
                << "Define a reaction instead" << nl
                << exit(FatalError);
        }

        if (auto* speciesDict = transportProperties.findDict(speciesName))
        {
            if (speciesDict->found("dispersionModel") || speciesDict->found("alphaDispersionCoeffs"))
            {
                FatalErrorInFunction
                    << "Per-species dispersion parameters are no longer supported by porousMicroTransport" << nl
                    << "Define these only in the top-level transportProperties dictionary" << nl
                    << exit(FatalError);
            }
        }
    }


}
