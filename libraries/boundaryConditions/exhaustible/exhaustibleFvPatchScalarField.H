#ifndef PMT_exhaustibleFvPatchScalarField_H
#define PMT_exhaustibleFvPatchScalarField_H

#include <mixedFvPatchField.H>

namespace Foam
{
namespace Pmt
{

class exhaustibleFvPatchScalarField
:
    public mixedFvPatchField<scalar>
{
public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("exhaustible");
    #pragma clang diagnostic pop

    exhaustibleFvPatchScalarField
    (
        const fvPatch& p,
        const DimensionedField<scalar, volMesh>& iF,
        const dictionary& dict
    );

    using mixedFvPatchField<scalar>::mixedFvPatchField;

    tmp<fvPatchScalarField> clone() const override;

    tmp<fvPatchScalarField>
    clone(const DimensionedField<scalar, volMesh>& iF) const override;

    void updateCoeffs() override;

    void write(Ostream& os) const override;

private:
    const scalar capacity_{Zero};
    scalar used_{Zero};
    label curTimeIndex_{-1};
    scalar inflow_{Zero};
};

}
}

#endif
