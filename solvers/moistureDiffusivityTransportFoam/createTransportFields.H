Info<< "\nReading species\n" << endl;

Pmt::porousMixture composition{medium, phase, transportProperties};

#if __has_include("reagency.H")

Info<< "\nReading reactions\n" << endl;

reagency reactions{composition, transportProperties};

#endif
