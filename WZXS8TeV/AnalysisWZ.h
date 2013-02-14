#ifndef ANALYSIS_WZ_H
#define ANALYSIS_WZ_H 1

#include "packages/CMSAnalysisSelectorMiniTrees/CMSAnalysisSelectorMiniTrees.h"
#include "packages/PUWeight/PUWeight.h"
#include "packages/TProofVector/TProofVector.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


const Double_t Z_MASS = 91.1876;  // GeV


const UInt_t nChannels = 4;

enum {MMM, EEE, MME, EEM};

TString sChannel[] = {"MMM", "EEE", "MME", "EEM"};


const UInt_t nCuts = 10;

enum {
  AllEvents,
  HLT,
  Has2Leptons,
  Has2PVLeptons,
  Has2IsoLeptons,
  Has2IsoGoodLeptons,
  AtLeast3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET
};

TString sCut[] = {
  "AllEvents",
  "HLT",
  "Has2Leptons",
  "Has2PVLeptons",
  "Has2IsoLeptons",
  "Has2IsoGoodLeptons",
  "AtLeast3Leptons",
  "HasZCandidate",
  "HasWCandidate",
  "MET"
};


enum {Muon, Electron};

enum {Rejected, Tight, Loose};

struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;
  UInt_t         type;
  Double_t       charge;
  Double_t       SF;
  Double_t       f;  // fake rate
  Double_t       p;  // prompt rate
  TLorentzVector v;

  Bool_t operator<(const Lepton& a) const
  {
    return v.Pt() < a.v.Pt();
  }
};


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
  void         GetParameters             ();
			    
  Bool_t       ElectronID                (UInt_t iElec);
  Bool_t       ElectronCloseToPV         (UInt_t iElec);
  UInt_t       ElectronBDT               (UInt_t iElec);
  UInt_t       ElectronIsolation         (UInt_t iElec);
			    	      
  Bool_t       MuonID                    (UInt_t iMuon);
  UInt_t       MuonCloseToPV             (UInt_t iMuon);
  UInt_t       MuonIsolation             (UInt_t iMuon);
  
  void         FillChannelCounters       (UInt_t iChannel,
					  UInt_t iCut);

  Bool_t       FillCounters              (UInt_t nElec,
					  UInt_t nMuon,
					  UInt_t iCut);

  void         FillHistograms            (UInt_t iChannel,
  					  UInt_t iCut);

  const Bool_t WgammaFilter              () const;


 public:
  
  // Histograms
  //----------------------------------------------------------------------------
  TH1D*                       hCounterRaw[nChannels][nCuts];
  TH1D*                       hCounterPU [nChannels][nCuts];
  TH1D*                       hCounterEff[nChannels][nCuts];
  TH1D*                       hCounter   [nChannels][nCuts];

  TH1D*                       hNPV        [nChannels][nCuts];
  TH1D*                       hMET        [nChannels][nCuts];
  TH1D*                       hSumCharges [nChannels][nCuts];
  TH1D*                       hInvMass2Lep[nChannels][nCuts];
  TH1D*                       hInvMass3Lep[nChannels][nCuts];
  TH1D*                       hPtLepton1  [nChannels][nCuts];
  TH1D*                       hPtLepton2  [nChannels][nCuts];
  TH1D*                       hPtLepton3  [nChannels][nCuts];
  TH1D*                       hPtZLepton1 [nChannels][nCuts];
  TH1D*                       hPtZLepton2 [nChannels][nCuts];
  TH1D*                       hPtWLepton  [nChannels][nCuts];


  // Input parameters
  //----------------------------------------------------------------------------
  Bool_t                      useFakes;
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
  std::vector<Lepton>         AnalysisLeptons;

  TLorentzVector              ZLepton1;
  TLorentzVector              ZLepton2;
  TLorentzVector              WLepton;

  Bool_t                      isData;
  Double_t                    invMass2Lep;
  Double_t                    invMass3Lep;
  Double_t                    sumCharges;
  UInt_t                      theChannel;


  // SF, FR and PR
  //----------------------------------------------------------------------------
  TFile* MuonSF_file;
  TFile* MuonFR_file;
  TFile* MuonPR_file;
  TFile* ElecSF_file;
  TFile* ElecFR_file;
  TFile* ElecPR_file;

  TH2F*  MuonSF_hist;
  TH2F*  MuonFR_hist;
  TH2F*  MuonPR_hist;
  TH2F*  ElecSF_hist;
  TH2F*  ElecFR_hist;
  TH2F*  ElecPR_hist;


  ClassDef(AnalysisWZ, 0);
};


#endif
