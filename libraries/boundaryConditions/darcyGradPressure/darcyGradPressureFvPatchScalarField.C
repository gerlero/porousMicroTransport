#include "darcyGradPressureFvPatchScalarField.H"

#include <addToRunTimeSelectionTable.H>
#include <surfaceFields.H>
#include <volFields.H>

namespace Foam
{
namespace Pmt
{
makePatchTypeField(fvPatchScalarField, darcyGradPressureFvPatchScalarField);
}
}

Foam::tmp<Foam::fvPatchScalarField>
Foam::Pmt::darcyGradPressureFvPatchScalarField::clone() const
{
    return tmp<fvPatchScalarField>::NewFrom<darcyGradPressureFvPatchScalarField>(*this);
}

Foam::tmp<Foam::fvPatchScalarField>
Foam::Pmt::darcyGradPressureFvPatchScalarField::clone
(
    const DimensionedField<scalar, volMesh>& iF
) const
{
    return tmp<fvPatchScalarField>::NewFrom<darcyGradPressureFvPatchScalarField>(*this, iF);
}

Foam::tmp<Foam::scalarField>
Foam::Pmt::darcyGradPressureFvPatchScalarField::computeGradient()
{
    const auto& phi = patch().lookupPatchField<surfaceScalarField, scalar>("phi");
    const auto& Dtheta = patch().lookupPatchField<volScalarField, scalar>("Dtheta");

    return -phi/patch().magSf()/Dtheta;
}
