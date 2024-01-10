#include <error.H>

inline const Foam::fvMesh& Foam::Pmt::porousMedium::mesh() const noexcept
{
    return mesh_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::eps() const
{
    if (!eps_)
    {
        FatalErrorInFunction
            << "Effective porosity (eps | thetamax) required but not set" << endl
            << exit(FatalError);
    }
    return *eps_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::K() const
{
    if (!K_)
    {
        FatalErrorInFunction
            << "Intrinsic permeability K required but not set" << endl
            << exit(FatalError);
    }

    return *K_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::rs() const
{
    if (!rs_)
    {
        FatalErrorInFunction
            << "Particle density rs required but not set" << endl
            << exit(FatalError);
    }

    return *rs_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::epsTotal() const
{
    if (!epsTotal_)
    {
        return eps();
    }

    return *epsTotal_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::tau() const
{
    if (!tau_)
    {
        FatalErrorInFunction
            << "Tortuosity tau required but not set" << endl
            << exit(FatalError);
    }

    return *tau_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::alphaT() const
{
    if (!alphaT_)
    {
        FatalErrorInFunction
            << "Transverse dispersion coefficient alphaT required but not set" << endl
            << exit(FatalError);
    }

    return *alphaT_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::alphaL() const
{
    if (!alphaL_)
    {
        FatalErrorInFunction
            << "Longitudinal dispersion coefficient alphaL required but not set" << endl
            << exit(FatalError);
    }

    return *alphaL_;
}

