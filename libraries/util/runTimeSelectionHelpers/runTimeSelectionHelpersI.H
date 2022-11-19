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


template<class To, class From_, template<class> class UniquePtr_>
UniquePtr_<To> Foam::Pmt::dynamicPtrCast(UniquePtr_<From_>& p)
{
    static_assert
    (
        std::has_virtual_destructor<To>::value,
        "Cast target type must have a virtual destructor"
    );
    static_assert
    (
        noexcept(p.release()),
        "p.release() must be noexcept"
    );

    if (auto* target = dynamic_cast<To*>(p.get()))
    {
        p.release();
        return UniquePtr_<To>{target};
    }

    return UniquePtr_<To>{nullptr};
}

template<class To, class From_, template<class> class UniquePtr_>
UniquePtr_<To> Foam::Pmt::dynamicPtrCast(UniquePtr_<From_>&& p)
{
    return dynamicPtrCast<To>(p);
}
