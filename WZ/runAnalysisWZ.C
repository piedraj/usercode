void runAnalysisWZ(TString sample     = "203_DoubleMuon2012B",
		   Int_t   systematic = 0,
		   Int_t   mode       = 1)
{
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ(sample, systematic, mode);
}
