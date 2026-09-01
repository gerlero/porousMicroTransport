{
    C = flowModel->C(p);
    M = flowModel->M();

    fvScalarMatrix pEqn
    {
            C*fvm::ddt(p)
          - fvm::laplacian(M, p, "laplacian(M,p)")
        ==
            fvOptions(p)
    };

    fvOptions.constrain(pEqn);
    pEqn.solve();
    fvOptions.correct(p);

    phi = pEqn.flux();
    U = fvc::reconstruct(phi);
}
