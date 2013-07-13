void runAnalysisWZ(TString sample     = "037_DY50toLLMad",
		   Int_t   systematic = 0,
		   Int_t   mode       = 0,
		   TString muonJetEt  = "20",
		   TString elecJetEt  = "35",
		   TString directory  = "results")
{
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ(sample, systematic, mode, muonJetEt, elecJetEt, directory);
}
