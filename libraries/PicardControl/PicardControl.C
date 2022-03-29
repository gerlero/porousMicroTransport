#include "PicardControl.H"

#include <volFields.H>
#include <messageStream.H>

Foam::Pmt::PicardControl::PicardControl(Time& runTime, volScalarField& field)
:
    timeStepControl{runTime},
    field_{field},
    tolerance_{-1.0},
    maxIter_{-1},
    iter_{-1}
{}

bool Foam::Pmt::PicardControl::correct()
{
    if (iter_ > 0)
    {
        auto residual = gMax((mag(field_-field_.prevIter()))->internalField());
        Info<< "Picard iteration " << iter_ << ": residual = " << residual << endl;

        if (residual <= tolerance_)
        {
            return false;
        }

        if (iter_ >= maxIter_)
        {
            Info<< "Max Picard iteration reached" << nl
                << endl;
            
            if (restartTimeStepIfAdjustable(deltaTValue()/3))
            {
                return true;
            }
            else
            {
                Warning
                    << "Max Picard iteration reached with adjustTimeStep off" << nl
                    << endl;
                return false;
            }
        }
    }

    field_.storePrevIter();

    ++iter_;
    return true;
}

void Foam::Pmt::PicardControl::operator++()
{
    maxIter_ = 10;
    tolerance_ = 0.0;

    if (const auto* Picard = field_.mesh().solutionDict().findDict("Picard"))
    {
        Picard->readCheckIfPresent("maxIter", maxIter_, [](label v){ return v>=8; });
        Picard->readCheckIfPresent("tolerance", tolerance_, [](scalar v){ return v>=0; });
    }

    iter_ = 0;

    timeStepControl::operator++();
}

void Foam::Pmt::PicardControl::operator--()
{
    field_ = field_.oldTime();

    timeStepControl::operator--();
}

Foam::scalar Foam::Pmt::PicardControl::maxDeltaTValue()
{
    scalar deltaTFact{1.0};

    if (iter_ > 0 && iter_ <= 3)
    {
        deltaTFact =
            controlDict().getCheckOrDefault("deltaTUpFact", 1.3, [](scalar v){ return v>1; });
    }
    else if (iter_ >= 7)
    {
        deltaTFact =
            controlDict().getCheckOrDefault("deltaTDownFact", 0.7, [](scalar v){ return v>0 && v<1; });
    }

    return min(timeStepControl::maxDeltaTValue(), deltaTFact*deltaTValue());
}
