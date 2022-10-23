{
    D = flowModel->D(theta);

    fvScalarMatrix thetaEqn
    {
            fvm::ddt(theta)
            - fvm::laplacian(D, theta, "laplacian(D,theta)")
        ==
            fvOptions(theta)
    };

    fvOptions.constrain(thetaEqn);
    thetaEqn.solve();
    fvOptions.correct(theta);

    phi = thetaEqn.flux();
    U = fvc::reconstruct(phi);
}
