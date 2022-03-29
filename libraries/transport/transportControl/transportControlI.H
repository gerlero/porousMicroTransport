#include <messageStream.H>
#include <fvc.H>

#include <utility>
#include <type_traits>

template<class Base>
template<typename... Args>
Foam::Pmt::transportControl<Base>::transportControl
(
    basicMultiComponentMixture& composition,
    Args&&... args
)
:
    Base{std::forward<Args>(args)...},
    composition_{composition},
    maxDeltaY_(composition.Y().size(), 0.0),
    maxDYdT_(composition.Y().size(), 0.0),
    upToDate_{false}
{
    static_assert
    (
        std::is_base_of<timeStepControl, Base>::value,
        "Base template argument must be timeStepControl or a class that derives from it"
    );
}

template<class Base>
bool Foam::Pmt::transportControl<Base>::retry()
{
    update();

    auto newDeltaT = GREAT;

    forAll(composition_.Y(), speciesi)
    {
        auto maxDeltaY = maxDeltaY_[speciesi];
        auto maxDYdT = maxDYdT_[speciesi];

        if (maxDYdT*this->deltaTValue() > 1.5*maxDeltaY)
        {
            Info<< "Excessive concentration change of species " << composition_.species()[speciesi] << endl;
            newDeltaT = min(newDeltaT, maxDeltaY/maxDYdT/2);
        }
    }

    if (newDeltaT < GREAT)
    {
        if (this->restartTimeStepIfAdjustable(newDeltaT))
        {
            return true;
        }
        else
        {
            Warning
                << "Excessive species concentration changes with adjustTimeStep off" << nl
                << endl;

            return false;
        }
    }

    return false;
}

template<class Base>
Foam::scalar Foam::Pmt::transportControl<Base>::maxDeltaTValue()
{
    auto maxDeltaT = min(Base::maxDeltaTValue(), 1.2*this->deltaTValue());

    update();

    forAll(composition_.Y(), speciesi)
    {
        if (auto maxDYdT = maxDYdT_[speciesi])
        {
            maxDeltaT = min(maxDeltaT, maxDeltaY_[speciesi]/maxDYdT);
        }
    }

    return maxDeltaT;
}

template<class Base>
void Foam::Pmt::transportControl<Base>::operator++()
{
    upToDate_ = false;

    Base::operator++();
}

template<class Base>
void Foam::Pmt::transportControl<Base>::operator--()
{
    for (auto& Y : composition_.Y())
    {
        Y = Y.oldTime();
    }

    upToDate_ = !upToDate_;

    Base::operator--();
}

template<class Base>
void Foam::Pmt::transportControl<Base>::update()
{
    if (!upToDate_)
    {
        auto maxDeltaC =
            this->controlDict().getCheckOrDefault("maxDeltaC", GREAT, [](scalar v){ return v>=0; });
        auto relMaxDeltaC =
            this->controlDict().getCheckOrDefault("relMaxDeltaC", GREAT, [](scalar v){ return v>=0; });

        forAll(composition_.Y(), speciesi)
        {
            const auto& Y = composition_.Y(speciesi);

            maxDeltaY_[speciesi] = max(relMaxDeltaC*gMax(Y), maxDeltaC);
            maxDYdT_[speciesi] = max(mag(fvc::ddt(Y))).value();
        }

        upToDate_ = true;
    }
}
