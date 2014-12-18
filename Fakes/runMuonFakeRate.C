void runMuonFakeRate()
{
  gInterpreter->LoadMacro("MuonFakeRate.C++");

  MuonFakeRate mfr;

  mfr.Loop();
}
