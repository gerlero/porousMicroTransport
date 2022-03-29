#ifndef PMT_darcyGradPressureFvPatchScalarField_H
#define PMT_darcyGradPressureFvPatchScalarField_H

#include "variableGradientFvPatchField.H"

namespace Foam
{
namespace Pmt
{

class darcyGradPressureFvPatchScalarField
:
    public variableGradientFvPatchField<scalar>
{
public:
    TypeName("darcyGradPressure");

    using variableGradientFvPatchField<scalar>::variableGradientFvPatchField;

    tmp<fvPatchScalarField> clone() const override;

    tmp<fvPatchScalarField>
    clone(const DimensionedField<scalar, volMesh>& iF) const override;

private:
    tmp<scalarField> computeGradient() override;
};

}
}

#endif
