#include "exhaustibleFvPatchScalarField.H"

#include <addToRunTimeSelectionTable.H>
#include <surfaceFields.H>
#include <volFields.H>

namespace Foam
{
namespace Pmt
{
makePatchTypeField(fvPatchScalarField, exhaustibleFvPatchScalarField);
}
}

Foam::Pmt::exhaustibleFvPatchScalarField::exhaustibleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<scalar>{p, iF},
    capacity_{dict.get<scalar>("remaining")},
    used_{Zero},
    curTimeIndex_{this->db().time().timeIndex() + 1},
    inflow_{Zero}
{
    this->refValue() = scalarField{"value", dict, p.size()};
    this->refGrad() = Zero;
    this->valueFraction() = One;

    fvPatchScalarField::operator=(this->refValue());
}

Foam::tmp<Foam::fvPatchScalarField>
Foam::Pmt::exhaustibleFvPatchScalarField::clone() const
{
    return tmp<fvPatchScalarField>::NewFrom<exhaustibleFvPatchScalarField>(*this);
}

Foam::tmp<Foam::fvPatchScalarField>
Foam::Pmt::exhaustibleFvPatchScalarField::clone
(
    const DimensionedField<scalar, volMesh>& iF
) const
{
    return tmp<fvPatchScalarField>::NewFrom<exhaustibleFvPatchScalarField>(*this, iF);
}

void Foam::Pmt::exhaustibleFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    auto timeIndex = this->db().time().timeIndex();

    if (timeIndex != curTimeIndex_ && timeIndex != curTimeIndex_ + 1)
    {
        FatalErrorInFunction
            << "Unexpected timeIndex jump: from " << curTimeIndex_ << " to " << timeIndex << endl
            << abort(FatalError);
    }

    const auto& phip = patch().lookupPatchField<surfaceScalarField, scalar>("phi");

    auto deltaT = timeIndex == curTimeIndex_ ? this->db().time().deltaTValue() : this->db().time().deltaT0Value();

    inflow_ = gSum(-phip)*deltaT;

    if (timeIndex == curTimeIndex_ + 1)
    {
        used_ += inflow_;
        inflow_ = Zero;
        curTimeIndex_ = timeIndex;

        Info << "Exhaustible patch " << patch().name() << " used " << used_ << " of " << capacity_ << " (remaining  " << capacity_ - used_ << ")" << endl;
    }

    this->valueFraction() = (capacity_ > used_);

    mixedFvPatchField<scalar>::updateCoeffs();
}

void Foam::Pmt::exhaustibleFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    os.writeEntry("remaining", capacity_ - (used_ + inflow_));
    this->writeEntry("value", os);
}
