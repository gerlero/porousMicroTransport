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
    },
    rs_
    {
        [&]
        {
            const auto& porousTransport = transportProperties.optionalSubDict("porousTransport");

            if (auto rs = constantFields::readIfPresent("rs", mesh, dimDensity, porousTransport))
            {
                return autoPtr<volScalarField>{rs.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    },
    epsTotal_
    {
        [&]
        {
            if (auto epsTotal = constantFields::readIfPresent("epsTotal", mesh, dimless, transportProperties))
            {
                return autoPtr<volScalarField>{epsTotal.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    },
    tau_
    {
        [&]
        {
            if (auto tau = constantFields::readIfPresent("tau", mesh, dimless, transportProperties.optionalSubDict("alphaDispersionCoeffs")))
            {
                return autoPtr<volScalarField>{tau.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    },
    alphaT_
    {
        [&]
        {
            if (auto alphaT = constantFields::readIfPresent("alphaT", mesh, dimLength, transportProperties.optionalSubDict("alphaDispersionCoeffs")))
            {
                return autoPtr<volScalarField>{alphaT.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    },
    alphaL_
    {
        [&]
        {
            if (auto alphaL = constantFields::readIfPresent("alphaL", mesh, dimLength, transportProperties.optionalSubDict("alphaDispersionCoeffs")))
            {
                return autoPtr<volScalarField>{alphaL.ptr()};
            }

            return autoPtr<volScalarField>{nullptr};
        }()
    }
{
    Info<< nl
        << "Porous medium properties" << nl
        << "{" << nl
        << "    Effective porosity (eps | thetamax): ";
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
    Info<< "    Particle density (rs): ";
    if (rs_)
    {
       rs_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << nl;
    }
    Info<< "    Total porosity (epsTotal): ";
    if (epsTotal_)
    {
        epsTotal_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set (defaults to effective porosity)" << nl;
    }
    Info<< "    Diffusive tortuosity (tau): ";
    if (tau_)
    {
        tau_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << nl;
    }
    Info<< "    Transverse dispersion coefficient (alphaT): ";
    if (alphaT_)
    {
        alphaT_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << nl;
    }
    Info<< "    Longitudinal dispersion coefficient (alphaL): ";
    if (alphaL_)
    {
        alphaL_->writeMinMax(Info);
    }
    else
    {
        Info<< "not set" << nl;
    }
    Info<< "}" << nl
        << endl;


    auto dispersionModel = transportProperties.getOrDefault<word>("dispersionModel", {});
    if (!dispersionModel.empty() && dispersionModel != "alphaDispersion")
    {
        FatalErrorInFunction
            << "Dispersion models (other than the default alphaDispersion) are not supported by porousMicroTransport" << endl
            << exit(FatalError);
    }
}
