void runAnalysisWZ(TString sample     = "074_WZJetsMad",
		   Int_t   systematic = 0,
		   Int_t   mode       = 0,
		   Int_t   jetChannel = -1,
		   TString muonJetEt  = "20",
		   TString elecJetEt  = "35",
		   TString directory  = "results")
{
  gInterpreter->LoadMacro("AnalysisWZ.C+");

  AnalysisWZ(sample,
	     systematic,
	     mode,
	     jetChannel,
	     muonJetEt,
	     elecJetEt,
	     directory);
}
