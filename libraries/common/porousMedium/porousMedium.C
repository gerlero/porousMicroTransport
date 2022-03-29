#include "porousMedium.H"
#include "constantFields.H"

#include <messageStream.H>
#include <error.H>

Foam::Pmt::porousMedium::porousMedium(const fvMesh& mesh, const dictionary& transportProperties)
:
    mesh_{mesh},
    eps_
    {
        [&]
        {
            auto thetamax = constantFields::readIfPresent("thetamax", mesh, dimless, transportProperties);

            if (!thetamax)
            {
                return constantFields::read("eps", mesh, dimless, transportProperties);
            }

            if (auto eps = constantFields::readIfPresent("eps", mesh, dimless, transportProperties))
            {
                return eps;
            }

            return thetamax;
        }()
    },
    K_
    {
        [&]
        {
            if (auto K = constantFields::readIfPresent("K", mesh, dimArea, transportProperties))
            {
                return autoPtr<volScalarField>{K.ptr()}; // ptr() call releases ownership
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    }
{
    Info<< nl
        << "Porous medium properties" << nl
        << "{" << nl
        << "    Porosity (eps or thetamax): "; eps_.writeMinMax(Info);
    Info<< "    Intrinsic permeability K: ";
    if (K_)
    {
        K_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << endl;
    }
    Info<< "}" << nl
        << endl;
}
