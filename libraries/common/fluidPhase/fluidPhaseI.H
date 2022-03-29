#include <error.H>

inline const Foam::dimensionedScalar& Foam::Pmt::fluidPhase::rho() const
{
    if (!rhoSet_)
    {
        FatalErrorInFunction
            << "Density rho of this phase " << name() << " was required but never set" << endl
            << exit(FatalError);
    }

    return rho_;
}

inline const Foam::dimensionedScalar& Foam::Pmt::fluidPhase::mu() const
{
    if (!muSet_)
    {
        FatalErrorInFunction
            << "Viscosity mu of phase " << name() << " was required but never set" << endl
            << exit(FatalError);
    }

    return mu_;
}

inline const Foam::word& Foam::Pmt::fluidPhase::name() const noexcept
{
    return name_;
}
