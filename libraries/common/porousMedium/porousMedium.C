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
            if (auto eps = constantFields::readIfPresent("eps", mesh, dimless, transportProperties))
            {
                return autoPtr<volScalarField>{eps.ptr()};  // ptr() call releases ownership
            }
            
            if (auto thetamax = constantFields::readIfPresent("thetamax", mesh, dimless, transportProperties))
            {
                return autoPtr<volScalarField>{thetamax.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    },
    K_
    {
        [&]
        {
            if (auto K = constantFields::readIfPresent("K", mesh, dimArea, transportProperties))
            {
                return autoPtr<volScalarField>{K.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    }
{
    Info<< nl
        << "Porous medium properties" << nl
        << "{" << nl
        << "    Porosity (eps | thetamax): ";
    if (eps_)
    {
        eps_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << nl;
    }
    Info<< "    Intrinsic permeability (K): ";
    if (K_)
    {
        K_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << nl;
    }
    Info<< "}" << nl
        << endl;
}
