#include "alphaDispersion.H"
#include "constantFields.H"
#include "porousMedium.H"

#include <className.H>
#include <addToRunTimeSelectionTable.H>
#include <messageStream.H>

#include <utility>

namespace Foam
{
namespace Pmt
{
namespace dispersionModels
{
defineTypeNameAndDebug(alphaDispersion, 0);
addToRunTimeSelectionTable(dispersionModel, alphaDispersion, dictionary);
}
}
}

Foam::Pmt::dispersionModels::alphaDispersion::alphaDispersion
(
    const porousMedium& medium,
    const fluidPhase&,
    const word&,
    dimensionedScalar Dm,
    const dictionary& coeffs
)
:
    medium_{medium},
    Dm_{std::move(Dm)},
    tau_{constantFields::read("tau", medium.mesh(), dimless, coeffs)},
    alphaT_{constantFields::read("alphaT", medium.mesh(), dimLength, coeffs)},
    alphaL_{constantFields::read("alphaL", medium.mesh(), dimLength, coeffs)}
{
    Info<< nl
        << typeName << " model" << nl
        << "{" << nl
        << "    "; tau_.writeMinMax(Info);
    Info<< "    "; alphaT_.writeMinMax(Info);
    Info<< "    "; alphaL_.writeMinMax(Info);
    Info<< "}" << nl
        << endl;
}

Foam::tmp<Foam::volTensorField> Foam::Pmt::dispersionModels::alphaDispersion::Deff
(
    const volVectorField& U,
    const phaseFractionField&
)
{
    volVectorField V{U/medium_.eps()};
    volScalarField magV{mag(V)};

    return
        (Dm_/tau_ + alphaT_*magV)*tensor::I
      + (alphaL_ - alphaT_)*(V*V)/(magV + dimensionedScalar{dimVelocity, VSMALL});
}
