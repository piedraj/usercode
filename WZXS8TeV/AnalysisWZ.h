#ifndef ANALYSIS_WZ_H
#define ANALYSIS_WZ_H 1

#include "packages/CMSAnalysisSelectorMiniTrees/CMSAnalysisSelectorMiniTrees.h"
#include "packages/PUWeight/PUWeight.h"
#include "packages/TProofVector/TProofVector.h"

#include "TH1F.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <fstream>
#include <iostream>
#include <vector>


// Lepton scale factors
//------------------------------------------------------------------------------
// 30-10-2012
const Double_t SF_trigger_mumu = 0.965;  // +- 0.0102;
const Double_t SF_trigger_ee   = 0.962;  // +- 0.0130;
const Double_t SF_trigger_mue  = 0.943;  // +- 0.0120;

// 02-11-2012
const Double_t SF_IDISO_mumu = 0.997;  // +- 0.0009;
const Double_t SF_IDISO_ee   = 0.975;  // +- 0.0006;
const Double_t SF_IDISO_mue  = 0.986;  // +- 0.0007;

const Double_t SFmumu = SF_trigger_mumu * SF_IDISO_mumu;
const Double_t SFee   = SF_trigger_ee   * SF_IDISO_ee;
const Double_t SFmue  = SF_trigger_mue  * SF_IDISO_mue;


const Double_t Z_MASS = 91.1876;  // GeV

const UInt_t nChannels = 4;

enum {MMM, EEE, MME, EEM};

TString sChannel[] = {"MMM", "EEE", "MME", "EEM"};


const UInt_t nCuts = 3;

enum {
  PreSelection,
  ZCandidate,
  WCandidate
};

TString sCut[] = {
  "PreSelection",
  "ZCandidate",
  "WCandidate"
};


// Forward declaration
class TLorentzVector;


class AnalysisWZ: public CMSAnalysisSelectorMiniTrees
{
 public:
  AnalysisWZ(TTree* tree = 0);

  virtual ~AnalysisWZ() {}


 protected:
  virtual void Initialise();
  virtual void InsideLoop();
  virtual void SetDataMembersAtTermination();
  virtual void Summary();


  // My member functions
  //----------------------------------------------------------------------------
  void     GetParameters       ();

  Double_t SelectedMuonPt      (UInt_t iMuon);

  Double_t SelectedElectronPt  (UInt_t iElec);

			   
  void     GetSelectedMuons    (Double_t ptMin);

  void     GetSelectedElectrons(Double_t ptMin);

  void     FillHistogramsAtCut (UInt_t   iChannel,
				UInt_t   iCut);


 public:
  
  // Histograms
  //----------------------------------------------------------------------------
  TH1F*                       hCounterEff[nChannels][nCuts];
  TH1F*                       hCounter   [nChannels][nCuts];
  TH1F*                       hNPV       [nChannels][nCuts];
  TH1F*                       hMET       [nChannels][nCuts];
  TH1F*                       hPtZLepton1[nChannels][nCuts];
  TH1F*                       hPtZLepton2[nChannels][nCuts];
  TH1F*                       hPtWLepton [nChannels][nCuts];
  TH1F*                       hInvMassZ  [nChannels][nCuts];


  // Input parameters
  //----------------------------------------------------------------------------
  TString                     folder;
  TString                     sample;
  TString                     fileSuffix;
  Double_t                    efficiency_weight;
  Double_t                    pu_weight;
  Double_t                    xs_weight;
  Double_t                    luminosity;
  PUWeight*                   fPUWeight;


  // Data members
  //----------------------------------------------------------------------------
  std::vector<Double_t>       Muons_Charge;
  std::vector<Double_t>       Electrons_Charge;

  std::vector<TLorentzVector> Muons;
  std::vector<TLorentzVector> Electrons;

  TLorentzVector              ZLepton1;
  TLorentzVector              ZLepton2;
  TLorentzVector              WLepton;

  Double_t                    dileptonInvMass;
  UInt_t                      nSelMuon;
  UInt_t                      nSelElec;
  UInt_t                      theChannel;


  ClassDef(AnalysisWZ, 0);
};


#endif
