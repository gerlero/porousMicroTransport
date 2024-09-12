#include "porousMedium.H"
#include "fluidPhase.H"
#include "phaseFractionField.H"
#include "porousMixture.H"
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

    Pmt::TransportControl<> control{composition, runTime};

    Info<< "\nStarting time loop\n" << endl;

    while (control.loop())
    {
        do
        {
            #include "CEqn.H"

        } while (control.retry());

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
