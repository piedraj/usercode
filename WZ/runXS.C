void runXS(UInt_t cut     = 4,
	   UInt_t mode    = 0,
	   UInt_t wcharge = 0,
	   Int_t  njet    = -1)
{
  gInterpreter->LoadMacro("XS.C+");

  XS(cut, mode, wcharge, njet);
}
