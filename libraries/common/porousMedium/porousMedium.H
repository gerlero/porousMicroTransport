#ifndef PMT_porousMedium_H
#define PMT_porousMedium_H

#include "porousMediumFwd.H"

#include <fvMesh.H>
#include <dictionary.H>
#include <volFields.H>
#include <autoPtr.H>

namespace Foam
{
namespace Pmt
{

class porousMedium
{
public:
    porousMedium(const fvMesh& mesh, const dictionary& transportProperties);

    const fvMesh& mesh() const noexcept;

    const volScalarField& eps() const;

    const volScalarField& K() const;

    porousMedium(const porousMedium&) = delete;
    void operator=(const porousMedium&) = delete;

private:
    const fvMesh& mesh_;
    autoPtr<volScalarField> eps_;
    autoPtr<volScalarField> K_;
};

}
}

#include "porousMediumI.H"

#endif
