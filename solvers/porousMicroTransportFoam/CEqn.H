//reactions.correct();

forAll(composition.Y(), speciesi)
{
    auto& C = composition.Y(speciesi);
    auto Deff = composition.Deff(speciesi, U, theta);
    const auto& R = reactions.reactionTerm(speciesi);

    fvScalarMatrix CEqn
    {
            fvm::ddt(theta, C)
          + fvm::div(phi, C, "div(phi,C)")
          - fvm::laplacian(theta*Deff, C, "laplacian(Deff,C)")
        ==
            theta*R
          + theta*fvOptions(C)
    };

    fvOptions.constrain(CEqn);
    CEqn.solve(mesh.solver("C"));
    fvOptions.correct(C);
}
