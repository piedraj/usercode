void runAnalysisWZ(TString sample     = "203_DoubleMuon2012B",
		   Int_t   systematic = 8,
		   Int_t   mode       = 1,
		   Int_t   muonJetEt  = 15,
		   Int_t   elecJetEt  = 15,
		   TString directory  = "results")
{
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ(sample, systematic, mode, muonJetEt, elecJetEt, directory);
}
