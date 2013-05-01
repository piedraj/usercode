#ifndef ANALYSIS_WZ_H
#define ANALYSIS_WZ_H 1

#include "packages/CMSAnalysisSelectorMiniTrees/CMSAnalysisSelectorMiniTrees.h"
#include "packages/PUWeight/PUWeight.h"
#include "packages/TProofVector/TProofVector.h"

#include "TFile.h"
#include "TGraphErrors.h"
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


const UInt_t nCut = 9;

enum {
  Exactly3Leptons,
  HasZCandidate,
  MET30_Z20,
  MET30_Z10,
  MET40_Z20,
  MET40_Z10,
  MET40AntiBtag_Z20,
  MET40AntiBtag_Z10,
  ClosureTest_Z10
};

TString sCut[] = {
  "Exactly3Leptons",
  "HasZCandidate",
  "MET30_Z20",
  "MET30_Z10",
  "MET40_Z20",
  "MET40_Z10",
  "MET40AntiBtag_Z20",
  "MET40AntiBtag_Z10",
  "ClosureTest_Z10"
};


enum {Muon, Electron};

enum {Tight, Fail};

enum {RAW, PPF, PPP};


const UInt_t nFakeRateMuon = 4;
const UInt_t nFakeRateElec = 3;

enum {MuonJet15, MuonJet20, MuonJet30, MuonJet50};
enum {ElecJet15, ElecJet35, ElecJet50};

struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;  // Muon, Electron
  UInt_t         type;    // Tight, Fail
  Double_t       charge;
  Double_t       sf;
  Double_t       frMuon[nFakeRateMuon];
  Double_t       frElec[nFakeRateElec];
  Double_t       pr;
  Double_t       lead;
  Double_t       trail;
  TLorentzVector vraw;
  TLorentzVector v;

  Bool_t operator<(const Lepton& a) const
  {
    return v.Pt() < a.v.Pt();
  }
};


enum {
  noSyst,
  metSyst,
  muonUpSyst,
  muonDownSyst,
  electronUpSyst,
  electronDownSyst
};


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
  Bool_t         ElectronBDT               (UInt_t   iElec);
  Bool_t         ElectronIsolation         (UInt_t   iElec);
			    	      
  Bool_t         MuonID                    (UInt_t   iMuon);
  Bool_t         MuonIsolation             (UInt_t   iMuon);
  
  void           FillHistograms            (UInt_t   iChannel,
					    UInt_t   iCut,
					    Double_t dd_weight);

  const Bool_t   WgammaFilter              () const;

  Double_t       GetPPFWeight              (UInt_t   muonJetPt,
					    UInt_t   elecJetPt);

  Double_t       GetPPPWeight              (UInt_t   muonJetPt,
					    UInt_t   elecJetPt);

  TH2F*          LoadHistogram             (TString  filename,
					    TString  hname,
					    TString  cname);

  void           CounterSummary            (TString  title);

  TLorentzVector GetMET                    ();

  Double_t       GetTriggerWeight          ();

  Bool_t         PassTrigger               ();

  Double_t       ScaleLepton               (UInt_t   flavor,
					    Double_t pt,
					    Double_t eta = -999);

  TH1F*          GetHistogramFromGraph     (TString  hname,
					    TString  gname);


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
  TH1D*                       hNJet30      [nChannel][nCut];
  TH1D*                       hNBJet30     [nChannel][nCut];

  TH1D*                       hInvMass2Lep_EE;
  TH1D*                       hInvMass2Lep_EE_BarrelBarrel;
  TH1D*                       hInvMass2Lep_EE_BarrelEndcap;
  TH1D*                       hInvMass2Lep_EE_EndcapEndcap;

  TH1D*                       hInvMass2Lep_MM;
  TH1D*                       hInvMass2Lep_MM_BarrelBarrel;
  TH1D*                       hInvMass2Lep_MM_BarrelEndcap;
  TH1D*                       hInvMass2Lep_MM_EndcapEndcap;


  // Input parameters
  //----------------------------------------------------------------------------
  TString                     directory;
  TString                     sample;
  Double_t                    luminosity;
  Double_t                    pu_luminosity;
  Double_t                    xs_weight;
  Int_t                       mode;
  Int_t                       runAtOviedo;
  Int_t                       systematic;


  // Data members
  //----------------------------------------------------------------------------
  std::vector<Lepton>         AnalysisLeptons;

  TLorentzVector              EventMET;
  TLorentzVector              WLepton;
  TLorentzVector              ZLepton1;
  TLorentzVector              ZLepton2;

  Bool_t                      isData;

  Double_t                    deltaZMass;
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

  UInt_t                      nJet30;
  UInt_t                      nBJet30;
  UInt_t                      nElectron;
  UInt_t                      nTight;
  UInt_t                      theChannel;
  UInt_t                      theTrigger;

  ofstream                    outputfile;

  PUWeight*                   fPUWeight;


  // SF, FR, PR and trigger efficiencies
  //----------------------------------------------------------------------------
  Double_t                    ddweight[nFakeRateMuon][nFakeRateElec];

  TH2F*                       MuonFR[nFakeRateMuon];
  TH2F*                       ElecFR[nFakeRateElec];
  TH2F*                       MuonSF;
  TH2F*                       ElecSF;
  TH2F*                       MuonPR;
  TH2F*                       ElecPR;
  TH2F*                       DoubleElLead;
  TH2F*                       DoubleMuLead;
  TH2F*                       DoubleElTrail;
  TH2F*                       DoubleMuTrail;


  // Electron energy scale systematics
  //----------------------------------------------------------------------------
  TH1F*                       hScaleInEB;
  TH1F*                       hScaleOutEB;
  TH1F*                       hScaleEE;


  ClassDef(AnalysisWZ, 0);
};


#endif
