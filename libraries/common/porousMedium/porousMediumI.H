#include <error.H>

inline const Foam::fvMesh& Foam::Pmt::porousMedium::mesh() const noexcept
{
    return mesh_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::eps() const noexcept
{
    return eps_;
}

inline const Foam::volScalarField& Foam::Pmt::porousMedium::K() const
{
    if (!K_)
    {
        FatalErrorInFunction
            << "Intrinsic permeability K was required but never set" << endl
            << exit(FatalError);
    }

    return *K_;
}
