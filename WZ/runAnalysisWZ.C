void runAnalysisWZ(TString sample     = "074_WZJetsMad",
		   Int_t   systematic = 0,
		   Int_t   mode       = 0,
		   Int_t   muonJetEt  = 20,
		   Int_t   elecJetEt  = 35,
		   TString directory  = "results")
{
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ(sample, systematic, mode, muonJetEt, elecJetEt, directory);
}
