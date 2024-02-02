#ifndef PMT_transportControl_H
#define PMT_transportControl_H

#include "timeStepControl.H"

#include <basicMultiComponentMixture.H>
#include <scalarList.H>

namespace Foam
{
namespace Pmt
{

template<class Base=timeStepControl>
class transportControl
:
    public Base
{
public:
    template<typename... Args_>
    transportControl(porousMixture& composition, Args_&&... args);

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
    porousMixture& composition_;

    void setDtState();
};

}
}

#include "transportControlI.H"

#endif
