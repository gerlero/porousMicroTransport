# porousMicroTransport

[![CI](https://github.com/gerlero/porousMicroTransport/actions/workflows/ci.yml/badge.svg)](https://github.com/gerlero/porousMicroTransport/actions/workflows/ci.yml) [![OpenFOAM versions](https://img.shields.io/badge/openfoam-v2012%20%7C%20v2106%20%7C%20v2112%20%7C%20v2206%20%7C%20v2212-informational)](https://www.openfoam.com) [![Docker](https://github.com/gerlero/porousMicroTransport/actions/workflows/docker.yml/badge.svg)](https://github.com/gerlero/porousMicroTransport/actions/workflows/docker.yml) [![Docker image](https://img.shields.io/badge/docker%20image-microfluidica%2Fporousmicrotransport-0085a0)](https://hub.docker.com/r/microfluidica/porousmicrotransport/)

**porousMicroTransport** is a set of additional solvers and related libraries for OpenFOAM developed for the purposes of simulating flow and transport in porous media, with an emphasis on paper-based microfluidics


## Installation

### Requirements

**porousMicroTransport** requires [OpenFOAM](https://www.openfoam.com), as distributed by OpenCFD (openfoam.com). Compatible OpenFOAM versions are v2012, v2106, v2112, v2206 and v2212.

_Versions produced by the OpenFOAM Foundation (openfoam.org) (e.g. OpenFOAM 9, OpenFOAM 10) are not compatible._

**porousMicroTransport** is provided as source code and must be compiled before use.

### Download

[Download the source code](https://github.com/gerlero/porousMicroTransport/archive/refs/heads/main.zip) of **porousMicroTransport**, or clone this repository with Git:

```sh
git clone https://github.com/gerlero/porousMicroTransport.git
```

### Compile and install

To build and install **porousMicroTransport**, just invoke the top-level `Allwmake` script:

```sh
cd porousMicroTransport
./Allwmake -j
```

_If necessary, activate/source the correct OpenFOAM environment before running `Allwmake`._

### Test

Optionally, you can verify the installation of **porousMicroTransport** by running the included test suite (requires Python 3).

```sh
tests/runtests.sh
```

### Docker image

Alternatively, you can use the [Docker image](https://hub.docker.com/r/microfluidica/porousmicrotransport/) of **porousMicroTransport**. This image is based on the official OpenFOAM Docker image and includes **porousMicroTransport** precompiled and ready to use. If you have [Docker](https://www.docker.com) installed, you can run the image in a new container with:

```sh
docker run -it microfluidica/porousmicrotransport:main
```

## Solvers

### `moistureDiffusivityFoam`

**(Unsaturated) capillarity-driven flow in a porous medium**, governed by the moisture diffusivity equation[^Bear]:

$$\frac{\partial\theta}{\partial t} - \nabla\cdot\left[D\nabla\theta\right] = 0$$

where $\theta$ is the moisture content and $D$ is a saturation-dependent diffusivity as defined by an unsaturated flow model.

### `porousMicroTransportFoam`

**Transport by steady flow of any number of species in a porous medium, with optional reactions between the species**. For each species (concentration $C$), the governing equation is:

$$\frac{\partial\theta C}{\partial t} + \nabla\cdot\left[UC\right] - \nabla\cdot\left[\theta D_{eff}\nabla C\right] = \theta R$$

where $D_{eff}$ is an effective diffusivity tensor—which models the effects of molecular diffusion and mechanical dispersion—and $R$ is a reaction term (see below).

### `moistureDiffusivityTransportFoam`

**Capillary flow + reactive transport in a porous medium**, coupling the moisture diffusivity equation for flow with the previous transport equation.


## Case layout

The layout of **porousMicroTransport** cases follows many conventions of [**porousMultiphaseFoam**](https://github.com/phorgue/porousMultiphaseFoam), especially in field names and entries in the `transportProperties` dictionary. This allows for easy conversion of cases from **porousMultiphaseFoam** to **porousMicroTransport** (and to some extent, vice versa).

### Common fields

These variable fields are defined in the time directories:

* `theta`: moisture content (scalar). _Optional for `porousMicroTransportFoam`_

* `U`: Darcy velocity (vector). _Optional for flow solvers_

### Common porous medium properties

Defined as scalar fields in `constant` or as dictionary entries in `transportProperties`:

* `eps` or `thetamax`: porosity

* `K`: intrinsic permeability. _Flow solvers only_

### Phase properties

_Flow solvers only._

Set these in a `phase.theta` subdictionary in `transportProperties`:

* `rho`: density

* `mu`: dynamic viscosity

### Moisture content options

_Flow solvers only._

Defined as scalar fields in `constant` or as dictionary entries in `transportProperties`:

* `thetamin`: minimum (a.k.a. residual) moisture content

* `thetamax`: maximum moisture content (usually equal to the porosity)

### Unsaturated flow models

_Flow solvers only._

Supported models of unsaturated flow are:

* `BrooksAndCorey`: Brooks and Corey[^BrooksAndCorey] model

    * In coefficient dictionary `BrooksAndCoreyCoeffs`: `pc0`, `alpha`, `n`, `l` (optional)

* `VanGenuchten`: Van Genuchten[^VanGenuchten] model

    * In coefficient dictionary `VanGenuchtenCoeffs`: `pc0`, `m` or `n`, `l` (optional)

* `LETxs`: LETx + LETs model[^LETxs]

    * In coefficient dictionary `LETxsCoeffs`: `pc0`, `Lw`, `Ew`, `Tw`, `Ls`, `Es`, `Ts`

* `LETd`: LETd[^LETd] model

    * In coefficient dictionary `LETdCoeffs`: `pc0`, `L`, `E`, `T`

To choose a model for your simulation, set the `unsaturatedFlowModel` entry in `transportProperties`. Then set the model-specific parameters in the corresponding coefficient subdictionary.

### Transported species

_Transport solvers only._

A `species` list in `transportProperties` contains the names of all transported species.

Each species must also define its own scalar concentration field (named the same as the species).

For each species, the following entries are required in `transportProperties`:

* `Dm`: molecular diffusivity ($D_M$)

* `dispersionModel`: selection of a dispersion model (see below)

* Coefficient dictionary for the selected dispersion model

### Dispersion models

_Transport solvers only._

Supported dispersion models for the species are:

* `alphaDispersion`: (an)isotropic mechanical dispersion with transverse and longitudinal coefficients. Defines the effective diffusivity of the species as:

   $$D_{eff} = \left(\frac{D_M}{\tau} + \alpha_T|V|\right)I + \left(\alpha_L - \alpha_T\right)\frac{VV}{|V|}$$

   where $I$ is the identity tensor and $V$ is the true velocity of the fluid ($=U/\theta$)

   * In coefficient dictionary `alphaDispersionCoeffs`:

        * `alphaT`: transverse dispersion coefficient ($\alpha_T$)
        * `alphaL`: longitudinal dispersion coefficient ($\alpha_L$)
        * `tau`: tortuosity ($\tau$)

### Reactions

_Transport solvers only._

Reactions are defined in a `reactions` subdictionary in `transportProperties`. The `reactions` dictionary contains a list of subdictionaries, each of which defines a single reaction. A reaction can have an arbitrary name and should contain the following entries:

* `reaction`: reaction equation. E.g. `"A^2 + B = 2C + D"`, where `A`, `B`, `C` and `D` are names of defined species

* `kf`: forward rate constant

* `kr`: optional reverse rate constant (for reversible reactions)


## Tutorials

Sample cases are available in the [`tutorials` directory](tutorials).


## Related projects

* [**porousMultiphaseFoam**](https://github.com/phorgue/porousMultiphaseFoam)[^porousMultiphaseFoam]: toolbox for OpenFOAM for modeling multiphase flow and transport. As previously stated, **porousMicroTransport** is mostly compatible with **porousMultiphaseFoam** in terms of case definitions, and can be installed alongside it.

* [**electroMicroTransport**](https://gitlab.com/santiagomarquezd/electroMicroTransport)[^electroMicroTransport]: toolbox for OpenFOAM dedicated to electromigrative separations. It includes support for modeling separations in paper-based media, and can also be installed alongside **porousMicroTransport**.


[^Bear]: [Bear, J., Cheng, A.H.-D.: Modeling Groundwater Flow and Contaminant Transport. Springer Dordrecht (2010)](https://doi.org/10.1007/978-1-4020-6682-5)

[^porousMultiphaseFoam]: [Horgue, P., Renard, F., Gerlero, G.S., Guibert, R., Debenest, G.: porousMultiphaseFoam v2107: An open-source tool for modeling saturated/unsaturated water flows and solute transfers at watershed scale. Comput. Phys. Comm., **273**, 108278 (2022)](https://doi.org/10.1016/j.cpc.2021.108278)

[^electroMicroTransport]: [Gerlero, G.S., Marquez Damián, S., Kler, P.A.: electroMicroTransport v2107: Open-source toolbox for paper-based electromigrative separations. Comput. Phys. Comm., **269**, 108143 (2021)](https://doi.org/10.1016/j.cpc.2021.108143)

[^BrooksAndCorey]: [Brooks, R., Corey, T.: Hydraulic properties of porous media. Hydrol. Pap. Colo. State Univ., **24**, 37 (1964)](https://mountainscholar.org/bitstream/handle/10217/61288/HydrologyPapers_n3.pdf)

[^VanGenuchten]: [Van Genuchten, M.Th.: A closed-form equation for predicting the hydraulic conductivity of unsaturated soils. Soil Sci. Soc. Am. J., **44**, 892–898 (1980)](https://doi.org/10.2136/sssaj1980.03615995004400050002x)

[^LETxs]: [Lomeland, F.: Overview of the LET family of versatile correlations for flow functions. In: Proceedings of the International Symposium of Core Analysts, pp. SCA2018-056 (2018)](http://www.jgmaas.com/SCA/2018/SCA2018-056.pdf)

[^LETd]: [Gerlero, G.S., Valdez, A.R., Urteaga, R., Kler, P.A.: Validity of Capillary Imbibition Models in Paper-Based Microfluidic Applications. Transp. Porous Media, **141**, 359-378 (2022)](https://doi.org/10.1007/s11242-021-01724-w)
