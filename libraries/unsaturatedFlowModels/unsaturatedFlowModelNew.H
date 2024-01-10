#ifndef PMT_unsaturatedFlowModelNew_H
#define PMT_unsaturatedFlowModelNew_H

#include "porousMediumFwd.H"
#include "fluidPhaseFwd.H"
#include "dictionaries.H"
#include "runTimeSelectionHelpers.H"
#include "capillarityModel.H"
#include "relativePermeabilityModel.H"
#include "mixedRichardsModel.H"

#include <autoPtr.H>
#include <dictionary.H>
#include <word.H>
#include <messageStream.H>
#include <typeInfo.H>

#include <utility>
#include <cassert>

namespace Foam
{
namespace Pmt
{
namespace Detail
{

template<class Model>
autoPtr<Model> unsaturatedFlowModelNew
(
    const porousMedium& medium,
    const fluidPhase& phase,
    const phaseFractionField& frac,
    const dictionary& transportProperties
)
{
    word capillarityType;
    if (transportProperties.readIfPresent("capillarityModel", capillarityType))
    {
        Info<< "Selecting capillarity model => " << capillarityType << endl;

        auto capillarityInstance =
            NewOfSelectedType<capillarityModel>
            (
                capillarityType,
                medium,
                phase,
                frac,
                transportProperties
            );

        auto relativePermeabilityType =
            transportProperties.get<word>("relativePermeabilityModel");

        Info<< "Selecting relative permeability model => " << relativePermeabilityType << endl;

        if
        (
            relativePermeabilityType == capillarityType
         && isA<relativePermeabilityModel>(*capillarityInstance)
        )
        {
            if (auto instance = dynamicPtrCast<Model>(capillarityInstance))
            {
                assert(!capillarityInstance);
                return instance;
            }

            assert(capillarityInstance);
        }

        auto relativePermeabilityInstance =
            NewOfSelectedType<relativePermeabilityModel>
            (
                relativePermeabilityType,
                medium,
                phase,
                frac,
                transportProperties
            );

        return
            autoPtr<Model>::template NewFrom<mixedRichardsModel>
            (
                std::move(capillarityInstance),
                std::move(relativePermeabilityInstance)
            );
    }

    auto modelType = transportProperties.get<word>("unsaturatedFlowModel");

    Info<< "Selecting unsaturated flow model => " << modelType << endl;

    return NewOfSelectedType<Model>
        (
            modelType,
            medium,
            phase,
            frac,
            transportProperties
        );
}

}
}
}

#endif
