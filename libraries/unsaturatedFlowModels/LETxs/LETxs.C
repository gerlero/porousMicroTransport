#include "LETxs.H"
#include "constantFields.H"
#include "dictionaries.H"
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
    const phaseFractionField& frac,
    const dictionary& transportProperties
)
:
    medium_{medium},
    phase_{phase},
    frac_{frac},
    pc0_{constantFields::read("pc0", medium.mesh(), dimPressure, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs"))},
    Lw_{"Lw", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    Ew_{"Ew", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    Tw_{"Tw", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    Ls_{"Ls", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    Es_{"Es", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")},
    Ts_{"Ts", dimless, dictionaries::subOrNullDictRef(transportProperties, "LETCoeffs")}
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
Foam::Pmt::unsaturatedFlowModels::LETxs::D()
{
    volScalarField Swp{frac_.eff()};
    auto Swir = frac_.min()/frac_.max();

    return
        medium_.K()/phase_.mu()*pc0_*Es_*pow(Swp,Lw_)*pow(Swp,Ts_)*pow(1 - Swp,Ls_)*(Ls_*Swp - Swp*Ts_ + Ts_)
       /(frac_.max()*Swp*(Swir - 1)*(Swp - 1)*pow(Es_*pow(Swp,Ts_) + pow(1 - Swp,Ls_),2)*(Ew_*pow(1 - Swp,Tw_) + pow(Swp,Lw_)));

}
