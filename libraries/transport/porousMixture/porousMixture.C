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
    medium_(medium),
    Dm_(Y().size())
{
    forAll(species(), speciesi)
    {
        const auto& speciesName = species()[speciesi];

        Info<< "Species " << speciesName << endl;

        dimensionedScalar Dm{"Dm", dimViscosity, transportProperties.optionalSubDict(speciesName)};

        Info<< "Dm = " << Dm.value() << nl
            << endl;

        Dm_[speciesi].dimensions().reset(dimViscosity);
        Dm_[speciesi] = Dm;

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
