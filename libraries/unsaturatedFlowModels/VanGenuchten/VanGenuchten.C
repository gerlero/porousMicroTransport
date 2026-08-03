#include "VanGenuchten.H"
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
    const phaseFractionField& frac,
    const dictionary& transportProperties
)
:
    medium_{medium},
    phase_{phase},
    frac_{frac},
    pc0_{constantFields::read("pc0", medium.mesh(), dimPressure, dictionaries::subOrNullDictRef(transportProperties, "VanGenuchtenCoeffs"))},
    m_{
        [&]
        {
            if (auto m = constantFields::readIfPresent("m", medium.mesh(), dimless, dictionaries::subOrNullDictRef(transportProperties, "VanGenuchtenCoeffs")))
            {
                return m;
            }

            auto n = constantFields::read("n", medium.mesh(), dimless, dictionaries::subOrNullDictRef(transportProperties, "VanGenuchtenCoeffs"));

            return volScalarField::New("m", 1 - 1/n);
        }()
    },
    l_{constantFields::readOrDefault("l", medium.mesh(), dimless, 0.5, dictionaries::subOrNullDictRef(transportProperties, "VanGenuchtenCoeffs"))}
{
    Info<< nl
        << typeName << " model" << nl
        << "{" << nl;
    Info<< "    "; pc0_.writeMinMax(Info);
    Info<< "    "; m_.writeMinMax(Info); 
    Info<< "    n = 1/(1 - m)" << nl;
    Info<< "    "; l_.writeMinMax(Info);
    Info<< "}" << nl
        << endl;
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::VanGenuchten::C()
{
    volScalarField Se{min(frac_.eff(), 1 - 1e-7)};

    return 1/pc0_*m_/(1 - m_)*(frac_.max() - frac_.min())*pow(Se, 1/m_)*pow((1 - pow(Se, 1/m_)), m_);
}

Foam::tmp<Foam::volScalarField>
Foam::Pmt::unsaturatedFlowModels::VanGenuchten::M()
{
    volScalarField Se{min(frac_.eff(), 1 - 1e-7)};

    return medium_.K()/phase_.mu()*pow(Se, l_)*pow(1 - pow(1 - pow(Se, 1/m_), m_), 2);
}
