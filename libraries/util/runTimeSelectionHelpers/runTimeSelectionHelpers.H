#ifndef PMT_runTimeSelectionHelpers_H
#define PMT_runTimeSelectionHelpers_H

#include <utility>

namespace Foam
{
namespace Pmt
{

template<class T, typename... Args>
auto NewOfSelectedType(const word& modelType, Args&&... args)
-> decltype(T::dictionaryConstructorTablePtr_->find(modelType)()(std::forward<Args>(args)...));


template<class T, class U>
T dynamicPtrCast(U&& uniquePtr);

}
}

#include "runTimeSelectionHelpersI.H"

#endif
