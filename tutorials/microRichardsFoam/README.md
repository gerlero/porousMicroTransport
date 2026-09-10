# microRichardsFoam tutorials

## `imbibition1D`

One-dimensional capillary imbibition in a homogeneous porous medium using the
Van Genuchten model. The initial pressure is -34.196 kPa and the inlet is held
at -3.0165 kPa. The corresponding moisture contents are approximately 0.025 and
0.7, respectively.

This is the pressure-form Richards counterpart of the standard
`moistureDiffusivityFoam` similarity-solution problem and is useful for comparing
`p`, `theta`, and `U` during unsaturated imbibition.

## `forcedFlow1D`

One-dimensional saturated pressure-driven flow. The inlet is held at 1 kPa and
the outlet at 0 Pa. Since `p >= 0`, the Van Genuchten model is saturated
throughout (`theta = thetamax = 0.7`), and the steady Darcy velocity is

    Ux = (K/mu) * (1000 Pa / 0.03 m) = 3.333333e-6 m/s.

This case demonstrates the forced-flow regime that is available in the Richards
pressure formulation.

Run either case with its `run` script and restore it with `clean`. Run or clean
both cases from this directory with `Allrun` or `Allclean`.
