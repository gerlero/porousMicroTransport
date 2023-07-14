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
    porousMixture& composition_;
    scalarList maxDeltaY_;
    scalarList maxDYdT_;
    bool upToDate_;

    void update();
};

}
}

#include "transportControlI.H"

#endif
