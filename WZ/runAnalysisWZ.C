//void runAnalysisWZ(TString sample     = "Run2012A_889.4pbinv",
//void runAnalysisWZ(TString sample     = "Run2012B_4417pbinv",
//void runAnalysisWZ(TString sample     = "Run2012C_7094pbinv",
void runAnalysisWZ(TString sample     = "Run2012D_7308pbinv",
//void runAnalysisWZ(TString sample     = "074_WZJetsMad",
//void runAnalysisWZ(TString sample     = "074_WZJetsMad_GenVars",
//void runAnalysisWZ(TString sample     = "076_WZJetsMad_TuneZ2star",
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
