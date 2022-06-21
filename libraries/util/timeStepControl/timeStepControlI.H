inline Foam::scalar Foam::Pmt::timeStepControl::maxDeltaTValue()
{
    return controlDict().getCheckOrDefault("maxDeltaT", GREAT, [](scalar v){ return v>0; });
}

inline Foam::scalar Foam::Pmt::timeStepControl::deltaTValue() const
{
    return runTime_.deltaTValue();
}

inline const Foam::dictionary& Foam::Pmt::timeStepControl::controlDict() const
{
    return runTime_.controlDict();
}