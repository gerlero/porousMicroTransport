#include "RichardsModel.H"
#include "constantFields.H"

#include <className.H>
#include <addToRunTimeSelectionTable.H>
#include <volFields.H>

namespace Foam
{
namespace Pmt
{
defineTypeNameAndDebug(RichardsModel, 0);
defineRunTimeSelectionTable(RichardsModel, dictionary);
}
}

Foam::tmp<Foam::volScalarField> Foam::Pmt::RichardsModel::D()
{
    return M()/C();
}
