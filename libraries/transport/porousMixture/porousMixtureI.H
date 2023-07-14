#include "porousMedium.H"
#include "phaseFractionField.H"

inline Foam::tmp<Foam::volTensorField> Foam::Pmt::porousMixture::Deff
(
    label speciesi,
    const volVectorField& U,
    const phaseFractionField& frac
)
{
    auto theta = medium_.eps()*frac/frac.max();

    volVectorField V{U/theta};
    volScalarField magV{mag(V)};

    return
        (Dm_[speciesi]/medium_.tau() + medium_.alphaT()*magV)*tensor::I
      + (medium_.alphaL() - medium_.alphaT())*(V*V)/(magV + dimensionedScalar{dimVelocity, VSMALL});
}
