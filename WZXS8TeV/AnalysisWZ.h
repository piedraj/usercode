#ifndef ANALYSIS_WZ_H
#define ANALYSIS_WZ_H 1

#include "packages/CMSAnalysisSelectorMiniTrees/CMSAnalysisSelectorMiniTrees.h"
#include "packages/PUWeight/PUWeight.h"
#include "packages/TProofVector/TProofVector.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TVector3.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


const Double_t Z_MASS = 91.1876;  // [GeV]


const UInt_t nChannel = 4;

enum {MMM, EEE, MME, EEM};

TString sChannel[] = {"MMM", "EEE", "MME", "EEM"};


const UInt_t nComposition = 5;

enum {FFF, TFF, TTF, TTT, LLL};

TString sComposition[] = {"FFF", "TFF", "TTF", "TTT", "LLL"};


const UInt_t nCut = 7;

enum {
  Exactly3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET30,
  MET40,
  MET40AntiBtag,
  ATLAS
};

TString sCut[] = {
  "Exactly3Leptons",
  "HasZCandidate",
  "HasWCandidate",
  "MET30",
  "MET40",
  "MET40AntiBtag",
  "ATLAS"
};


enum {Muon, Electron};

enum {Tight, Fail};

enum {RAW, PPF, PPP};

const UInt_t nFakeRate = 3;

enum {Jet15, Jet30, Jet50};

struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;  // Muon, Electron
  UInt_t         type;    // Tight, Fail
  Double_t       charge;
  Double_t       sf;
  Double_t       fr[nFakeRate];
  Double_t       pr;
  Double_t       lead;
  Double_t       trail;
  TLorentzVector v;

  Bool_t operator<(const Lepton& a) const
  {
    return v.Pt() < a.v.Pt();
  }
};


enum {noSyst, metSyst};


//------------------------------------------------------------------------------
//
// AnalysisWZ
//
//------------------------------------------------------------------------------
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


  // Member functions
  //----------------------------------------------------------------------------
  void           GetParameters             ();
			    
  Bool_t         ElectronID                (UInt_t   iElec);
  Bool_t         ElectronCloseToPV         (UInt_t   iElec);
  Bool_t         ElectronBDT               (UInt_t   iElec);
  Bool_t         ElectronIsolation         (UInt_t   iElec);
			    	      
  Bool_t         MuonID                    (UInt_t   iMuon);
  Bool_t         MuonCloseToPV             (UInt_t   iMuon);
  Bool_t         MuonIsolation             (UInt_t   iMuon);
  
  void           FillHistograms            (UInt_t   iChannel,
					    UInt_t   iCut,
					    Double_t dd_weight);

  const Bool_t   WgammaFilter              () const;

  Double_t       GetPPFWeight              (UInt_t   jetPt);
  Double_t       GetPPPWeight              (UInt_t   jetPt);

  TH2F*          LoadHistogram             (TString  filename,
					    TString  hname,
					    TString  cname);

  void           CounterSummary            (TString  title);

  TLorentzVector GetMET                    ();

  Double_t       GetTriggerWeight          ();

  Bool_t         PassTrigger               ();


 public:
  
  // Histograms
  //----------------------------------------------------------------------------
  TH1D*                       hCounterRaw[nChannel][nCut][nComposition];
  TH1D*                       hCounterPU [nChannel][nCut][nComposition];
  TH1D*                       hCounterEff[nChannel][nCut][nComposition];
  TH1D*                       hCounter   [nChannel][nCut][nComposition];

  TH1D*                       hLeptonWeight [nChannel][nCut];
  TH1D*                       hTriggerWeight[nChannel][nCut];
  TH1D*                       hTotalWeight  [nChannel][nCut];

  TH1D*                       hNPV         [nChannel][nCut];
  TH1D*                       hMET         [nChannel][nCut];
  TH1D*                       hSumCharges  [nChannel][nCut];
  TH1D*                       hInvMass2Lep [nChannel][nCut];
  TH1D*                       hInvMass3Lep [nChannel][nCut];
  TH1D*                       hPtLepton1   [nChannel][nCut];
  TH1D*                       hPtLepton2   [nChannel][nCut];
  TH1D*                       hPtLepton3   [nChannel][nCut];
  TH1D*                       hPtLeadingJet[nChannel][nCut];
  TH1D*                       hDPhiZLeptons[nChannel][nCut];
  TH1D*                       hPtZLepton1  [nChannel][nCut];
  TH1D*                       hPtZLepton2  [nChannel][nCut];
  TH1D*                       hPtWLepton   [nChannel][nCut];
  TH1D*                       hDRWZLepton1 [nChannel][nCut];
  TH1D*                       hDRWZLepton2 [nChannel][nCut];
  TH1D*                       hMtW         [nChannel][nCut];

  TH1D*                       hDRWZLepton1Zoom[nChannel][nCut];
  TH1D*                       hDRWZLepton2Zoom[nChannel][nCut];


  // Input parameters
  //----------------------------------------------------------------------------
  TString                     directory;
  TString                     sample;
  Double_t                    luminosity;
  Double_t                    pu_luminosity;
  Double_t                    xs_weight;
  Int_t                       mode;
  Int_t                       systematic;
  Int_t                       closure_test;
  Int_t                       runAtOviedo;


  // Data members
  //----------------------------------------------------------------------------
  std::vector<Lepton>         AnalysisLeptons;

  TLorentzVector              EventMET;
  TLorentzVector              WLepton;
  TLorentzVector              ZLepton1;
  TLorentzVector              ZLepton2;

  Bool_t                      isData;

  Double_t                    invMass2Lep;
  Double_t                    invMass3Lep;
  Double_t                    sumCharges;
  Double_t                    transverseMass;
  Double_t                    ptLeadingJet;
  Double_t                    efficiency_weight;
  Double_t                    pu_weight;
  Double_t                    mc_lepton_weight;
  Double_t                    mc_trigger_weight;
  Double_t                    mc_total_weight;

  UInt_t                      nBJet;
  UInt_t                      nElectron;
  UInt_t                      nTight;
  UInt_t                      theChannel;
  UInt_t                      theTrigger;

  ofstream                    outputfile;

  PUWeight*                   fPUWeight;


  // SF, FR, PR and trigger efficiencies
  //----------------------------------------------------------------------------
  Double_t                    dataDriven_weight[nFakeRate];

  TH2F*                       MuonFR[nFakeRate];
  TH2F*                       ElecFR[nFakeRate];
  TH2F*                       MuonSF;
  TH2F*                       ElecSF;
  TH2F*                       MuonPR;
  TH2F*                       ElecPR;
  TH2F*                       DoubleElLead;
  TH2F*                       DoubleMuLead;
  TH2F*                       DoubleElTrail;
  TH2F*                       DoubleMuTrail;


  ClassDef(AnalysisWZ, 0);
};


#endif
