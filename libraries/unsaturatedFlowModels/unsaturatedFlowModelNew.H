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
    const dictionary& transportProperties
)
{
    word capillarityType;
    if (transportProperties.readIfPresent("capillarityModel", capillarityType))
    {
        Info<< "Selecting capillarity model => " << capillarityType << endl;

        const auto& capillarityCoeffs =
            dictionaries::subOrNullDict
            (
                transportProperties,
                capillarityType + "Coeffs"
            );

        auto capillarityInstance =
            NewOfSelectedType<capillarityModel>
            (
                capillarityType,
                medium,
                phase,
                capillarityCoeffs
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
            if (auto instance = dynamicPtrCast<autoPtr<Model>>(capillarityInstance))
            {
                assert(!capillarityInstance);
                return instance;
            }
        }

        const auto& relativePermeabilityCoeffs =
            dictionaries::subOrNullDict
            (
                transportProperties,
                relativePermeabilityType + "Coeffs"
            );

        auto relativePermeabilityInstance =
            NewOfSelectedType<relativePermeabilityModel>
            (
                relativePermeabilityType,
                medium,
                phase,
                relativePermeabilityCoeffs
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

    const auto& coeffs = 
        dictionaries::subOrNullDict(transportProperties, modelType + "Coeffs");

    return NewOfSelectedType<Model>(modelType, medium, phase, coeffs);
}

}
}
}

#endif
