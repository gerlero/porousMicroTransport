#include "porousMedium.H"
#include "phaseFractionField.H"

inline void Foam::Pmt::porousMixture::correct
(
    const volVectorField& U,
    const phaseFractionField& frac
)
{
    auto theta = medium_.eps()*frac/frac.max();

    volVectorField V{U/theta};
    volScalarField magV{mag(V)};

    Disp_ = medium_.alphaT()*magV*tensor::I 
        + (medium_.alphaL() - medium_.alphaT())*(V*V)/(magV + dimensionedScalar{dimVelocity, VSMALL});


    forAll(species(), speciesi)
    {
        if (Kd_[speciesi].value() != 0)
        {
            Rd_[speciesi] = One + medium_.rs()*(One - medium_.epsTotal())*Kd_[speciesi]/frac;
        }
    }
}

inline Foam::tmp<Foam::volTensorField> Foam::Pmt::porousMixture::Deff
(
    label speciesi
) const
{
    return Disp_ + Dm_[speciesi]/medium_.tau()*tensor::I;
}

inline const Foam::volScalarField& Foam::Pmt::porousMixture::retardation
(
    label speciesi
) const
{
    return Rd_[speciesi];
}

inline Foam::tmp<Foam::fv::ddtScheme<Foam::scalar>> Foam::Pmt::porousMixture::ddtScheme
(
    label speciesi
) const
{
    const auto& mesh = Y(speciesi).mesh();

    return fv::ddtScheme<scalar>::New(mesh, mesh.ddtScheme("ddt(C)"));
}
