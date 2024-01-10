#include "BrooksAndCorey.H"
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
defineTypeNameAndDebug(BrooksAndCorey, 0);
addToRunTimeSelectionTable(RichardsModel, BrooksAndCorey, dictionary);
addToRunTimeSelectionTable(capillarityModel, BrooksAndCorey, dictionary);
addToRunTimeSelectionTable(relativePermeabilityModel, BrooksAndCorey, dictionary);
addToRunTimeSelectionTable(moistureDiffusivityModel, BrooksAndCorey, dictionary);
}
}
}

Foam::Pmt::unsaturatedFlowModels::BrooksAndCorey::BrooksAndCorey
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
    pc0_{constantFields::read("pc0", medium.mesh(), dimPressure,dictionaries::subOrNullDictRef(transportProperties, "BrooksAndCoreyCoeffs"))},
    alpha_{constantFields::read("alpha", medium.mesh(), dimless, dictionaries::subOrNullDictRef(transportProperties, "BrooksAndCoreyCoeffs"))},
    n_{constantFields::read("n", medium.mesh(), dimless, dictionaries::subOrNullDictRef(transportProperties, "BrooksAndCoreyCoeffs"))},
    l_{constantFields::readOrDefault("l", alpha_, dictionaries::subOrNullDictRef(transportProperties, "BrooksAndCoreyCoeffs"))}
{
    Info<< nl
        << typeName << " model" << nl
        << "{" << nl
        << "    "; pc0_.writeMinMax(Info);
    Info<< "    "; alpha_.writeMinMax(Info);
    Info<< "    "; n_.writeMinMax(Info);
    Info<< "    "; l_.writeMinMax(Info);
    Info<< "}" << nl
        << endl;
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::BrooksAndCorey::C()
{
    volScalarField p{-pc0_*pow(frac_.eff(), -alpha_)};

    return -neg(p + pc0_)*(frac_.max() - frac_.min())/(alpha_*p*pow(-p/pc0_, 1/alpha_));
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::BrooksAndCorey::M()
{
    volScalarField eff{frac_.eff()};

    return medium_.K()/phase_.mu()*pow(eff, n_ + l_ - 1)
         + neg0(1 - eff)*dimensionedScalar{dimArea/dimDynamicViscosity, One};
}
