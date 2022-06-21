#include "timeStepControl.H"

#include <messageStream.H>
#include <error.H>

Foam::Pmt::timeStepControl::timeStepControl(Time& runTime)
:
    runTime_{runTime}
{}

bool Foam::Pmt::timeStepControl::loop()
{
    if (!runTime_.run())
    {
        return false;
    }

    if (runTime_.isAdjustTimeStep())
    {
        runTime_.setDeltaT(maxDeltaTValue());
    }

    ++*this;

    return true;
}

bool Foam::Pmt::timeStepControl::restartTimeStepIfAdjustable(scalar newDeltaT)
{
    if (!runTime_.isAdjustTimeStep())
    {
        return false;
    }

    Info<< "Retrying time step with reduced deltaT..." << nl
        << endl;

    if (newDeltaT >= runTime_.deltaTValue())
    {
        FatalErrorInFunction
            << "New time step must be smaller than the current time step" << nl
            << "deltaT: old = " << runTime_.deltaTValue() << ", new = " << newDeltaT << nl
            << exit(FatalError);
    }

    --*this;
    runTime_.setDeltaT(newDeltaT);
    ++*this;

    return true;
}

void Foam::Pmt::timeStepControl::operator--()
{
    if (runTime_.timeIndex() == oldTimeState_.timeIndex())
    {
        FatalErrorInFunction
            << "Cannot rewind time more than one time step" << nl
            << endl
            << exit(FatalError);
    }

    runTime_.TimeState::operator=(oldTimeState_);
}

void Foam::Pmt::timeStepControl::operator++()
{
    oldTimeState_ = runTime_;

    ++runTime_;

    Info<< "deltaT = " << runTime_.deltaTValue() << nl
        << "Time = " << runTime_.timeName() << nl << endl;
}
