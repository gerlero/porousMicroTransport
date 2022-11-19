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


template<class To, class From_, template<class> class UniquePtr_>
UniquePtr_<To> dynamicPtrCast(UniquePtr_<From_>& p);

template<class To, class From_, template<class> class UniquePtr_>
UniquePtr_<To> dynamicPtrCast(UniquePtr_<From_>&& p);

}
}

#include "runTimeSelectionHelpersI.H"

#endif
