# microRichardsFoam tutorials

## `imbibition1D`

One-dimensional capillary imbibition in a homogeneous porous medium using the
Van Genuchten model. The initial pressure is -34.196 kPa and the inlet is held
at 0 Pa (saturation). The corresponding moisture contents are approximately
0.025 and 0.7, respectively.

This is the pressure-form Richards counterpart of the standard
`moistureDiffusivityFoam` similarity-solution problem and is useful for comparing
`p`, `theta`, and `U` during unsaturated imbibition.

## `forcedFlow1D`

One-dimensional forced infiltration into an initially unsaturated homogeneous
porous medium. The initial pressure is -34.196 kPa, the inlet is held at 1 kPa,
and the outlet is impermeable (`zeroGradient`). The positive inlet pressure
creates a saturated region behind a transient wetting front.


Run either case with its `run` script and restore it with `clean`. Run or clean
both cases from this directory with `Allrun` or `Allclean`.
