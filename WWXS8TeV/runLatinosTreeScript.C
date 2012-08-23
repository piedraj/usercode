void runLatinosTreeScript(Double_t luminosity    = 5.064,
			  Int_t    jetChannel    = 0,
			  TString  flavorChannel = "All",
			  TString  theSample     = "WWTo2L2Nu",
			  Bool_t   verbose       = true)
{
  gInterpreter->LoadMacro("LatinosTreeScript.C+");

  LatinosTreeScript(luminosity, jetChannel, flavorChannel, theSample, verbose);
}
