#ifndef PMT_TransportControl_H
#define PMT_TransportControl_H

#include "timeStepControl.H"

#include <basicMultiComponentMixture.H>
#include <scalarList.H>

namespace Foam
{
namespace Pmt
{

template<class Base=timeStepControl>
class TransportControl
:
    public Base
{
public:
    template<typename... Args_>
    TransportControl(basicMultiComponentMixture& composition, Args_&&... args);

    bool retry();

protected:
    scalar maxDeltaTValue() override;
    void operator++() override;
    void operator--() override;

private:
    enum class dtState
    {   
        UNKNOWN,
        SHOULD_DECREASE,
        SHOULD_MAINTAIN,
        SHOULD_INCREASE

    };

    dtState dtState_;
    basicMultiComponentMixture& composition_;

    void setDtState();
};

}
}

#include "TransportControlI.H"

#endif
