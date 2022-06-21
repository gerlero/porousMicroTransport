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
    dispersionModels_(Y().size())
{
    forAll(species(), speciesi)
    {
        const auto& speciesName = species()[speciesi];

        Info<< "Species " << speciesName << endl;

        const auto& speciesTransport =
            transportProperties.optionalSubDict(speciesName);

        dimensionedScalar Dm{"Dm", dimViscosity, speciesTransport};

        Info<< "Dm = " << Dm.value() << nl
            << endl;

        dispersionModels_.set
        (
            speciesi,
            dispersionModel::New
            (
                medium,
                phase,
                speciesName,
                std::move(Dm),
                speciesTransport
            )
        );
    }
}
