#ifndef PMT_phaseFractionField_H
#define PMT_phaseFractionField_H

#include "phaseFractionFieldFwd.H"

#include <dictionary.H>
#include <tmp.H>
#include <volFields.H>

namespace Foam
{
namespace Pmt
{

class phaseFractionField
:
    public volScalarField
{
public:
    template<typename... Args_>
    phaseFractionField(const dictionary& transportProperties, Args_&&... args);

    const volScalarField& min() const noexcept;

    const volScalarField& max() const noexcept;

    tmp<volScalarField> eff() const;

    using volScalarField::operator=;

    void clone() = delete;

    template<typename... Args_>
    static void New(Args_&&... args) = delete;

private:
    volScalarField min_;
    volScalarField max_;
};

}
}

#include "phaseFractionFieldI.H"

#endif
