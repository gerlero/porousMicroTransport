composition.correct(U, theta);

#ifdef PMT_WITH_REAGENCY
reactions.correct();
#endif

forAll(composition.Y(), speciesi)
{
    auto& C = composition.Y(speciesi);
    auto Deff = composition.Deff(speciesi);
    const auto& Rd = composition.retardation(speciesi);
    auto ddtScheme = composition.ddtScheme(speciesi);

    fvScalarMatrix CEqn
    {
          ddtScheme->fvmDdt(Rd, theta, C)
          + fvm::div(phi, C, "div(phi,C)")
          - fvm::laplacian(theta*Deff, C, "laplacian(Deff,C)")
        ==
          theta*fvOptions(C)
    };

#ifdef PMT_WITH_REAGENCY
    CEqn -= Rd*theta*reactions.reactionTerm(speciesi);
#endif

    fvOptions.constrain(CEqn);
    CEqn.solve(mesh.solver("C"));
    fvOptions.correct(C);
}
