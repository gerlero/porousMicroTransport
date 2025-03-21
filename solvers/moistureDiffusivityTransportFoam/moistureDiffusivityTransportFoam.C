#include "porousMedium.H"
#include "fluidPhase.H"
#include "phaseFractionField.H"
#include "moistureDiffusivityModel.H"
#include "porousMixture.H"
#include "PicardControl.H"
#include "TransportControl.H"

#if __has_include("reagency.H")
#include <reagency.H>
#endif

#include <fvCFD.H>
#include <fvOptions.H>
#include <messageStream.H>

int main(int argc, char *argv[])
{
    #include <setRootCaseLists.H>

    #include <createTime.H>
    #include <createMesh.H>

    #include "createFields.H"
    #include "createTransportFields.H"

    Pmt::TransportControl<Pmt::PicardControl> control{composition, runTime, theta};

    Info<< "\nStarting time loop\n" << endl;

    while (control.loop())
    {
        do
        {
            while (control.correct())
            {
                #include "thetaEqn.H"
            }
            
            #include "CEqn.H"

        }  while (control.retry());

        theta.writeMinMax(Info);
        mag(U)->writeMinMax(Info);

        for (const auto& C : composition.Y())
        {
            C.writeMinMax(Info);
        }

        runTime.write();

        runTime.printExecutionTime(Info);
    }

    Info<< "End\n" << endl;

    return 0;
}
