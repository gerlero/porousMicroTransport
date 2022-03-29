#include "fluidPhase.H"

#include <messageStream.H>

#include <utility>

Foam::Pmt::fluidPhase::fluidPhase(const dictionary& transportProperties, word name)
:
    name_{std::move(name)},
    rho_{"rho", dimDensity, Zero},
    rhoSet_
    {
        rho_.readIfPresent
        (
            !name_.empty()
          ? transportProperties.subOrEmptyDict("phase." + name_)
          : transportProperties
        )
    },
    mu_{"mu", dimDynamicViscosity, Zero},
    muSet_
    {
        mu_.readIfPresent
        (
            !name_.empty()
          ? transportProperties.subOrEmptyDict("phase." + name_)
          : transportProperties
        )
    }
{
    Info<< nl
        << "Phase " << name_ << nl
        << "{" << nl;
    Info<< "    rho";
    if (rhoSet_)
    {
        Info<< " = " << rho_.value() << nl;
    }
    else
    {
        Info<< ": not set" << nl;
    }
    Info<< "    mu";
    if (muSet_)
    {
        Info<< " = " << mu_.value() << nl;
    }
    else
    {
        Info<< ": not set" << nl;
    }
    Info<< "}" << nl
        << endl;
}
