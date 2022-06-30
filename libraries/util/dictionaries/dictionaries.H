#ifndef PMT_dictionaries_H
#define PMT_dictionaries_H

#include <dictionary.H>
#include <word.H>

namespace Foam
{
namespace Pmt
{
namespace dictionaries
{

const dictionary& subOrNullDictRef(const dictionary& dict, const word& keyword);

}
}
}

#endif
