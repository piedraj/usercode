#ifndef ANALYSIS_WW_H
#define ANALYSIS_WW_H 1

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
const Double_t SF_IDISO_mumu = 0.997; // +- 0.0009;
const Double_t SF_IDISO_ee   = 0.975; // +- 0.0006;
const Double_t SF_IDISO_mue  = 0.986; // +- 0.0007;

const Double_t SFmumu = SF_trigger_mumu * SF_IDISO_mumu;
const Double_t SFee   = SF_trigger_ee   * SF_IDISO_ee;
const Double_t SFmue  = SF_trigger_mue  * SF_IDISO_mue;


const Double_t Z_MASS = 91.1876;  // GeV

const UInt_t nChannels = 4;

enum {MuMu, EE, EMu, MuE};

TString sChannel[] = {"MuMu", "EE", "EMu", "MuE"};


const UInt_t nCuts = 2;

enum {
  TwoLeptons,
  WWLevel
};

TString sCut[] = {
  "TwoLeptons",
  "WWLevel"
};


// Forward declaration
class TLorentzVector;


class AnalysisWW: public CMSAnalysisSelectorMiniTrees
{
 public:
  AnalysisWW(TTree* tree = 0);

  virtual ~AnalysisWW() {}


 protected:
  virtual void Initialise();
  virtual void InsideLoop();
  virtual void SetDataMembersAtTermination();
  virtual void Summary();


  // My member functions
  //----------------------------------------------------------------------------
  void     GetParameters       ();

  Double_t SelectedMuonPt      (UInt_t   iMuon);

  Double_t SelectedElectronPt  (UInt_t   iElec);

  void     GetSelectedMuons    (Double_t ptMin);

  void     GetSelectedElectrons(Double_t ptMin);

  UInt_t   NumberOfJets        (Double_t ptMin,
				Double_t etaMax,
				Double_t tagCut);

  Bool_t   PassesDPhillJet     (Double_t ptMin,
				Double_t etaMax);

  void     FillHistogramsAtCut (UInt_t   iChannel,
				UInt_t   iCut);


 public:
  
  // Histograms
  //----------------------------------------------------------------------------
  TH1F*                       hCounterEff[nChannels][nCuts];
  TH1F*                       hCounter   [nChannels][nCuts];
  TH1F*                       hNPV       [nChannels][nCuts];
  TH1F*                       hMET       [nChannels][nCuts];
  TH1F*                       hPtLepton1 [nChannels][nCuts];
  TH1F*                       hPtLepton2 [nChannels][nCuts];
  TH1F*                       hMll       [nChannels][nCuts];


  // Input parameters
  //----------------------------------------------------------------------------
  TString                     sample;
  TString                     fileSuffix;
  Double_t                    efficiency_weight;
  Double_t                    pu_weight;
  Double_t                    xs_weight;
  Double_t                    luminosity;
  PUWeight*                   fPUWeight;


  // Data members
  //----------------------------------------------------------------------------
  std::vector<TLorentzVector> Muons;
  std::vector<TLorentzVector> Electrons;

  std::vector<Double_t>       Muons_Charge;
  std::vector<Double_t>       Electrons_Charge;

  TLorentzVector              Lepton1;
  TLorentzVector              Lepton2;

  Double_t                    Lepton1_Charge;
  Double_t                    Lepton2_Charge;

  Bool_t                      isData;
  UInt_t                      nmuon;
  UInt_t                      nelec;
  UInt_t                      channel;


  ClassDef(AnalysisWW, 0);
};


#endif
