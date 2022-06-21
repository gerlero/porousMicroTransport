#include <error.H>

#include <type_traits>

template<class Model, typename... Args_>
auto Foam::Pmt::NewOfSelectedType(const word& modelType, Args_&&... args)
-> decltype(Model::dictionaryConstructorTablePtr_->find(modelType)()(std::forward<Args_>(args)...))
{
    static_assert
    (
        std::has_virtual_destructor<Model>::value,
        "Target model type must have a virtual destructor"
    );

    auto cstrIter = Model::dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == Model::dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown " << Model::typeName << " type " << modelType << nl
            << nl
            << "Valid types are: " << nl
            << Model::dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(std::forward<Args_>(args)...);
}


template<class TargetPtr, class SourcePtr_>
TargetPtr Foam::Pmt::dynamicPtrCast(SourcePtr_&& uniquePtr)
{
    static_assert
    (
        std::has_virtual_destructor<typename TargetPtr::element_type>::value,
        "Cast target type must have a virtual destructor"
    );

    if (auto* ptr = dynamic_cast<typename TargetPtr::pointer>(uniquePtr.get()))
    {
        std::forward<SourcePtr_>(uniquePtr).release();
        return TargetPtr{ptr};
    }

    return TargetPtr{nullptr};
}
