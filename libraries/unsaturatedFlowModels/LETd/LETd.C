#include "LETd.H"
#include "constantFields.H"
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
    const dictionary& coeffs
)
:
    Dwt_{constantFields::read("Dwt", medium.mesh(), dimViscosity, coeffs)},
    L_{"L", dimless, coeffs},
    E_{"E", dimless, coeffs},
    T_{"T", dimless, coeffs}
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
Foam::Pmt::unsaturatedFlowModels::LETd::D(const phaseFractionField& frac)
{
    volScalarField Swp{frac.eff()};

    return Dwt_*pow(Swp, L_)/(pow(Swp, L_) + E_*pow(1 - Swp, T_));
}
