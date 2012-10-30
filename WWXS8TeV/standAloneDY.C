#include "DY.C"


//TString directory = "rootfiles_wwxs/rootfiles_noCorrection";
//TString directory = "rootfiles_wwxs/rootfiles_type1";
//TString directory = "rootfiles_wwxs/rootfiles_type1_xy";
//TString directory = "rootfiles_wwxs/rootfiles_type1_xy_smear";

//TString directory = "rootfiles_highmass/rootfiles_noCorrection";
//TString directory = "rootfiles_highmass/rootfiles_type1";
//TString directory = "rootfiles_highmass/rootfiles_type1_xy";
//TString directory = "rootfiles_highmass/rootfiles_type1_xy_smear";

//TString directory = "rootfiles_highmass_pfmet/rootfiles_noCorrection";
//TString directory = "rootfiles_highmass_pfmet/rootfiles_type1";
//TString directory = "rootfiles_highmass_pfmet/rootfiles_type1_xy";
//TString directory = "rootfiles_highmass_pfmet/rootfiles_type1_xy_smear";

// pt 20/10 + pfmet
//TString directory = "hcp/rootfiles_noCorrection";
//TString directory = "hcp/rootfiles_xy";
TString directory = "hcp/rootfiles_xy_smear";

//TString directory = "rootfiles_lowmass/rootfiles_noCorrection";
//TString directory = "rootfiles_lowmass/rootfiles_type1";


//------------------------------------------------------------------------------
//
// standAloneDY
//
//------------------------------------------------------------------------------
void standAloneDY(Int_t  njet          = 1,
		  Bool_t useDataDriven = true,
		  Int_t  printLevel    = 2,
		  Bool_t drawR         = true)
{
  Double_t dyYield;
  Double_t dyStatError;
  Double_t dySystError;
  Double_t dyScaleFactor;

  DY(dyYield,
     dyStatError,
     dySystError,
     dyScaleFactor,
     njet,
     "SF",
     directory,
     useDataDriven,
     printLevel,
     drawR);
}
