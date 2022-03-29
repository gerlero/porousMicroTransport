#include "constantFields.H"

#include <messageStream.H>

#include <utility>

template<typename... Args>
Foam::Pmt::phaseFractionField::phaseFractionField
(
    const dictionary& transportProperties, 
    Args&&... args
)
:
    volScalarField{std::forward<Args>(args)...},
    min_{constantFields::readOrDefault(name() + "min", mesh(), dimless, Zero, transportProperties)},
    max_{constantFields::readOrDefault(name() + "max", mesh(), dimless, One, transportProperties)}
{
    Info<< nl
        << "Phase fraction field " << name() << nl
        << "{" << nl
        << "    "; writeMinMax(Info);
    Info<< "    "; min_.writeMinMax(Info);
    Info<< "    "; max_.writeMinMax(Info);
    Info<< "}" << nl
        << endl;
}

inline const Foam::volScalarField& Foam::Pmt::phaseFractionField::min() const noexcept
{
    return min_;
}

inline const Foam::volScalarField& Foam::Pmt::phaseFractionField::max() const noexcept
{
    return max_;
}

inline Foam::tmp<Foam::volScalarField> Foam::Pmt::phaseFractionField::eff() const
{
    return (*this - min())/(max() - min());
}
