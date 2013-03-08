void runLatinosTreeScript(Double_t luminosity    = 19.468,
			  Int_t    jetChannel    = 1,
			  TString  flavorChannel = "MuE",
			  TString  theSample     = "DYtautau",
			  Bool_t   verbose       = true)
{
  gInterpreter->LoadMacro("LatinosTreeScript.C+");

  LatinosTreeScript(luminosity, jetChannel, flavorChannel, theSample, verbose);
}
