#include "BrooksAndCorey.H"
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
    const dictionary& coeffs
)
:
    medium_{medium},
    phase_{phase},
    pc0_{constantFields::read("pc0", medium.mesh(), dimPressure, coeffs)},
    alpha_{constantFields::read("alpha", medium.mesh(), dimless, coeffs)},
    n_{constantFields::read("n", medium.mesh(), dimless, coeffs)},
    l_{constantFields::readOrDefault("l", alpha_, coeffs)}
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
Foam::Pmt::unsaturatedFlowModels::BrooksAndCorey::C(const phaseFractionField& frac)
{
    volScalarField p{pc0_*pow(frac.eff(), -alpha_)};

    return -neg(p + pc0_)*(frac.max() - frac.min())/(alpha_*p*pow(-p/pc0_, 1/alpha_));
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::BrooksAndCorey::M(const phaseFractionField& frac)
{
    volScalarField eff{frac.eff()};

    return medium_.K()/phase_.mu()*pos(1 - eff)*pow(eff, (n_ + l_ - 1))
         + neg0(1 - eff);
}
