inline Foam::tmp<Foam::volTensorField> Foam::Pmt::porousMixture::Deff
(
    label speciesi,
    const volVectorField& U,
    const phaseFractionField& frac
)
{
    return dispersionModels_[speciesi].Deff(U, frac);
}
