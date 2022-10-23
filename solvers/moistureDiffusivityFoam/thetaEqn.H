{
    Dtheta = flowModel->D(theta);

    fvScalarMatrix thetaEqn
    {
            fvm::ddt(theta)
            - fvm::laplacian(Dtheta, theta)
        ==
            fvOptions(theta)
    };

    fvOptions.constrain(thetaEqn);
    thetaEqn.solve();
    fvOptions.correct(theta);

    phi = thetaEqn.flux();
    U = fvc::reconstruct(phi);
}
