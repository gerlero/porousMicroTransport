composition.correct(U, theta);
reactions.correct();

forAll(composition.Y(), speciesi)
{
    auto& C = composition.Y(speciesi);
    auto Deff = composition.Deff(speciesi);
    const auto& Rd = composition.retardation(speciesi);
    const auto& F = reactions.reactionTerm(speciesi);

    auto ddtScheme = composition.ddtScheme(speciesi);

    fvScalarMatrix CEqn
    {
          ddtScheme->fvmDdt(Rd, theta, C)
          + fvm::div(phi, C, "div(phi,C)")
          - fvm::laplacian(theta*Deff, C, "laplacian(Deff,C)")
        ==
            Rd*theta*F
          + theta*fvOptions(C)
    };

    fvOptions.constrain(CEqn);
    CEqn.solve(mesh.solver("C"));
    fvOptions.correct(C);
}
