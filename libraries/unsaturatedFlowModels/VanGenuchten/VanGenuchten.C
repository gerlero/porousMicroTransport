#include "VanGenuchten.H"
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
defineTypeNameAndDebug(VanGenuchten, 0);
addToRunTimeSelectionTable(RichardsModel, VanGenuchten, dictionary);
addToRunTimeSelectionTable(capillarityModel, VanGenuchten, dictionary);
addToRunTimeSelectionTable(relativePermeabilityModel, VanGenuchten, dictionary);
addToRunTimeSelectionTable(moistureDiffusivityModel, VanGenuchten, dictionary);
}
}
}

Foam::Pmt::unsaturatedFlowModels::VanGenuchten::VanGenuchten
(
    const porousMedium& medium,
    const fluidPhase& phase,
    const dictionary& coeffs
)
:
    medium_{medium},
    phase_{phase},
    pc0_{constantFields::read("pc0", medium.mesh(), dimPressure, coeffs)},
    m_{
        [&]
        {
            if (auto m = constantFields::readIfPresent("m", medium.mesh(), dimless, coeffs))
            {
                return m;
            }

            auto n = constantFields::read("n", medium.mesh(), dimless, coeffs);

            return volScalarField::New("m", 1 - 1/n);
        }()
    },
    l_{constantFields::readOrDefault("l", medium.mesh(), dimless, 0.5, coeffs)}
{
    Info<< nl
        << typeName << " model" << nl
        << "{" << nl;
    Info<< "    "; pc0_.writeMinMax(Info);
    Info<< "    "; m_.writeMinMax(Info);
    Info<< "     n = 1/(1 - m)";
    Info<< "    "; l_.writeMinMax(Info);
    Info<< "}" << nl
        << endl;
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::VanGenuchten::C(const phaseFractionField& frac)
{
    volScalarField Se{frac.eff()};

    return 1/pc0_*m_/(1 - m_)*(frac.max() - frac.min())*pow(Se, (1/m_))*pow((1 - pow(Se, (1/m_))), m_);
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::VanGenuchten::M(const phaseFractionField& frac)
{
    volScalarField Se{frac.eff()};

    return 
        medium_.K()/phase_.mu()*pos(1 - Se)*pow(Se, l_)*pow(1 - pow(1 - pow(Se, 1/m_), m_), 2)
      + neg0(1 - Se);
}
