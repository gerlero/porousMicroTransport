#include "LETxs.H"
#include "constantFields.H"
#include "porousMedium.H"
#include "fluidPhase.H"
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
defineTypeNameAndDebug(LETxs, 0);
addToRunTimeSelectionTable(moistureDiffusivityModel, LETxs, dictionary);
}
}
}

Foam::Pmt::unsaturatedFlowModels::LETxs::LETxs
(
    const porousMedium& medium,
    const fluidPhase& phase,
    const dictionary& coeffs
)
:
    medium_{medium},
    phase_{phase},
    pc0_{constantFields::read("pc0", medium.mesh(), dimPressure, coeffs)},
    Lw_{"Lw", dimless, coeffs},
    Ew_{"Ew", dimless, coeffs},
    Tw_{"Tw", dimless, coeffs},
    Ls_{"Ls", dimless, coeffs},
    Es_{"Es", dimless, coeffs},
    Ts_{"Ts", dimless, coeffs}
{
    Info<< nl
        << typeName << " model" << nl
        << "{" << nl
        << "    "; pc0_.writeMinMax(Info);
    Info<< "    Lw = " << Lw_.value() << nl
        << "    Ew = " << Ew_.value() << nl
        << "    Tw = " << Tw_.value() << nl
        << "    Ls = " << Ls_.value() << nl
        << "    Es = " << Es_.value() << nl
        << "    Ts = " << Ts_.value() << nl
        << "}" << nl
        << endl;
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::LETxs::D(const phaseFractionField& frac)
{
    volScalarField Swp{frac.eff()};
    auto Swir = frac.min()/frac.max();

    return
        medium_.K()/phase_.mu()*pc0_*Es_*pow(Swp,Lw_)*pow(Swp,Ts_)*pow(1 - Swp,Ls_)*(Ls_*Swp - Swp*Ts_ + Ts_)
       /(frac.max()*Swp*(Swir - 1)*(Swp - 1)*pow(Es_*pow(Swp,Ts_) + pow(1 - Swp,Ls_),2)*(Ew_*pow(1 - Swp,Tw_) + pow(Swp,Lw_)));

}
