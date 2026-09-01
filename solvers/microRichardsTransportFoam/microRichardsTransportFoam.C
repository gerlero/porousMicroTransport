#include "porousMedium.H"
#include "phaseFractionField.H"
#include "fluidPhase.H"
#include "RichardsModel.H"
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

    Pmt::TransportControl<Pmt::PicardControl> control{composition, runTime, p};

    Info<< "\nStarting time loop\n" << endl;
    while (control.loop())
    {
        do
        {
            while (control.correct())
            {
                #include "pEqn.H"
            }

            #include "CEqn.H"

        } while (control.retry());

        p.writeMinMax(Info);
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
