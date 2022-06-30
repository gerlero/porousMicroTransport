#include "dispersionModel.H"
#include "runTimeSelectionHelpers.H"
#include "dictionaries.H"

#include <word.H>

#include <utility>

Foam::autoPtr<Foam::Pmt::dispersionModel> Foam::Pmt::dispersionModel::New
(
    const porousMedium& medium,
    const fluidPhase& phase,
    const word& speciesName,
    dimensionedScalar Dm,
    const dictionary& speciesTransport
)
{
    auto modelType = speciesTransport.get<word>("dispersionModel");

    Info<< "Selecting dispersion model => " << modelType << endl;

    return
        NewOfSelectedType<dispersionModel>
        (
            modelType,
            medium,
            phase,
            speciesName,
            std::move(Dm),
            dictionaries::subOrNullDictRef(speciesTransport, modelType + "Coeffs")
        );
}
