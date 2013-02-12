#ifndef ANALYSIS_WZ_H
#define ANALYSIS_WZ_H 1

#include "packages/CMSAnalysisSelectorMiniTrees/CMSAnalysisSelectorMiniTrees.h"
#include "packages/PUWeight/PUWeight.h"
#include "packages/TProofVector/TProofVector.h"

#include "TH1F.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


const Double_t Z_MASS = 91.1876;  // GeV


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

enum {Tight, Loose};

struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;
  UInt_t         type;
  Double_t       charge;
  TLorentzVector v;

  Bool_t operator<(const Lepton& a) const
  {
    return v.Pt() < a.v.Pt();
  }
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
  void         GetParameters             ();
			    
  Bool_t       ElectronID                (UInt_t iElec);
  Bool_t       ElectronCloseToPV         (UInt_t iElec);
  Bool_t       ElectronBDT               (UInt_t iElec, Bool_t isLoose = false);
  Bool_t       ElectronIsolation         (UInt_t iElec, Bool_t isLoose = false);
			    	      
  Bool_t       MuonID                    (UInt_t iMuon);
  Bool_t       MuonCloseToPV             (UInt_t iMuon, Bool_t isLoose = false);
  Bool_t       MuonIsolation             (UInt_t iMuon, Bool_t isLoose = false);
  
  void         FillChannelCounters       (UInt_t iChannel,
					  UInt_t iCut);

  Bool_t       FillCounters              (UInt_t nElec,
					  UInt_t nMuon,
					  UInt_t iCut);

  void         FillHistograms            (UInt_t iChannel,
  					  UInt_t iCut);

  const Bool_t isSignalMCInsideZmassRange(const float & masslow,
					  const float & masshigh) const;

  const Bool_t WgammaFilter              () const;

  void         GenStudy                  ();


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


  // Gen study
  //----------------------------------------------------------------------------
  TH1D*                       h_n_tau_St3;
  TH1D*                       h_pt_tau_St3;
  TH1D*                       h_eta_tau_St3;

  TH1D*                       h_n_muon_St3;
  TH1D*                       h_n_muon_Gen;
  TH1D*                       h_n_muon_Rec;
  TH1D*                       h_n_muon_Global;
  TH1D*                       h_n_muon_PV;
  TH1D*                       h_n_muon_Iso;
  TH1D*                       h_n_muon_ID;
  TH1D*                       h_n_elec_St3;
  TH1D*                       h_n_elec_Gen;
  TH1D*                       h_n_elec_Rec;
  TH1D*                       h_n_elec_Global;
  TH1D*                       h_n_elec_PV;
  TH1D*                       h_n_elec_Iso;
  TH1D*                       h_n_elec_ID;

  TH1D*                       h_pt_muon_St3;
  TH1D*                       h_pt_muon_Gen;
  TH1D*                       h_pt_muon_Rec;
  TH1D*                       h_pt_muon_Global;
  TH1D*                       h_pt_muon_PV;
  TH1D*                       h_pt_muon_Iso;
  TH1D*                       h_pt_muon_ID;
  TH1D*                       h_pt_elec_St3;
  TH1D*                       h_pt_elec_Gen;
  TH1D*                       h_pt_elec_Rec;
  TH1D*                       h_pt_elec_Global;
  TH1D*                       h_pt_elec_PV;
  TH1D*                       h_pt_elec_Iso;
  TH1D*                       h_pt_elec_ID;

  TH1D*                       h_eta_muon_St3;
  TH1D*                       h_eta_muon_Gen;
  TH1D*                       h_eta_muon_Rec;
  TH1D*                       h_eta_muon_Global;
  TH1D*                       h_eta_muon_PV;
  TH1D*                       h_eta_muon_Iso;
  TH1D*                       h_eta_muon_ID;
  TH1D*                       h_eta_elec_St3;
  TH1D*                       h_eta_elec_Gen;
  TH1D*                       h_eta_elec_Rec;
  TH1D*                       h_eta_elec_Global;
  TH1D*                       h_eta_elec_PV;
  TH1D*                       h_eta_elec_Iso;
  TH1D*                       h_eta_elec_ID;

  TH1D*                       h_mvaRings_muon_r1;
  TH1D*                       h_mvaRings_muon_r2;
  TH1D*                       h_mvaRings_muon_r3;
  TH1D*                       h_mvaRings_muon_r4;


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


  ClassDef(AnalysisWZ, 0);
};


#endif
