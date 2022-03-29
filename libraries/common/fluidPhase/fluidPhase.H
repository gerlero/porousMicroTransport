#ifndef PMT_fluidPhase_H
#define PMT_fluidPhase_H

#include "fluidPhaseFwd.H"

#include <dictionary.H>
#include <word.H>
#include <dimensionedScalar.H>

namespace Foam
{
namespace Pmt
{

class fluidPhase
{
public:
    fluidPhase(const dictionary& transportProperties, word name={});

    const word& name() const noexcept;

    const dimensionedScalar& rho() const;

    const dimensionedScalar& mu() const;

    fluidPhase(const fluidPhase&) = delete;
    void operator=(const fluidPhase&) = delete;

private:
    word name_;
    dimensionedScalar rho_;
    bool rhoSet_;
    dimensionedScalar mu_;
    bool muSet_;
};

}
}

#include "fluidPhaseI.H"

#endif
