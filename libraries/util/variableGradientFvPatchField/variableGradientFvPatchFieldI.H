#include <type_traits>

template<typename Type>
Foam::Pmt::variableGradientFvPatchField<Type>::variableGradientFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary&
)
:
    fixedGradientFvPatchField<Type>{p, iF}
{
   this->evaluate();
}

template<typename Type>
void Foam::Pmt::variableGradientFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    this->gradient() = computeGradient();

    fvPatchField<Type>::updateCoeffs();
}

template<typename Type>
Foam::tmp<Foam::Field<Type>>
Foam::Pmt::variableGradientFvPatchField<Type>::computeGradient()
{
    return this->gradient();
};
