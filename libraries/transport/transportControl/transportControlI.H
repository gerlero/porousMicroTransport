#include <messageStream.H>
#include <fvc.H>

#include <utility>
#include <type_traits>
#include <limits>


#include <cassert>


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
    dtState_{dtState::NOT_SPECIFIED}
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
    assert(dtState_ != dtState::NOT_SPECIFIED);

    if (dtState_ == dtState::SHOULD_DECREASE)
    {
        if (this->restartTimeStepIfAdjustable(this->deltaTValue()/2))
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
    update();
    assert(dtState_ != dtState::NOT_SPECIFIED);

    switch (dtState_)
    {
        case dtState::SHOULD_INCREASE:
            return min(Base::maxDeltaTValue(), 1.2*this->deltaTValue());

        case dtState::SHOULD_MAINTAIN:
            return min(Base::maxDeltaTValue(), this->deltaTValue());
            
        default:  //  DECREASE not possible, retry should've fixed deltaT in previous iteration.
            assert(false);
    }
    
}

template<class Base>
void Foam::Pmt::transportControl<Base>::operator++()
{
    dtState_ = dtState::NOT_SPECIFIED;

    Base::operator++();
}

template<class Base>
void Foam::Pmt::transportControl<Base>::operator--()
{
    for (auto& Y : composition_.Y())
    {
        Y = Y.oldTime();
    }

    dtState_ = dtState::NOT_SPECIFIED;

    Base::operator--();
}

template<class Base>
void Foam::Pmt::transportControl<Base>::update()
{
    if (dtState_ == dtState::NOT_SPECIFIED)
    {
        bool maxDeltaCSet = false;
        scalarList maxDeltaC(composition_.species().size(), std::numeric_limits<scalar>::infinity());
        
        scalarList maxDeltaY(composition_.Y().size(), 0.0);

        if (const dictionary* maxDeltaCDict = this->controlDict().findDict("maxDeltaC"))
        {
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
        }
        else
        {
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

        dtState_ = dtState::SHOULD_INCREASE;

        forAll(composition_.Y(), speciesi)
        {
            const auto& Y = composition_.Y(speciesi);
            const auto& Y_old = composition_.Y(speciesi).oldTime();

            if (maxDeltaCSet && relMaxDeltaCSet)
            {
                maxDeltaY[speciesi] = max(relMaxDeltaC*gMax(Y), maxDeltaC[speciesi]);
            }
            else if (maxDeltaCSet)
            {
                maxDeltaY[speciesi] = maxDeltaC[speciesi];
            }
            else if (relMaxDeltaCSet)
            {
                maxDeltaY[speciesi] = max(relMaxDeltaC*gMax(Y), SMALL);
            }
            else
            {
                maxDeltaY[speciesi] = std::numeric_limits<scalar>::infinity();
            }

            //use the iterations to calculate field variations and decide state

            auto residual = gMax((mag(Y-Y_old))->internalField());

            Info << "Species " << composition_.species()[speciesi] << " variation: "<< residual << endl;

            if (maxDeltaY[speciesi] != std::numeric_limits<scalar>::infinity()){ //because order of SPECIES's maxDeltaC could affect

                if (dtState_ != dtState::SHOULD_DECREASE)   //Implementing a finite state machine
                {
                    if (residual >= 1.*maxDeltaY[speciesi]){
                        dtState_ = dtState::SHOULD_DECREASE;
                    } 
                    else if (residual >= 0.75*maxDeltaY[speciesi]){
                        dtState_ = dtState::SHOULD_MAINTAIN;
                    }
                    else if (dtState_ != dtState::SHOULD_MAINTAIN){
                        dtState_ = dtState::SHOULD_INCREASE;
                    }
                }
            }
        }
    }
}
