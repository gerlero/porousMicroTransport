#include "porousMedium.H"
#include "phaseFractionField.H"
#include "fluidPhase.H"
#include "moistureDiffusivityModel.H"
#include "PicardControl.H"

#include <fvCFD.H>
#include <fvOptions.H>
#include <messageStream.H>

int main(int argc, char *argv[])
{
    #include <setRootCaseLists.H>

    #include <createTime.H>
    #include <createMesh.H>

    #include "createFields.H"

    Pmt::PicardControl control(runTime, theta);

    Info<< "\nStarting time loop\n" << endl;

    while (control.loop())
    {
        while (control.correct())
        {
            #include "thetaEqn.H"
        }

        theta.writeMinMax(Info);
        mag(U)->writeMinMax(Info);

        runTime.write();

        runTime.printExecutionTime(Info);
    }

    Info<< "End\n" << endl;

    return 0;
}
