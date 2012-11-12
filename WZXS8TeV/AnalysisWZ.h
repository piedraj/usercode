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
  void     GetParameters      ();
  Bool_t   IsGenAccepted      ();

  Double_t SelectedMuonPt     (UInt_t iMuon);
  Double_t SelectedElecPt     (UInt_t iElec);

  void     FillHistogramsAtCut(UInt_t iChannel,
			       UInt_t iCut);
				   
  void     GetSelectedMuon    ();
  void     GetSelectedElec    ();


 public:
  
  // Histograms
  //----------------------------------------------------------------------------
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
  Double_t                    weight;
  Double_t                    fWeight;
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
