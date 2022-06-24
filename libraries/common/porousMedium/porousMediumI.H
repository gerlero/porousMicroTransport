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
            << "Porosity (eps | thetamax) required but not set" << endl
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
