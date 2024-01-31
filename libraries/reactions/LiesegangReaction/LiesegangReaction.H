#ifndef PMT_LiesegangReaction_H
#define PMT_LiesegangReaction_H

#include "reaction.H"

namespace Foam
{
namespace Pmt
{

class LiesegangReaction
:
    public reaction
{
public:
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winconsistent-missing-override"
    TypeName("Liesegang");
    #pragma clang diagnostic pop

    LiesegangReaction
    (
        const basicMultiComponentMixture& composition,
        const dictionary& reactionDict
    );

    tmp<volScalarField> kf() override;
    tmp<volScalarField> kr() override;

private:
    const basicMultiComponentMixture& composition_;
    const dimensionedScalar kf1_scalar;
    const dimensionedScalar kf2_scalar;
    const dimensionedScalar kr_scalar;
    const dimensionedScalar cs;
    const dimensionedScalar rho;
    const dimensionedScalar cradius;
    //label stripeSpecies;

    volScalarField cs_field;
    volScalarField heaviField;
    volScalarField cellSizes;
    volScalarField binary;
    //boolField binary = List<bool>(Foam::one{}, Zero);

    //heaviField: Field with results of applying Heaviside Step function through Foam::neg() to each cell
    //binary: Field with check results on concentration levels (below rho (1) or above or equal (0))

    List<List<label>> inRadius;

    void calcCellsInRadius();
    label find(const List<label>& list,label elem);

    void updateCSfield(dimensionedScalar cs_scalar);

    void heaviside2InternalField(
		const DimensionedField<scalar, Foam::volMesh>& cField,
		const DimensionedField<scalar, Foam::volMesh>& csField
	);
};

}
}

#endif
