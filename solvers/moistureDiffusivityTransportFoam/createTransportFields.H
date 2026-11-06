Info<< "\nReading species\n" << endl;

Pmt::porousMixture composition{medium, phase, transportProperties};

#ifdef PMT_WITH_REAGENCY

Info<< "\nReading reactions\n" << endl;

reagency reactions{composition, transportProperties};

#else

if (transportProperties.found("reactions"))
{
    FatalErrorInFunction
        << "Reactions are listed in transportProperties, but "
        << "porousMicroTransport was built without reagency support." << nl
        << "Rebuild porousMicroTransport against reagency, or remove the "
        << "reactions." << nl
        << exit(FatalError);
}

#endif
