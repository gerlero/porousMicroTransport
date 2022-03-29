Info<< "\nReading species\n" << endl;

Pmt::porousMixture composition{medium, phase, transportProperties};

Info<< "\nReading reactions\n" << endl;

Pmt::reactions reactions{composition, transportProperties};
