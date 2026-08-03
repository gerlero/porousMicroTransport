#include "LETd.H"
#include "constantFields.H"
#include "dictionaries.H"
#include "porousMedium.H"
#include "phaseFractionField.H"

#include <className.H>
#include <addToRunTimeSelectionTable.H>
#include <messageStream.H>

namespace Foam
{
namespace Pmt
{
namespace unsaturatedFlowModels
{
defineTypeNameAndDebug(LETd, 0);
addToRunTimeSelectionTable(moistureDiffusivityModel, LETd, dictionary);
}
}
}

Foam::Pmt::unsaturatedFlowModels::LETd::LETd
(
    const porousMedium& medium,
    const fluidPhase&,
    const phaseFractionField& frac,
    const dictionary& transportProperties
)
:
    frac_{frac},
    Dwt_{constantFields::read("Dwt", medium.mesh(), dimViscosity, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs"))},
    L_{"L", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    E_{"E", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    T_{"T", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")}
{
    Info<< nl
        << typeName << " model" << nl
        << "{" << nl
        << "    "; Dwt_.writeMinMax(Info);
    Info<< "    L = " << L_.value() << nl
        << "    E = " << E_.value() << nl
        << "    T = " << T_.value() << nl
        << "}" << nl
        << endl;
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::LETd::D()
{
    volScalarField Swp{min(frac_.eff(), 1 - 1e-7)};

    return Dwt_*pow(Swp, L_)/(pow(Swp, L_) + E_*pow(1 - Swp, T_));
}
