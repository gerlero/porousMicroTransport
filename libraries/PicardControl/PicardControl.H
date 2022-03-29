#ifndef PMT_PicardControl_H
#define PMT_PicardControl_H

#include "timeStepControl.H"

#include <volFieldsFwd.H>
#include <scalar.H>
#include <label.H>

namespace Foam
{
namespace Pmt
{

class PicardControl
:
    public timeStepControl
{

public:
    PicardControl(Time& runTime, volScalarField& field);

    bool correct();

protected:
    scalar maxDeltaTValue() override;
    void operator++() override;
    void operator--() override;

private:
    volScalarField& field_;
    scalar tolerance_;
    label maxIter_;
    label iter_;
};

}
}

#endif
