#include <error.H>

#include <type_traits>

template<class T, typename... Args>
auto Foam::Pmt::NewOfSelectedType(const word& modelType, Args&&... args)
-> decltype(T::dictionaryConstructorTablePtr_->find(modelType)()(std::forward<Args>(args)...))
{
    static_assert
    (
        std::has_virtual_destructor<T>::value,
        "Target type must have a virtual destructor"
    );

    auto cstrIter = T::dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == T::dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown " << T::typeName << " type " << modelType << nl
            << nl
            << "Valid types are: " << nl
            << T::dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(std::forward<Args>(args)...);
}


template<class T, class U>
T Foam::Pmt::dynamicPtrCast(U&& uniquePtr)
{
    static_assert
    (
        std::has_virtual_destructor<typename T::element_type>::value,
        "Cast target type must have a virtual destructor"
    );

    if (auto* ptr = dynamic_cast<typename T::pointer>(uniquePtr.get()))
    {
        std::forward<U>(uniquePtr).release();
        return T{ptr};
    }

    return T{nullptr};
}
