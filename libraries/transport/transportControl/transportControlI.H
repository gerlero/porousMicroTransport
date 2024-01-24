#include <messageStream.H>
#include <fvc.H>

#include <utility>
#include <type_traits>
#include <limits>


template<class Base>
template<typename... Args_>
Foam::Pmt::transportControl<Base>::transportControl
(
    porousMixture& composition,
    Args_&&... args
)
:
    Base{std::forward<Args_>(args)...},
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

    auto newDeltaT = std::numeric_limits<scalar>::infinity();

    forAll(composition_.Y(), speciesi)
    {
        auto maxDeltaY = maxDeltaY_[speciesi];
        auto maxDYdT = maxDYdT_[speciesi];

        if (maxDYdT*this->deltaTValue() > 1.05*maxDeltaY)
        {
            Info<< "Excessive concentration change of species " << composition_.species()[speciesi] << endl;
            newDeltaT = min(newDeltaT, maxDeltaY/maxDYdT/2);
        }
    }

    if (newDeltaT < std::numeric_limits<scalar>::infinity())
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

        bool maxDeltaCSet = false;
        List<scalar> maxDeltaC(composition_.species().size(), std::numeric_limits<scalar>::infinity());

        const dictionary* maxDeltaCDict = this->controlDict().findDict("maxDeltaC");

        if (maxDeltaCDict){
            maxDeltaCSet = true;
            for (const auto& maxDCSpecies : *maxDeltaCDict)
            {
                const auto speciesName = maxDCSpecies.keyword();
                const label speciesIndex = composition_.species().find(speciesName);

                if (speciesIndex<0)
                {
                    FatalErrorInFunction
                        << "Sub-dict maxDeltaC in controlDict has an unknown species." << nl
                        << abort(FatalError);
                }
                
                maxDeltaC[speciesIndex] = maxDCSpecies.getCheck<scalar>([](scalar v){ return v>=0; });
                
            }
        } else {
            scalar globalMaxDeltaC;
            maxDeltaCSet = this->controlDict().readCheckIfPresent
            (
                "maxDeltaC", 
                globalMaxDeltaC,
                [](scalar v){ return v>=0; }
            );

            if (maxDeltaCSet) {
                forAll (maxDeltaC, idx)
                {
                    maxDeltaC[idx] = globalMaxDeltaC;
                }
            }
        }

        scalar relMaxDeltaC;
        bool relMaxDeltaCSet = this->controlDict().readCheckIfPresent
            (
                "relMaxDeltaC", 
                relMaxDeltaC,
                [](scalar v){ return v>=0; }
            );


        forAll(composition_.Y(), speciesi)
        {
            const auto& Y = composition_.Y(speciesi);

            if (maxDeltaCSet && relMaxDeltaCSet)
            {
                maxDeltaY_[speciesi] = max(relMaxDeltaC*gMax(Y), maxDeltaC[speciesi]);
            }
            else if (maxDeltaCSet)
            {
                maxDeltaY_[speciesi] = maxDeltaC[speciesi];
            }
            else if (relMaxDeltaCSet)
            {
                maxDeltaY_[speciesi] = max(relMaxDeltaC*gMax(Y), SMALL);
            }
            else
            {
                maxDeltaY_[speciesi] = std::numeric_limits<scalar>::infinity();
            }

            maxDYdT_[speciesi] = gMax(mag(composition_.ddtScheme(speciesi)->fvcDdt(Y))());
        }

        upToDate_ = true;
    }
}
