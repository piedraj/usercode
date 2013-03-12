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


const Double_t Z_MASS = 91.1876;  // [GeV]


// Input parameters for the WZ cross section
//------------------------------------------------------------------------------
const Double_t xsWplusZ    = 13.89;  // pb (MCFM with 71 < mZ < 111 GeV)
const Double_t xsWminusZ   =  8.06;  // pb (MCFM with 71 < mZ < 111 GeV)
const Double_t W2e         = 0.1075;
const Double_t W2m         = 0.1057;
const Double_t W2tau       = 0.1125;
const Double_t Z2ll        = 0.033658;
const Double_t WZ23lnu     = 3 * Z2ll * (W2e + W2m + W2tau);
const Double_t ngenWZphase = 1449067;  // (71 < mZ < 111 GeV)


const UInt_t nChannels = 4;

enum {MMM, EEE, MME, EEM};

TString sChannel[] = {"MMM", "EEE", "MME", "EEM"};


const UInt_t nCompositions = 5;

enum {FFF, TFF, TTF, TTT, LLL};

TString sComposition[] = {"FFF", "TFF", "TTF", "TTT", "LLL"};


const UInt_t nCuts = 20;

enum {
  Exactly3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET,
  mll20_MET30, mll20_MET35, mll20_MET40, mll20_MET45, mll20_MET50,
  mll15_MET30, mll15_MET35, mll15_MET40, mll15_MET45, mll15_MET50,
  mll10_MET30, mll10_MET35, mll10_MET40, mll10_MET45, mll10_MET50,
  SSLikeAntiBtag
};

TString sCut[] = {
  "Exactly3Leptons",
  "HasZCandidate",
  "HasWCandidate",
  "MET",
  "mll20_MET30", "mll20_MET35", "mll20_MET40", "mll20_MET45", "mll20_MET50",
  "mll15_MET30", "mll15_MET35", "mll15_MET40", "mll15_MET45", "mll15_MET50",
  "mll10_MET30", "mll10_MET35", "mll10_MET40", "mll10_MET45", "mll10_MET50",
  "SSLikeAntiBtag"
};


enum {Muon, Electron};

enum {Tight, Fail};

enum {RAW, PPF, PPP};

const UInt_t nFakeRates = 3;

enum {Jet15, Jet30, Jet50};

struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;  // Muon, Electron
  UInt_t         type;    // Tight, Fail
  Double_t       charge;
  Double_t       sf;
  Double_t       fr[nFakeRates];
  Double_t       pr;
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
			    
  Bool_t       ElectronID                (UInt_t   iElec);
  Bool_t       ElectronCloseToPV         (UInt_t   iElec);
  Bool_t       ElectronBDT               (UInt_t   iElec);
  Bool_t       ElectronIsolation         (UInt_t   iElec);
			    	      
  Bool_t       MuonID                    (UInt_t   iMuon);
  Bool_t       MuonCloseToPV             (UInt_t   iMuon);
  Bool_t       MuonIsolation             (UInt_t   iMuon);
  
  void         FillChannelCounters       (UInt_t   iChannel,
					  UInt_t   iCut,
					  Double_t dd_weight);

  void         FillHistograms            (UInt_t   iChannel,
  					  UInt_t   iCut,
					  Double_t dd_weight);

  const Bool_t WgammaFilter              () const;

  Double_t     GetPPFWeight              (UInt_t   jetPt);
  Double_t     GetPPPWeight              (UInt_t   jetPt);
  Double_t     GetPPFWeightApprx         (UInt_t   jetPt);

  TH2F*        LoadHistogram             (TString  filename,
					  TString  hname,
					  TString  cname);

  void         CounterSummary            (TString  title);


 public:
  
  // Histograms
  //----------------------------------------------------------------------------
  TH1D*                       hCounterRaw[nChannels][nCuts][nCompositions];
  TH1D*                       hCounterPU [nChannels][nCuts][nCompositions];
  TH1D*                       hCounterEff[nChannels][nCuts][nCompositions];
  TH1D*                       hCounter   [nChannels][nCuts][nCompositions];

  TH1D*                       hNPV         [nChannels][nCuts];
  TH1D*                       hMET         [nChannels][nCuts];
  TH1D*                       hSumCharges  [nChannels][nCuts];
  TH1D*                       hInvMass2Lep [nChannels][nCuts];
  TH1D*                       hInvMass3Lep [nChannels][nCuts];
  TH1D*                       hPtLepton1   [nChannels][nCuts];
  TH1D*                       hPtLepton2   [nChannels][nCuts];
  TH1D*                       hPtLepton3   [nChannels][nCuts];
  TH1D*                       hPtLeadingJet[nChannels][nCuts];
  TH1D*                       hDPhiZLeptons[nChannels][nCuts];
  TH1D*                       hPtZLepton1  [nChannels][nCuts];
  TH1D*                       hPtZLepton2  [nChannels][nCuts];
  TH1D*                       hPtWLepton   [nChannels][nCuts];
  TH1D*                       hDRWZLepton1 [nChannels][nCuts];
  TH1D*                       hDRWZLepton2 [nChannels][nCuts];
  TH1D*                       hMtW         [nChannels][nCuts];


  // Input parameters
  //----------------------------------------------------------------------------
  Int_t                       mode;
  Int_t                       closure_test;
  TString                     directory;
  TString                     sample;
  TString                     fileSuffix;
  Double_t                    efficiency_weight;
  Double_t                    pu_weight;
  Double_t                    xs_weight;
  Double_t                    luminosity;
  Double_t                    pu_luminosity;
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
  Double_t                    transverseMass;
  Double_t                    sumCharges;
  Double_t                    ptLeadingJet;
  UInt_t                      nbjets;
  UInt_t                      theChannel;
  UInt_t                      electronCounter;
  UInt_t                      tightCounter;

  Double_t                    dataDriven_weight[nFakeRates];

  ofstream                    outputfile;


  // SF, FR and PR
  //----------------------------------------------------------------------------
  TH2F*                       MuonSF;
  TH2F*                       ElecSF;
  TH2F*                       MuonPR;
  TH2F*                       ElecPR;

  TH2F*                       MuonFR[nFakeRates];
  TH2F*                       ElecFR[nFakeRates];


  ClassDef(AnalysisWZ, 0);
};


#endif
