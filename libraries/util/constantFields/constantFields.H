#ifndef PMT_constantFields_H
#define PMT_constantFields_H

#include <tmp.H>
#include <volFieldsFwd.H>
#include <word.H>
#include <fvMesh.H>
#include <dimensionSet.H>
#include <scalarFwd.H>
#include <dictionary.H>

namespace Foam
{
namespace Pmt
{
namespace constantFields
{

tmp<volScalarField> read
(
    const word& name,
    const fvMesh& mesh,
    const dimensionSet& dims,
    const dictionary& dict=dictionary::null
);


tmp<volScalarField> readIfPresent
(
    const word& name,
    const fvMesh& mesh,
    const dimensionSet& dims,
    const dictionary& dict=dictionary::null
);


tmp<volScalarField> readOrDefault
(
    const word& name,
    const fvMesh& mesh,
    const dimensionSet& dims,
    scalar val,
    const dictionary& dict=dictionary::null
);

tmp<volScalarField> readOrDefault
(
    const word& name,
    const volScalarField& deflt,
    const dictionary& dict=dictionary::null
);

}
}
}

#endif
