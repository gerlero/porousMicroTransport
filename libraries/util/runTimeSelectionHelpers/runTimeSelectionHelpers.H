#ifndef PMT_runTimeSelectionHelpers_H
#define PMT_runTimeSelectionHelpers_H

#include <utility>

namespace Foam
{
namespace Pmt
{

template<class Model, typename... Args_>
auto NewOfSelectedType(const word& modelType, Args_&&... args)
-> decltype(Model::dictionaryConstructorTablePtr_->find(modelType)()(std::forward<Args_>(args)...));


template<class TargetPtr, class SourcePtr_>
TargetPtr dynamicPtrCast(SourcePtr_&& uniquePtr);

}
}

#include "runTimeSelectionHelpersI.H"

#endif
