#ifndef PMT_timeStepControl_H
#define PMT_timeStepControl_H

#include <Time.H>
#include <TimeState.H>
#include <scalarFwd.H>
#include <dictionary.H>

namespace Foam
{
namespace Pmt
{

class timeStepControl
{
public:
    explicit timeStepControl(Time& runTime);

    bool loop();

    virtual ~timeStepControl() = default;

    timeStepControl(const timeStepControl&) = delete;
    void operator=(const timeStepControl&) = delete;

protected:
    virtual scalar maxDeltaTValue();
    virtual void operator++();
    virtual void operator--();

    scalar deltaTValue() const;
    const dictionary& controlDict() const;
    bool restartTimeStepIfAdjustable(scalar newDeltaT);

private:
    Time& runTime_;
    TimeState oldTimeState_;
};

}
}

#endif
