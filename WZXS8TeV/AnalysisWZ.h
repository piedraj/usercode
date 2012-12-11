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


// Lepton (top) scale factors
//------------------------------------------------------------------------------
const Double_t SF_Trigger_MM = 0.965;  // +- 0.0102;
const Double_t SF_Trigger_EE = 0.962;  // +- 0.0130;
const Double_t SF_Trigger_ME = 0.943;  // +- 0.0120;

const Double_t SF_Global_M = 0.9985;  // +- 0.0006;
const Double_t SF_Global_E = 0.9850;  // +- 0.0003;

const Double_t SF_Global_MMM = SF_Global_M * SF_Global_M * SF_Global_M;
const Double_t SF_Global_EEE = SF_Global_E * SF_Global_E * SF_Global_E;
const Double_t SF_Global_MME = SF_Global_M * SF_Global_M * SF_Global_E;
const Double_t SF_Global_EEM = SF_Global_E * SF_Global_E * SF_Global_M;


const Double_t Z_MASS = 91.1876;  // GeV

const UInt_t nChannels = 4;

enum {MMM, EEE, MME, EEM};

TString sChannel[] = {"MMM", "EEE", "MME", "EEM"};


const UInt_t nCuts = 7;

enum {
  AllEvents,
  HLT,
  Has2IsoGoodLeptons,
  Exactly3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET
};

TString sCut[] = {
  "AllEvents",
  "HLT",
  "Has2IsoGoodLeptons",
  "Exactly3Leptons",
  "HasZCandidate",
  "HasWCandidate",
  "MET"
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

  Bool_t   ElectronBDT         (UInt_t   iElec);
  Bool_t   ElectronID          (UInt_t   iElec);
  Bool_t   ElectronCloseToPV   (UInt_t   iElec);
  Bool_t   ElectronIsolation   (UInt_t   iElec);

  Bool_t   MuonID              (UInt_t   iMuon);
  Bool_t   MuonCloseToPV       (UInt_t   iMuon);
  Bool_t   MuonIsolation       (UInt_t   iMuon);

  Double_t SelectedElectronPt  (UInt_t   iElec);
  Double_t SelectedMuonPt      (UInt_t   iMuon);

  void     GetSelectedElectrons(Double_t ptMin, Double_t etaMax);
  void     GetSelectedMuons    (Double_t ptMin, Double_t etaMax);

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
  TString                     directory;
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

  Bool_t                      isData;
  Double_t                    dileptonInvMass;
  UInt_t                      nSelMuon;
  UInt_t                      nSelElec;
  UInt_t                      theChannel;


  ClassDef(AnalysisWZ, 0);
};


#endif
