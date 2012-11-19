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
  Double_t                    total_weight;
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

  UInt_t                      nmuon;
  UInt_t                      nelec;
  UInt_t                      channel;


  ClassDef(AnalysisWW, 0);
};


#endif
