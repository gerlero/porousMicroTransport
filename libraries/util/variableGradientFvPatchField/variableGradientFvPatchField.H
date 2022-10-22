#ifndef PMT_variableGradientFvPatchField_H
#define PMT_variableGradientFvPatchField_H

#include <fixedGradientFvPatchField.H>

namespace Foam
{
namespace Pmt
{

template<typename Type>
class variableGradientFvPatchField
:
    public fixedGradientFvPatchField<Type>
{
public:
    variableGradientFvPatchField
    (
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF,
        const dictionary&
    );

    using fixedGradientFvPatchField<Type>::fixedGradientFvPatchField;

    tmp<fvPatchField<Type>> clone() const override = 0;
    
    tmp<fvPatchField<Type>>
    clone(const DimensionedField<Type, volMesh>& iF) const override = 0;

    void updateCoeffs() final;

private:
    virtual tmp<Field<Type>> computeGradient();
};

}
}

#include "variableGradientFvPatchFieldI.H"

#endif
