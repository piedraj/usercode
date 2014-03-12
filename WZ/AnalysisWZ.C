#include "TChain.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TTree.h"
#include "TVector3.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>


//==============================================================================
//
// Constants, enums and structs
//
//==============================================================================
const Float_t luminosity = 19.602;  // [fb-1]


const UInt_t number_of_leptons = 4;
const UInt_t number_of_jets    = 7;


const Float_t METELECPTSYS = 0.025;
const Float_t METMUONPTSYS = 0.015;
const Float_t METJETSPTSYS = 0.05;


const Float_t E_MASS =  0.000511;  // [GeV]
const Float_t M_MASS =  0.106;     // [GeV]
const Float_t Z_MASS = 91.1876;    // [GeV]


const UInt_t nChannel = 5;

enum {
  EEE,
  EEM,
  MME,
  MMM,
  combined
};

const TString sChannel[nChannel] = {
  "EEE",
  "EEM",
  "MME",
  "MMM",
  "combined"
};


const UInt_t nComposition = 5;

enum {
  FFF,
  TFF,
  TTF,
  TTT,
  LLL};

const TString sComposition[nComposition] = {
  "FFF",
  "TFF",
  "TTF",
  "TTT",
  "LLL"
};


const UInt_t nCut = 14;

enum {
  Exactly3Leptons,
  InvMass3Lep100,
  HasZ,
  HasW,
  MET30,
  MET30Pt20,
  MET30Btag,
  MET30Pt20Btag,
  MET40,
  SantiagoCuts,
  ZJetsRegion,
  TopRegion,
  VBFSelection,
  EXO_12_025
};

const TString sCut[nCut] = {
  "Exactly3Leptons",
  "InvMass3Lep100",
  "HasZ",
  "HasW",
  "MET30",
  "MET30Pt20",
  "MET30Btag",
  "MET30Pt20Btag",
  "MET40",
  "SantiagoCuts",
  "ZJetsRegion",
  "TopRegion",
  "VBFSelection",
  "EXO_12_025"
};


const UInt_t nCharge = 3;

enum {
  WPlus,
  WMinus,
  WInclusive
};

const TString sCharge[nCharge] = {
  "WPlus",
  "WMinus",
  "WInclusive"
};


enum {Muon, Electron};

enum {Fail, Tight};

enum {RAW, PPF, ATLAS};


struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;  // Muon, Electron
  UInt_t         type;    // Tight, Fail
  Float_t        charge;
  Float_t        sf;
  Float_t        pr;
  Float_t        fr;
  Float_t        lead;
  Float_t        trail;
  TLorentzVector vraw;
  TLorentzVector v;

  Bool_t operator<(const Lepton& a) const
  {
    return v.Pt() < a.v.Pt();
  }
};


enum {
  noSyst,
  metUpSyst,
  metDownSyst,
  muonUpSyst,
  muonDownSyst,
  electronUpSyst,
  electronDownSyst,
  pileupSyst,
  fakesSyst
};


//==============================================================================
//
// Member functions
//
//==============================================================================
void           FillHistograms       (UInt_t  iChannel,
				     UInt_t  iCut);

Float_t        GetTriggerWeight     ();

TH2F*          LoadHistogram        (TString filename,
				     TString hname,
				     TString cname);

TH1F*          GetHistogramFromGraph(TString hname,
				     TString gname);

Float_t        GetFactor            (TH2F*   h2,
				     Float_t leptonPt,
				     Float_t leptonEta,
				     Float_t leptonPtMax = -999.);

Float_t        GetDataDrivenWeight  (UInt_t  nPrompt);

TLorentzVector GetMET               (Float_t metModule,
				     Float_t metPhi);

Float_t        ScaleLepton          (UInt_t  flavor,
				     Float_t leptonPt,
				     Float_t leptonEta);

void           CounterSummary       (TString title);


//==============================================================================
//
// Data members
//
//==============================================================================
TH1F*                         h_gen_mZ_denominator;
TH1F*                         h_gen_mZ_numerator;

TH1F*                         hCounterRaw   [nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounterEff   [nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounter      [nChannel][nCut][nCharge][nComposition];
TH1F*                         hPromptCounter[nChannel][nCut][nCharge][nComposition];

TH1F*                         hLeptonWeight [nChannel][nCut][nCharge];
TH1F*                         hTriggerWeight[nChannel][nCut][nCharge];
TH1F*                         hTotalWeight  [nChannel][nCut][nCharge];

TH1F*                         hNPV           [nChannel][nCut][nCharge];
TH1F*                         hMET           [nChannel][nCut][nCharge];
TH1F*                         h_chmet        [nChannel][nCut][nCharge];
TH1F*                         hSumCharges    [nChannel][nCut][nCharge];
TH1F*                         hInvMass2Lep   [nChannel][nCut][nCharge];
TH1F*                         hInvMass3Lep   [nChannel][nCut][nCharge];
TH1F*                         hPtLepton1     [nChannel][nCut][nCharge];
TH1F*                         hPtLepton2     [nChannel][nCut][nCharge];
TH1F*                         hPtLepton3     [nChannel][nCut][nCharge];
TH1F*                         hDPhiZLeptons  [nChannel][nCut][nCharge];
TH1F*                         hDPhiWLeptonMET[nChannel][nCut][nCharge];
TH1F*                         hPtZLepton1    [nChannel][nCut][nCharge];
TH1F*                         hPtZLepton2    [nChannel][nCut][nCharge];
TH1F*                         hPtZ           [nChannel][nCut][nCharge];
TH1F*                         hPtWLepton     [nChannel][nCut][nCharge];
TH1F*                         hPtW           [nChannel][nCut][nCharge];
TH1F*                         hDRWZLepton1   [nChannel][nCut][nCharge];
TH1F*                         hDRWZLepton2   [nChannel][nCut][nCharge];
TH1F*                         hMtW           [nChannel][nCut][nCharge];
TH1F*                         hMinDeltaR2Lep [nChannel][nCut][nCharge];
TH1F*                         hMinInvMass2Lep[nChannel][nCut][nCharge];
TH1F*                         hNJetAbove30   [nChannel][nCut][nCharge];
TH1F*                         hNJetBelow30   [nChannel][nCut][nCharge];
TH1F*                         hNBJetAbove30  [nChannel][nCut][nCharge];

TH2F*                         hPtLepton3_invMass2Lep[nChannel][nCut][nCharge];
TH2F*                         hPtWLepton_invMass2Lep[nChannel][nCut][nCharge];

TH1F*                         hPtLeadingJet   [nChannel][nCut][nCharge];
TH1F*                         hPtSecondJet    [nChannel][nCut][nCharge];
TH1F*                         hEtaLeadingJet  [nChannel][nCut][nCharge];
TH1F*                         hEtaSecondJet   [nChannel][nCut][nCharge];
TH1F*                         hPhiLeadingJet  [nChannel][nCut][nCharge];
TH1F*                         hPhiSecondJet   [nChannel][nCut][nCharge];
TH1F*                         hDRLeadingJetLep[nChannel][nCut][nCharge];
TH1F*                         hDEtaJets       [nChannel][nCut][nCharge];
TH1F*                         hInvMass2Jet    [nChannel][nCut][nCharge];

TH1F*                         hInvMass2LepBB[2];
TH1F*                         hInvMass2LepBE[2];
TH1F*                         hInvMass2LepEE[2];

std::vector<Lepton>           AnalysisLeptons;
std::vector<TLorentzVector>   SelectedJets;
std::vector<TLorentzVector>   LowPtJets;

Lepton                        WLepton;
Lepton                        ZLepton1;
Lepton                        ZLepton2;

TLorentzVector                EventMET;
TLorentzVector                TrackMET;

Bool_t                        isData;

Float_t                       invMass2Lep;
Float_t                       invMass3Lep;
Float_t                       sumCharges;
Float_t                       transverseMass;

Int_t                         nJetAbove30;
Int_t                         nJetBelow30;
Int_t                         nBJetAbove30;

UInt_t                        nElectron;
UInt_t                        nTight;
UInt_t                        reco_channel;

TFile*                        root_output;
ofstream                      txt_output;
ofstream                      txt_data_events;
ofstream                      txt_signal_events;


// Input parameters
//------------------------------------------------------------------------------
TString                       _sample;
Int_t                         _systematic;
Int_t                         _mode;
Int_t                         _jetChannel;
TString                       _directory;


// Weights;
//------------------------------------------------------------------------------
Float_t                       efficiency_weight;
Float_t                       dd_weight;
Float_t                       mc_lepton_weight;
Float_t                       mc_trigger_weight;
Float_t                       mc_total_weight;
Float_t                       pu_weight;
Float_t                       xs_weight;


// SF, FR, PR and trigger efficiencies histograms
//------------------------------------------------------------------------------
TH2F*                         MuonFR;
TH2F*                         ElecFR;
TH2F*                         MuonSF;
TH2F*                         ElecSF;
TH2F*                         MuonPR;
TH2F*                         ElecPR;
TH2F*                         DoubleElLead;
TH2F*                         DoubleMuLead;
TH2F*                         DoubleElTrail;
TH2F*                         DoubleMuTrail;


// Electron energy scale systematics
//------------------------------------------------------------------------------
TH1F*                         hScaleInEB;
TH1F*                         hScaleOutEB;
TH1F*                         hScaleEE;


// Tree leaves
//------------------------------------------------------------------------------
Long64_t                      run;
Long64_t                      event;
Long64_t                      lumi;
Float_t                       baseW;
Float_t                       channel;
Float_t                       chmet;
Float_t                       chmetphi;
Float_t                       dataset;
Float_t                       fakeW;
Float_t                       njet;
Float_t                       nextra;
Float_t                       nvtx;
Float_t                       pfmet;
Float_t                       pfmetphi;
Float_t                       puW;
Float_t                       trigger;
Float_t                       bdt          [number_of_leptons];
Float_t                       ch           [number_of_leptons];
Float_t                       eta          [number_of_leptons];
Float_t                       ip           [number_of_leptons];
Float_t                       iso          [number_of_leptons];
Float_t                       isomva       [number_of_leptons];
Int_t                         pass2012ICHEP[number_of_leptons];
Float_t                       phi          [number_of_leptons];
Float_t                       pt           [number_of_leptons];
Float_t                       jeteta       [number_of_jets];
Float_t                       jetphi       [number_of_jets];
Float_t                       jetpt        [number_of_jets];
Float_t                       jettche      [number_of_jets];
Float_t                       jetid        [number_of_jets];


// Create aTGC tree
//------------------------------------------------------------------------------
TTree*                        tgcTree;

Float_t                       gen_channel;
Float_t                       gen_mZ;
Float_t                       gen_ptZ;
Float_t                       reco_mZ;
Float_t                       reco_ptZ;


//==============================================================================
//
// AnalysisWZ
//
//==============================================================================
void AnalysisWZ(TString sample,
		Int_t   systematic,
		Int_t   mode,
		Int_t   jetChannel,
		TString muonJetPt,
		TString elecJetPt,
		TString directory)
{
  TH1::SetDefaultSumw2();

  _sample     = sample;
  _systematic = systematic;
  _mode       = mode;
  _jetChannel = jetChannel;
  _directory  = directory;

  if (_jetChannel >= 0) _directory = Form("%s/%djet", _directory.Data(), _jetChannel);

  if      (_systematic == noSyst)           _directory += "/analysis";
  else if (_systematic == metUpSyst)        _directory += "/systematics/metUp";
  else if (_systematic == metDownSyst)      _directory += "/systematics/metDown";
  else if (_systematic == muonUpSyst)       _directory += "/systematics/muonUp";
  else if (_systematic == muonDownSyst)     _directory += "/systematics/muonDown";
  else if (_systematic == electronUpSyst)   _directory += "/systematics/electronUp";
  else if (_systematic == electronDownSyst) _directory += "/systematics/electronDown";
  else if (_systematic == pileupSyst)       _directory += "/systematics/pileup";
  else if (_systematic == fakesSyst)        _directory += Form("/systematics/muonJet%s_elecJet%s",
							       muonJetPt.Data(),
							       elecJetPt.Data());

  if (_mode == ATLAS)
    {
      _directory = directory;

      if (_jetChannel >= 0) _directory += Form("/%djet", _jetChannel);

      _directory += "/atlas";
    }

  gSystem->mkdir(_directory, kTRUE);

  TString filename = _sample;

  if (_mode == PPF) filename += "_PPF";

  root_output = new TFile(_directory + "/" + filename + ".root", "recreate");


  // Set aTGC tree branches and generation histograms
  //----------------------------------------------------------------------------
  if (_sample.Contains("GenVars"))
    {
      tgcTree = new TTree("tgcTree", "tgcTree");

      tgcTree->Branch("gen_channel",       &gen_channel);
      tgcTree->Branch("gen_mZ",            &gen_mZ);
      tgcTree->Branch("gen_ptZ",           &gen_ptZ);
      tgcTree->Branch("reco_channel",      &reco_channel);
      tgcTree->Branch("reco_mZ",           &reco_mZ);
      tgcTree->Branch("reco_ptZ",          &reco_ptZ);
      tgcTree->Branch("efficiency_weight", &efficiency_weight);
      tgcTree->Branch("pu_weight",         &pu_weight);
      tgcTree->Branch("xs_weight",         &xs_weight);

      h_gen_mZ_denominator = new TH1F("h_gen_mZ_denominator", "", 200, 0, 200);
      h_gen_mZ_numerator   = new TH1F("h_gen_mZ_numerator",   "", 200, 0, 200);
    }
  else
    {
      tgcTree              = NULL;
      h_gen_mZ_denominator = NULL;
      h_gen_mZ_numerator   = NULL;
    }


  // Histogram definition
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {

    for (UInt_t j=0; j<nCut; j++) {

      for (UInt_t iCharge=0; iCharge<nCharge; iCharge++) {

	TString suffix = "_" + sChannel[i] + "_" + sCut[j] + "_" + sCharge[iCharge];
	
	for (UInt_t k=0; k<nComposition; k++) {

	  hCounterRaw[i][j][iCharge][k] = new TH1F("hCounterRaw" + suffix + "_" + sComposition[k], "", 3, 0, 3);
	  hCounterEff[i][j][iCharge][k] = new TH1F("hCounterEff" + suffix + "_" + sComposition[k], "", 3, 0, 3);
	  hCounter   [i][j][iCharge][k] = new TH1F("hCounter"    + suffix + "_" + sComposition[k], "", 3, 0, 3);

	  hPromptCounter[i][j][iCharge][k] = new TH1F(Form("hPromptCounter%s_%dprompt", suffix.Data(), k), "", 3, 0, 3);
	}

	hLeptonWeight [i][j][iCharge] = new TH1F("hLeptonWeight"  + suffix, "", 90, 0.75, 1.05);
	hTriggerWeight[i][j][iCharge] = new TH1F("hTriggerWeight" + suffix, "", 90, 0.75, 1.05);
	hTotalWeight  [i][j][iCharge] = new TH1F("hTotalWeight"   + suffix, "", 90, 0.75, 1.05);

	hNPV           [i][j][iCharge] = new TH1F("hNPV"            + suffix, "",  50,   0,    50);
	hMET           [i][j][iCharge] = new TH1F("hMET"            + suffix, "", 200,   0,   200);
	h_chmet        [i][j][iCharge] = new TH1F("h_chmet"         + suffix, "", 200,   0,   200);
	hSumCharges    [i][j][iCharge] = new TH1F("hSumCharges"     + suffix, "",   9,  -4,     5);
	hInvMass2Lep   [i][j][iCharge] = new TH1F("hInvMass2Lep"    + suffix, "", 400,   0,   200);
	hInvMass3Lep   [i][j][iCharge] = new TH1F("hInvMass3Lep"    + suffix, "", 400,   0,   400);
	hPtLepton1     [i][j][iCharge] = new TH1F("hPtLepton1"      + suffix, "", 200,   0,   200);
	hPtLepton2     [i][j][iCharge] = new TH1F("hPtLepton2"      + suffix, "", 200,   0,   200);
	hPtLepton3     [i][j][iCharge] = new TH1F("hPtLepton3"      + suffix, "", 200,   0,   200);    
	hDPhiZLeptons  [i][j][iCharge] = new TH1F("hDPhiZLeptons"   + suffix, "", 320,   0,     3.2);    
	hDPhiWLeptonMET[i][j][iCharge] = new TH1F("hDPhiWLeptonMET" + suffix, "", 320,   0,     3.2);    
	hPtZLepton1    [i][j][iCharge] = new TH1F("hPtZLepton1"     + suffix, "", 200,   0,   200);
	hPtZLepton2    [i][j][iCharge] = new TH1F("hPtZLepton2"     + suffix, "", 200,   0,   200);
	hPtZ           [i][j][iCharge] = new TH1F("hPtZ"            + suffix, "", 400,   0,   400);
	hPtWLepton     [i][j][iCharge] = new TH1F("hPtWLepton"      + suffix, "", 200,   0,   200);    
	hPtW           [i][j][iCharge] = new TH1F("hPtW"            + suffix, "", 400,   0,   400);
	hDRWZLepton1   [i][j][iCharge] = new TH1F("hDRWZLepton1"    + suffix, "", 300,   0,     6);    
	hDRWZLepton2   [i][j][iCharge] = new TH1F("hDRWZLepton2"    + suffix, "", 300,   0,     6);    
	hMtW           [i][j][iCharge] = new TH1F("hMtW"            + suffix, "", 200,   0,   200);    
	hMinDeltaR2Lep [i][j][iCharge] = new TH1F("hMinDeltaR2Lep"  + suffix, "", 300,   0,     6);    
	hMinInvMass2Lep[i][j][iCharge] = new TH1F("hMinInvMass2Lep" + suffix, "", 400,   0,   200);    
	hNJetAbove30   [i][j][iCharge] = new TH1F("hNJetAbove30"    + suffix, "",  10,   0,    10);    
	hNJetBelow30   [i][j][iCharge] = new TH1F("hNJetBelow30"    + suffix, "",  10,   0,    10);    
	hNBJetAbove30  [i][j][iCharge] = new TH1F("hNBJetAbove30"   + suffix, "",  10,   0,    10);

	hPtLepton3_invMass2Lep[i][j][iCharge] = new TH2F("hPtLepton3_invMass2Lep" + suffix, "", 120, 0, 120, 120, 0, 120);    
	hPtWLepton_invMass2Lep[i][j][iCharge] = new TH2F("hPtWLepton_invMass2Lep" + suffix, "", 120, 0, 120, 120, 0, 120);    


	// Jet histograms
	//----------------------------------------------------------------------
	hPtLeadingJet   [i][j][iCharge] = new TH1F("hPtLeadingJet"    + suffix, "", 200,  0,    200);
	hPtSecondJet    [i][j][iCharge] = new TH1F("hPtSecondJet"     + suffix, "", 200,  0,    200);
	hEtaLeadingJet  [i][j][iCharge] = new TH1F("hEtaLeadingJet"   + suffix, "", 240, -6,      6);
	hEtaSecondJet   [i][j][iCharge] = new TH1F("hEtaSecondJet"    + suffix, "", 240, -6,      6);
	hPhiLeadingJet  [i][j][iCharge] = new TH1F("hPhiLeadingJet"   + suffix, "", 256, -3.2,    3.2);
	hPhiSecondJet   [i][j][iCharge] = new TH1F("hPhiSecondJet"    + suffix, "", 256, -3.2,    3.2);
	hDRLeadingJetLep[i][j][iCharge] = new TH1F("hDRLeadingJetLep" + suffix, "", 300,  0,      6);
	hDEtaJets       [i][j][iCharge] = new TH1F("hDEtaJets"        + suffix, "", 240, -6,      6);
	hInvMass2Jet    [i][j][iCharge] = new TH1F("hInvMass2Jet"     + suffix, "", 400,  0,   4000);
      }
    }
  }
  
  for (UInt_t i=0; i<2; i++) {

    TString suffix = (i == Muon) ? "MM" : "EE";

    hInvMass2LepBB[i] = new TH1F("hInvMass2LepBB_" + suffix, "", 2000, 0, 200);
    hInvMass2LepBE[i] = new TH1F("hInvMass2LepBE_" + suffix, "", 2000, 0, 200);
    hInvMass2LepEE[i] = new TH1F("hInvMass2LepEE_" + suffix, "", 2000, 0, 200);
  }


  // SF, FR, PR and trigger efficiencies histograms
  //----------------------------------------------------------------------------
  MuonSF = LoadHistogram("MuSF_2012",            "h2inverted", "MuonSF");
  ElecSF = LoadHistogram("EleSF_2012",           "h2inverted", "ElecSF");
  MuonPR = LoadHistogram("MuPR_Moriond13_2012",  "h2inverted", "MuonPR");
  ElecPR = LoadHistogram("ElePR_Moriond13_2012", "h2inverted", "ElecPR");

  MuonFR = LoadHistogram(Form("MuFR_Moriond13_jet%s_EWKcorr", muonJetPt.Data()),
			 "FR_pT_eta_EWKcorr",
			 Form("MuonFR_Jet%s", muonJetPt.Data()));

  ElecFR = LoadHistogram(Form("EleFR_Moriond13_jet%s_EWKcorr", elecJetPt.Data()),
			 "fakeElH2",
			 Form("ElecFR_Jet%s", elecJetPt.Data()));

  DoubleElLead  = LoadHistogram("triggerEfficiencies", "DoubleElLead",  "DoubleElLead");
  DoubleMuLead  = LoadHistogram("triggerEfficiencies", "DoubleMuLead",  "DoubleMuLead");
  DoubleElTrail = LoadHistogram("triggerEfficiencies", "DoubleElTrail", "DoubleElTrail");
  DoubleMuTrail = LoadHistogram("triggerEfficiencies", "DoubleMuTrail", "DoubleMuTrail");


  // Electron energy scale systematics
  //----------------------------------------------------------------------------
  hScaleInEB  = GetHistogramFromGraph("hScaleInEB",  "gScaleInEB");
  hScaleOutEB = GetHistogramFromGraph("hScaleOutEB", "gScaleOutEB");
  hScaleEE    = GetHistogramFromGraph("hScaleEE",    "gScaleEE");


  // Input and output files
  //----------------------------------------------------------------------------
  isData = (_sample.Contains("Run2012")) ? true : false;


  if (isData)
    {
      txt_data_events.open(Form("%s/%s_data_events.txt", _directory.Data(), filename.Data()));
    }

  if (_sample.Contains("074_WZJetsMad"))
    {
      txt_signal_events.open(Form("%s/%s_signal_events.txt", _directory.Data(), filename.Data()));
    }


  TChain* tree = new TChain("latino", "latino");

  TString path;

  if (isData)
    {
      path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17newJEC/Data_LooseLooseTypeI";
      
      if (_mode == ATLAS) path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17newJEC/Data_NoSelTypeI";
    }
  else
    {
      path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17newJEC/MC_LooseLooseTypeI";

      if (_mode == ATLAS) path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17newJEC/MC_NoSelTypeI";  // Falta 076_WZJetsMad_TuneZ2star
    }

  tree->Add(path + "/latino_" + _sample + ".root");


  // Tree leaves
  //----------------------------------------------------------------------------
  tree->SetBranchAddress("run",           &run);
  tree->SetBranchAddress("event",         &event);
  tree->SetBranchAddress("lumi",          &lumi);
  tree->SetBranchAddress("baseW",         &baseW);
  tree->SetBranchAddress("channel",       &channel);
  tree->SetBranchAddress("chmet",         &chmet);
  tree->SetBranchAddress("chmetphi",      &chmetphi);
  tree->SetBranchAddress("dataset",       &dataset);
  tree->SetBranchAddress("fakeW",         &fakeW);
  tree->SetBranchAddress("njet",          &njet);
  tree->SetBranchAddress("nextra",        &nextra);
  tree->SetBranchAddress("nvtx",          &nvtx);
  tree->SetBranchAddress("pfmet",         &pfmet);
  tree->SetBranchAddress("pfmetphi",      &pfmetphi);
  tree->SetBranchAddress("puW",           &puW);
  tree->SetBranchAddress("trigger",       &trigger);

  for (UInt_t i=0; i<number_of_leptons; i++)
    {
      tree->SetBranchAddress(Form("bdt%d",           i+1), &bdt          [i]);
      tree->SetBranchAddress(Form("ch%d",            i+1), &ch           [i]);
      tree->SetBranchAddress(Form("eta%d",           i+1), &eta          [i]);
      tree->SetBranchAddress(Form("iso%d",           i+1), &iso          [i]);
      tree->SetBranchAddress(Form("isomva%d",        i+1), &isomva       [i]);
      tree->SetBranchAddress(Form("pass2012ICHEP%d", i+1), &pass2012ICHEP[i]);
      tree->SetBranchAddress(Form("phi%d",           i+1), &phi          [i]);
      tree->SetBranchAddress(Form("pt%d",            i+1), &pt           [i]);

      if (_mode == ATLAS) tree->SetBranchAddress(Form("ip%d", i+1), &ip[i]);
    }

  for (UInt_t i=0; i<number_of_jets; i++)
    {
      tree->SetBranchAddress(Form("jeteta%d",  i+1), &jeteta [i]);
      tree->SetBranchAddress(Form("jetphi%d",  i+1), &jetphi [i]);
      tree->SetBranchAddress(Form("jetpt%d",   i+1), &jetpt  [i]);
      tree->SetBranchAddress(Form("jettche%d", i+1), &jettche[i]);
      tree->SetBranchAddress(Form("jetid%d",   i+1), &jetid  [i]);
    }


  // Leaves for aTGC tree
  //----------------------------------------------------------------------------
  if (_sample.Contains("GenVars"))
    {
      tree->SetBranchAddress("WZchan", &gen_channel);
      tree->SetBranchAddress("MZ",     &gen_mZ);
      tree->SetBranchAddress("PtZ",    &gen_ptZ);
    }
  else
    {
      gen_channel = -999.;
      gen_mZ      = -999.;
      gen_ptZ     = -999.;
    }


  //============================================================================
  //
  // Event loop
  //
  //============================================================================
  for (int ievent=0; ievent<tree->GetEntries(); ievent++) {


    // Initialize data members
    //--------------------------------------------------------------------------
    efficiency_weight = 1.;
    dd_weight         = 1.;
    mc_lepton_weight  = 1.;
    mc_trigger_weight = 1.;
    mc_total_weight   = 1.;

    invMass2Lep    = 999.;
    invMass3Lep    = 999.;
    transverseMass = 999.;

    sumCharges   = 0.;
    nElectron    = 0;
    nJetAbove30  = 0;
    nJetBelow30  = 0;
    nBJetAbove30 = 0;
    nTight       = 0;


    // Reset some aTGC variables
    //--------------------------------------------------------------------------
    reco_mZ  = 0.;
    reco_ptZ = 0.;


    // GetEntry
    //--------------------------------------------------------------------------
    tree->GetEntry(ievent);

    pu_weight = (_systematic == pileupSyst) ? 1.0 : puW;

    if (!trigger) continue;

    if (run == 201191) continue;

    xs_weight = (1.0 + 0.6 * (dataset >= 82 && dataset <= 84)) * baseW * luminosity;

    if (_sample.Contains("WZZJets")) xs_weight *= (0.01968  / 0.0192);
    if (_sample.Contains("ZZZJets")) xs_weight *= (0.005527 / 0.00459);
    if (_sample.Contains("WWZJets")) xs_weight *= (0.05795  / 0.0633);
    if (_sample.Contains("WWWJets")) xs_weight *= (0.08058  / 0.0822);
    if (_sample.Contains("TTWJets")) xs_weight *= (0.232    / 0.232);
    if (_sample.Contains("TTZJets")) xs_weight *= (0.2057   / 0.174);
	
    if (isData) xs_weight = 1.0;

    Bool_t accept_WGstar = (chmet < (0.75*pt[0]+100) && chmet < (0.75*jetpt[0]+100));

    if (dataset == 82 && !accept_WGstar) continue;


    // Selection efficiency (denominator) as a function of the generated Z mass
    //--------------------------------------------------------------------------
    if (_sample.Contains("GenVars"))
      {
	h_gen_mZ_denominator->Fill(gen_mZ);
      }


    // Set the MET of the event
    //--------------------------------------------------------------------------
    EventMET = GetMET(pfmet, pfmetphi);
    
    TrackMET = GetMET(chmet, chmetphi);


    // Loop over leptons
    //--------------------------------------------------------------------------
    AnalysisLeptons.clear();

    for (UInt_t i=0; i<number_of_leptons; i++) {

      if (pt[i] <= 0.) continue;

      Lepton lep;
      
      lep.index = i;

      lep.flavor = (bdt[i] < 100.) ? Electron : Muon;

      Float_t spt = ScaleLepton(lep.flavor, pt[i], eta[i]);

      if (spt <= 10.) continue;

      lep.type = (pass2012ICHEP[i]) ? Tight : Fail;
    
      lep.charge = ch[i];

      Float_t mass;

      //------------------------------------------------------------------------
      //
      // Electron
      //
      //------------------------------------------------------------------------
      if (lep.flavor == Electron)
	{
	  mass = E_MASS;

	  lep.sf    = GetFactor(ElecSF,        spt, eta[i]);
	  lep.pr    = GetFactor(ElecPR,        spt, eta[i]);
	  lep.fr    = GetFactor(ElecFR,        spt, eta[i]);
	  lep.lead  = GetFactor(DoubleElLead,  spt, eta[i]);
	  lep.trail = GetFactor(DoubleElTrail, spt, eta[i]);
	}
      //------------------------------------------------------------------------
      //
      // Muon 
      //
      //------------------------------------------------------------------------
      else
	{
	  mass = M_MASS;

	  lep.sf    = GetFactor(MuonSF,        spt, eta[i]);
	  lep.pr    = GetFactor(MuonPR,        spt, eta[i]);
	  lep.fr    = GetFactor(MuonFR,        spt, eta[i], 34.);
	  lep.lead  = GetFactor(DoubleMuLead,  spt, eta[i]);
	  lep.trail = GetFactor(DoubleMuTrail, spt, eta[i]);
	}

      TLorentzVector tlv;

      tlv.SetPtEtaPhiM(spt, eta[i], phi[i], mass);
      
      lep.v = tlv;

      lep.vraw = (pt[i] / spt) * tlv;

      AnalysisLeptons.push_back(lep);
    }


    // Require at least two leptons
    //--------------------------------------------------------------------------
    if (AnalysisLeptons.size() < 2) continue;


    // Fill the selected jets vector
    //--------------------------------------------------------------------------
    SelectedJets.clear();
    LowPtJets.clear();

    for (UInt_t i=0; i<number_of_jets; i++) {

      if (fabs(jeteta[i]) >= 4.7) continue;

      TLorentzVector Jet;
      
      Jet.SetPtEtaPhiM(jetpt[i], jeteta[i], jetphi[i], 0);

      Bool_t thisJetIsLepton = false;
      
      for (UInt_t j=0; j<AnalysisLeptons.size(); j++)
	{
	  if (Jet.DeltaR(AnalysisLeptons[j].v) <= 0.4)
	    {
	      thisJetIsLepton = true;
	      
	      break;
	    }
	}

      if (thisJetIsLepton) continue;

      if (jetpt[i] > 30.)
	{
	  SelectedJets.push_back(Jet);
	  
	  nJetAbove30++;
	  
	  if (jettche[i] > 2.1 && jetid[i] >= 4) nBJetAbove30++;  // jetid 4 = MVA LOOSE
	}
      else if (jetpt[i] > 20.)
	{
	  LowPtJets.push_back(Jet);

	  nJetBelow30++;
	}
    }


    // Inclusive and {0,1,2}-jet cross sections
    //--------------------------------------------------------------------------
    if (_jetChannel >= 0 && nJetAbove30 != _jetChannel) continue;


    // MET systematic uncertainty
    //--------------------------------------------------------------------------
    if (_systematic == metUpSyst || _systematic == metDownSyst)
      {
	TLorentzVector approxMET;
	TLorentzVector smearMET;

	for (UInt_t i=0; i<number_of_jets; i++)
	  {
	    if (jetpt[i] < 0) continue;

	    Float_t scale = METJETSPTSYS;

	    if (_systematic == metDownSyst) scale = -scale;

	    TLorentzVector Jet;
	    
	    Jet.SetPtEtaPhiM(jetpt[i], jeteta[i], jetphi[i], 0);

	    if (Jet.Pt() <= 30. || fabs(Jet.Eta() >= 5.))
	      {
		approxMET -= Jet;
		
		Jet.SetPx(Jet.Px() * (1. + scale));
		Jet.SetPy(Jet.Py() * (1. + scale));
		
		smearMET -= Jet;

		continue;
	      }

	    Bool_t thisJetIsLepton = false;

	    for (UInt_t j=0; j<AnalysisLeptons.size(); j++)
	      {
		if (Jet.DeltaR(AnalysisLeptons[j].v) <= 0.3)
		  {
		    thisJetIsLepton = true;
		    
		    break;
		  }
	      }

	    if (thisJetIsLepton) continue;
	    
	    approxMET -= Jet;

	    Jet.SetPx(Jet.Px() * (1. + scale));
	    Jet.SetPy(Jet.Py() * (1. + scale));

	    smearMET -= Jet;
	  }

	for (UInt_t i=0; i<AnalysisLeptons.size(); i++)
	  {
	    Float_t scale = (AnalysisLeptons[i].flavor == Muon) ? METMUONPTSYS : METELECPTSYS;

	    if (_systematic == metDownSyst) scale = -scale;

	    TLorentzVector tlv = AnalysisLeptons[i].v;

	    approxMET -= tlv;

	    tlv.SetPx(tlv.Px() * (1. + scale));
	    tlv.SetPy(tlv.Py() * (1. + scale));

	    smearMET -= tlv;
	  }

	const Float_t resolMET = fabs(approxMET.Pt() - smearMET.Pt()) / approxMET.Pt();

	EventMET *= (1. + resolMET); 
      }


    // Lepton scale propagation to the MET
    //--------------------------------------------------------------------------
    if (_systematic == muonUpSyst ||
	_systematic == muonDownSyst ||
	_systematic == electronUpSyst ||
	_systematic == electronDownSyst)
      {
	for (UInt_t i=0; i<AnalysisLeptons.size(); i++)
	  {
	    EventMET += (AnalysisLeptons[i].vraw - AnalysisLeptons[i].v);
	  }
      }


    // Apply DeltaR cut
    //--------------------------------------------------------------------------
    Float_t smallestDeltaR = 999.;

    for (UInt_t i=0; i<AnalysisLeptons.size(); i++) {

      for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++) {

	Float_t tmpDeltaR = AnalysisLeptons[i].v.DeltaR(AnalysisLeptons[j].v);

	if (tmpDeltaR < smallestDeltaR) smallestDeltaR = tmpDeltaR;
      }
    }

    if (smallestDeltaR < 0.001) continue;


    // Make Z and W candidates
    //--------------------------------------------------------------------------
    Bool_t isZee = false;
    Bool_t isZmm = false;

    for (UInt_t i=0; i<AnalysisLeptons.size(); i++) {

      for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++) {
      
	if (AnalysisLeptons[i].flavor != AnalysisLeptons[j].flavor) continue;

	if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0.) continue;

	Float_t inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

	if (fabs(inv_mass - Z_MASS) < fabs(invMass2Lep - Z_MASS)) {

	  invMass2Lep = inv_mass;

	  ZLepton1 = AnalysisLeptons[i];
	  ZLepton2 = AnalysisLeptons[j];
	  
	  isZee = (AnalysisLeptons[i].flavor == Electron) ? true : false;
	  isZmm = (AnalysisLeptons[i].flavor == Muon)     ? true : false;

	  if (AnalysisLeptons.size() != 3) continue;

	  if (_mode == ATLAS && AnalysisLeptons[i].type != Tight) continue;
	  if (_mode == ATLAS && AnalysisLeptons[j].type != Tight) continue;

	  for (UInt_t k=0; k<3; k++) {
	
	    if (k == i) continue;
	    if (k == j) continue;


	    // ATLAS data-driven
	    //------------------------------------------------------------------
	    if (_mode == ATLAS)
	      {
		if (AnalysisLeptons[k].type == Electron)
		  {
		    if (fabs(ip[k]) < 0.02) continue;
		  }
		if (AnalysisLeptons[k].type == Muon)
		  {
		    Float_t ipcut = (pt[k] < 20.) ? 0.01 : 0.02;

		    if (fabs(ip[k]) < ipcut) continue;
		  }
	      }

	    WLepton = AnalysisLeptons[k];
	  }
	}
      }
    }

    if (!isZee && !isZmm) continue;

    if (fabs(WLepton.charge) < 1.) continue;


    // Fill Z invariant mass at two-lepton level
    //--------------------------------------------------------------------------
    UInt_t index = (isZee) ? Electron : Muon;

    if (fabs(ZLepton1.v.Eta()) < 1.479 && fabs(ZLepton2.v.Eta()) < 1.479)
      {
	hInvMass2LepBB[index]->Fill(invMass2Lep, pu_weight * xs_weight);
      }
    else if (fabs(ZLepton1.v.Eta()) < 1.479 || fabs(ZLepton2.v.Eta()) < 1.479)
      {
	hInvMass2LepBE[index]->Fill(invMass2Lep, pu_weight * xs_weight);
      }
    else
      {
	hInvMass2LepEE[index]->Fill(invMass2Lep, pu_weight * xs_weight);
      }


    // Require exactly three leptons
    //--------------------------------------------------------------------------
    if (AnalysisLeptons.size() != 3) continue;


    // Classify the channels
    //--------------------------------------------------------------------------
    for (UInt_t i=0; i<3; i++)
      {
	if (AnalysisLeptons[i].flavor == Electron) nElectron++;
	if (AnalysisLeptons[i].type   == Tight)    nTight++;
	
	sumCharges += AnalysisLeptons[i].charge;
      }

    if (_mode == RAW && nTight != 3) continue;

    invMass3Lep = (AnalysisLeptons[0].v + AnalysisLeptons[1].v + AnalysisLeptons[2].v).M();

    if      (nElectron == 0) reco_channel = MMM;
    else if (nElectron == 1) reco_channel = MME;
    else if (nElectron == 2) reco_channel = EEM;
    else if (nElectron == 3) reco_channel = EEE;


    // Apply lepton SF and trigger efficiencies
    //--------------------------------------------------------------------------
    if (!isData)
      {
	for (UInt_t i=0; i<3; i++) mc_lepton_weight *= AnalysisLeptons[i].sf;

	mc_trigger_weight = GetTriggerWeight();

	mc_total_weight = mc_lepton_weight * mc_trigger_weight;

	efficiency_weight *= mc_total_weight;
      }


    // Data-driven estimates
    //--------------------------------------------------------------------------
    if (_mode == PPF)
      {
	dd_weight =
	  GetDataDrivenWeight(2) + 
	  GetDataDrivenWeight(1) + 
	  GetDataDrivenWeight(0);
      }


    // Compute the transverse mass
    //--------------------------------------------------------------------------
    const Float_t met  = EventMET.Et();
    const Float_t lWEt = WLepton.v.Et();

    transverseMass = lWEt*lWEt + met*met - 2.*lWEt*met*cos(WLepton.v.Angle(EventMET.Vect()));

    transverseMass = sqrt(transverseMass);


    //--------------------------------------------------------------------------
    //
    //  VBF selection
    //
    //  The following is a standard selection for either vector boson fusion or
    //  the related vector boson scattering. You'll find that almost all pheno
    //  studies and cross section predictions follow a similar selection. Note
    //  the expectation for 8 TeV would be only a few events from EWK VV
    //  scattering or backgrounds pass this selection. What we would be
    //  interested in is a similar selection, initially without the last three
    //  cuts that focus on scattering.
    //
    //                                             Matt Herndon
    //
    //  Lepton  pT > 20.0 GeV
    //  Lepton |eta| < 2.5
    //  Jet pT > 30, 2 jets
    //  Jet |eta| < 4.7
    //  (the jet eta range is important to have any sensitivity)
    //  deltaR (l,l) > 0.4, all lepton pairs
    //  (this cut has to be removed to have sensitivity to aQGC which can
    //  involve TeV momentum vector bosons)
    //  deltaR (l,j) > 0.4 ---> 0.3
    //  deltaEta(j,j') > 4.0
    //  etaj1*etaj2 > 0.0
    //  m_jj > 600 GeV
    //
    //--------------------------------------------------------------------------
    if (nJetAbove30 == 2 &&
	AnalysisLeptons[2].v.Pt() > 20.)
      {
	Bool_t deltaR_lepton_lepton = true;

	for (UInt_t i=0; i<AnalysisLeptons.size(); i++)
	  {
	    TLorentzVector iLep = AnalysisLeptons[i].v;
	    
	    for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++)
	      {
		TLorentzVector jLep = AnalysisLeptons[j].v;
		
		if (iLep.DeltaR(jLep) <= 0.4) deltaR_lepton_lepton = false;
	      }
	  }

	if (deltaR_lepton_lepton &&
	    fabs(invMass2Lep - Z_MASS) < 20. &&
	    EventMET.Et() > 30.)
	  {
	    FillHistograms(reco_channel, VBFSelection);
	    FillHistograms(combined,     VBFSelection);
	  }
      }
    
    
    // Fill histograms
    //--------------------------------------------------------------------------
    FillHistograms(reco_channel, Exactly3Leptons);
    FillHistograms(combined,     Exactly3Leptons);

    if (invMass3Lep <= 100.) continue;

    FillHistograms(reco_channel, InvMass3Lep100);
    FillHistograms(combined,     InvMass3Lep100);

    if (ZLepton1.v.Pt() <= 20.) continue;

    if (fabs(invMass2Lep - Z_MASS) > 25. && nJetAbove30 > 1 && nBJetAbove30 > 0 && EventMET.Et() > 40.)
      {
	FillHistograms(reco_channel, TopRegion);
	FillHistograms(combined,     TopRegion);
      }
    
    if (fabs(invMass2Lep - Z_MASS) >= 20. && _mode != ATLAS) continue;

    if ((invMass2Lep < 50. || invMass2Lep > 120.) && _mode == ATLAS) continue;
    
    FillHistograms(reco_channel, HasZ);
    FillHistograms(combined,     HasZ);

    if (WLepton.v.DeltaR(ZLepton1.v) <= 0.1) continue;
    if (WLepton.v.DeltaR(ZLepton2.v) <= 0.1) continue;

    if (fabs(invMass2Lep - Z_MASS) < 15. && EventMET.Et() < 20.)
      {
	FillHistograms(reco_channel, ZJetsRegion);
	FillHistograms(combined,     ZJetsRegion);
      }

    if (WLepton.v.Pt() <= 20.) continue;

    FillHistograms(reco_channel, HasW);
    FillHistograms(combined,     HasW);

    if (EventMET.Et() <= 30.) continue;
    
    FillHistograms(reco_channel, MET30);
    FillHistograms(combined,     MET30);

    if (AnalysisLeptons[2].v.Pt() > 20.)
      {
	FillHistograms(reco_channel, MET30Pt20);
	FillHistograms(combined,     MET30Pt20);

	if (nBJetAbove30 == 0)
	  {
	    FillHistograms(reco_channel, MET30Pt20Btag);
	    FillHistograms(combined,     MET30Pt20Btag);
	  }
      }

    if (nBJetAbove30 == 0)
      {
	FillHistograms(reco_channel, MET30Btag);
	FillHistograms(combined,     MET30Btag);
      }
    
    if (EventMET.Et() > 40.)
      {
	FillHistograms(reco_channel, MET40);
	FillHistograms(combined,     MET40);
      }
    
    
    // EXO-12-025
    //--------------------------------------------------------------------------
    Bool_t etaElec1 = true;
    Bool_t etaElec2 = true;
    Bool_t etaElec3 = true;

    if (ZLepton1.flavor == Electron && fabs(ZLepton1.v.Eta()) >= 1.4442 && fabs(ZLepton1.v.Eta()) <= 1.566) etaElec1 = false;
    if (ZLepton2.flavor == Electron && fabs(ZLepton2.v.Eta()) >= 1.4442 && fabs(ZLepton2.v.Eta()) <= 1.566) etaElec2 = false;
    if (WLepton.flavor  == Electron && fabs(WLepton.v.Eta())  >= 1.4442 && fabs(WLepton.v.Eta())  <= 1.566) etaElec3 = false;
    
    if (((ZLepton1.flavor == Electron && ZLepton1.v.Pt() > 35) || (ZLepton1.flavor == Muon && ZLepton1.v.Pt() > 25)) &&
	((ZLepton2.flavor == Electron && ZLepton2.v.Pt() > 35) || ZLepton2.flavor == Muon) &&
	invMass3Lep > 120 &&
	WLepton.v.DeltaR(ZLepton1.v) > 0.3 &&
	WLepton.v.DeltaR(ZLepton2.v) > 0.3 &&
	etaElec1 && etaElec2 && etaElec3)
      {
	FillHistograms(reco_channel, EXO_12_025);
	FillHistograms(combined,     EXO_12_025);
      }


    // Fill aTGC tree and selection efficiency numerator
    //--------------------------------------------------------------------------
    if (_sample.Contains("GenVars"))
      {
	reco_mZ  = invMass2Lep;
	reco_ptZ = (ZLepton1.v + ZLepton2.v).Pt();
    
	tgcTree->Fill();

	h_gen_mZ_numerator->Fill(gen_mZ);
      }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //
    // Additional cuts
    //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (transverseMass > 40. &&
	AnalysisLeptons[2].v.Pt() > 20. &&
	nBJetAbove30 == 0 &&
	EventMET.Et() > 40. &&
	fabs(invMass2Lep - Z_MASS) < 15.)
      {
	FillHistograms(reco_channel, SantiagoCuts);
	FillHistograms(combined,     SantiagoCuts);
      }


    // Save data events in a txt file
    //--------------------------------------------------------------------------
    if (isData)
      {
	txt_data_events << Form(" [%s] run:%lld  event:%10lld  lumi:%4lld  met:%7.3f  metphi:%7.3f  mll:%7.3f",
				sChannel[reco_channel].Data(),
				run, event, lumi,
				EventMET.Et(), EventMET.Phi(), invMass2Lep);

	txt_data_events << Form("  zl1pt:%7.3f  zl1eta:%7.3f  zl2pt:%7.3f  zl2eta:%7.3f  wlpt:%7.3f  wleta:%7.3f\n",
				ZLepton1.v.Pt(), ZLepton1.v.Eta(),
				ZLepton2.v.Pt(), ZLepton2.v.Eta(),
				WLepton.v.Pt(),  WLepton.v.Eta());
      }


    // Save signal events in a txt file
    //--------------------------------------------------------------------------
    if (_sample.Contains("074_WZJetsMad"))
      {
	txt_signal_events << Form(" [%s] event:%10lld  pu_w:%f  lepton_w:%f  trigger_w:%f  xs_w:%f\n",
				  sChannel[reco_channel].Data(),
				  event, pu_weight, mc_lepton_weight, mc_trigger_weight, xs_weight);
      }
  }
  

  //============================================================================
  //
  // Summary
  //
  //============================================================================
  txt_output.open(Form("%s/%s.txt", _directory.Data(), filename.Data()));

  txt_output << Form("\n %39s results with %7.1f pb\n", filename.Data(), luminosity);

  CounterSummary("No weights");
  CounterSummary("All weights");

  txt_output.close();


  if (isData) txt_data_events.close();

  if (_sample.Contains("074_WZJetsMad")) txt_signal_events.close();


  root_output->cd();

  if (_sample.Contains("GenVars")) tgcTree->Write();

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void FillHistograms(UInt_t iChannel, UInt_t iCut)
{
  Float_t hweight            = pu_weight * efficiency_weight * xs_weight * dd_weight;
  Float_t deltaPhiZLeptons   = ZLepton1.v.DeltaPhi(ZLepton2.v);
  Float_t deltaPhiWLeptonMET = WLepton.v.DeltaPhi(EventMET);
  Float_t deltaR1            = WLepton.v.DeltaR(ZLepton1.v);
  Float_t deltaR2            = WLepton.v.DeltaR(ZLepton2.v);

  Float_t deltaEtaJets = -999;
  Float_t invMass2Jet  = -999;

  if (nJetAbove30 > 1)
    {
      deltaEtaJets = SelectedJets[0].Eta() - SelectedJets[1].Eta();
      invMass2Jet  = (SelectedJets[0] + SelectedJets[1]).M();
    }


  // Minimum deltaR between the leading jet and the leptons
  Float_t minDeltaRLeadingJetLep = 999;

  if (nJetAbove30 > 0 || nJetBelow30 > 0)
    {
      Float_t theDeltaRLeadingJetLep;
      
      for (UInt_t i=0; i<3; i++)
	{
	  if (nJetAbove30 > 0)
	    {
	      theDeltaRLeadingJetLep = SelectedJets[0].DeltaR(AnalysisLeptons[i].v);
	    }
	  else
	    {
	      theDeltaRLeadingJetLep = LowPtJets[0].DeltaR(AnalysisLeptons[i].v);
	    }
	  
	  if (theDeltaRLeadingJetLep < minDeltaRLeadingJetLep)
	    {
	      minDeltaRLeadingJetLep = theDeltaRLeadingJetLep;
	    }
	}
    }


  // Minimum deltaR and minimum invariant mass among the same flavor and
  // opposite charge lepton pairs
  Float_t minDeltaR2Lep  = 999;
  Float_t minInvMass2Lep = 999;

  for (UInt_t i=0; i<3; i++)
    {
      for (UInt_t j=i+1; j<3; j++)
	{
	  if (AnalysisLeptons[i].flavor != AnalysisLeptons[j].flavor) continue;

	  if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0.) continue;

	  Float_t theDeltaR2Lep = AnalysisLeptons[i].v.DeltaR(AnalysisLeptons[j].v);

	  Float_t theInvMass2Lep = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();
  
	  if (theDeltaR2Lep < minDeltaR2Lep) minDeltaR2Lep = theDeltaR2Lep;

	  if (theInvMass2Lep < minInvMass2Lep) minInvMass2Lep = theInvMass2Lep;
	}
    }


  for (UInt_t iCharge=0; iCharge<nCharge; iCharge++)
    {
      if (iCharge == WPlus  && WLepton.charge < 0.) continue;
      if (iCharge == WMinus && WLepton.charge > 0.) continue;


      // Counters
      //------------------------------------------------------------------------
      hCounterRaw[iChannel][iCut][iCharge][nTight]->Fill(1);
      hCounterEff[iChannel][iCut][iCharge][nTight]->Fill(1, pu_weight * efficiency_weight             * dd_weight);
      hCounter   [iChannel][iCut][iCharge][nTight]->Fill(1, pu_weight * efficiency_weight * xs_weight * dd_weight);

      hCounterRaw[iChannel][iCut][iCharge][LLL]->Fill(1);
      hCounterEff[iChannel][iCut][iCharge][LLL]->Fill(1, pu_weight * efficiency_weight             * dd_weight);
      hCounter   [iChannel][iCut][iCharge][LLL]->Fill(1, pu_weight * efficiency_weight * xs_weight * dd_weight);

      for (UInt_t iPrompt=0; iPrompt<=3; iPrompt++)
	{
	  hPromptCounter[iChannel][iCut][iCharge][iPrompt]->Fill(1, GetDataDrivenWeight(iPrompt));
	}

      hPromptCounter[iChannel][iCut][iCharge][4]->Fill(1,
						       GetDataDrivenWeight(0) +
						       GetDataDrivenWeight(1) +
						       GetDataDrivenWeight(2) +
						       GetDataDrivenWeight(3));


      // MC weight histograms
      //------------------------------------------------------------------------
      hLeptonWeight [reco_channel][iCut][iCharge]->Fill(mc_lepton_weight,  pu_weight);
      hTriggerWeight[reco_channel][iCut][iCharge]->Fill(mc_trigger_weight, pu_weight);
      hTotalWeight  [reco_channel][iCut][iCharge]->Fill(mc_total_weight,   pu_weight);


      // Analysis histograms
      //------------------------------------------------------------------------
      hNPV           [iChannel][iCut][iCharge]->Fill(nvtx,                           hweight);
      hMET           [iChannel][iCut][iCharge]->Fill(EventMET.Et(),                  hweight);
      h_chmet        [iChannel][iCut][iCharge]->Fill(chmet,                          hweight);
      hSumCharges    [iChannel][iCut][iCharge]->Fill(sumCharges,                     hweight);
      hInvMass2Lep   [iChannel][iCut][iCharge]->Fill(invMass2Lep,                    hweight);
      hInvMass3Lep   [iChannel][iCut][iCharge]->Fill(invMass3Lep,                    hweight);
      hPtLepton1     [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[0].v.Pt(),      hweight);
      hPtLepton2     [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[1].v.Pt(),      hweight);
      hPtLepton3     [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[2].v.Pt(),      hweight);
      hDPhiZLeptons  [iChannel][iCut][iCharge]->Fill(fabs(deltaPhiZLeptons),         hweight);
      hDPhiWLeptonMET[iChannel][iCut][iCharge]->Fill(fabs(deltaPhiWLeptonMET),       hweight);
      hPtZLepton1    [iChannel][iCut][iCharge]->Fill(ZLepton1.v.Pt(),                hweight);
      hPtZLepton2    [iChannel][iCut][iCharge]->Fill(ZLepton2.v.Pt(),                hweight);
      hPtZ           [iChannel][iCut][iCharge]->Fill((ZLepton1.v + ZLepton2.v).Pt(), hweight);
      hPtWLepton     [iChannel][iCut][iCharge]->Fill(WLepton.v.Pt(),                 hweight);
      hPtW           [iChannel][iCut][iCharge]->Fill((WLepton.v + EventMET).Pt(),    hweight);
      hDRWZLepton1   [iChannel][iCut][iCharge]->Fill(deltaR1,                        hweight);
      hDRWZLepton2   [iChannel][iCut][iCharge]->Fill(deltaR2,                        hweight);
      hMtW           [iChannel][iCut][iCharge]->Fill(transverseMass,                 hweight);
      hMinDeltaR2Lep [iChannel][iCut][iCharge]->Fill(minDeltaR2Lep,                  hweight);
      hMinInvMass2Lep[iChannel][iCut][iCharge]->Fill(minInvMass2Lep,                 hweight);
      hNJetAbove30   [iChannel][iCut][iCharge]->Fill(nJetAbove30,                    hweight);
      hNJetBelow30   [iChannel][iCut][iCharge]->Fill(nJetBelow30,                    hweight);
      hNBJetAbove30  [iChannel][iCut][iCharge]->Fill(nBJetAbove30,                   hweight);

      hPtLepton3_invMass2Lep[iChannel][iCut][iCharge]->Fill(AnalysisLeptons[2].v.Pt(), invMass2Lep, hweight);
      hPtWLepton_invMass2Lep[iChannel][iCut][iCharge]->Fill(WLepton.v.Pt(),            invMass2Lep, hweight);


      // Jet histograms
      //------------------------------------------------------------------------
      if (nJetAbove30 > 0)
	{
	  hPtLeadingJet   [iChannel][iCut][iCharge]->Fill(SelectedJets[0].Pt(),   hweight);
	  hEtaLeadingJet  [iChannel][iCut][iCharge]->Fill(SelectedJets[0].Eta(),  hweight);
	  hPhiLeadingJet  [iChannel][iCut][iCharge]->Fill(SelectedJets[0].Phi(),  hweight);
	  hDRLeadingJetLep[iChannel][iCut][iCharge]->Fill(minDeltaRLeadingJetLep, hweight);

	  if (nJetAbove30 > 1)
	    {
	      hPtSecondJet [iChannel][iCut][iCharge]->Fill(SelectedJets[1].Pt(),  hweight);
	      hEtaSecondJet[iChannel][iCut][iCharge]->Fill(SelectedJets[1].Eta(), hweight);
	      hPhiSecondJet[iChannel][iCut][iCharge]->Fill(SelectedJets[1].Phi(), hweight);
	      hDEtaJets    [iChannel][iCut][iCharge]->Fill(deltaEtaJets,          hweight);
	      hInvMass2Jet [iChannel][iCut][iCharge]->Fill(invMass2Jet,           hweight);
	  }
	}
      else if (nJetBelow30 > 0)
	{
	  hPtLeadingJet   [iChannel][iCut][iCharge]->Fill(LowPtJets[0].Pt(),      hweight);
	  hEtaLeadingJet  [iChannel][iCut][iCharge]->Fill(LowPtJets[0].Eta(),     hweight);
	  hPhiLeadingJet  [iChannel][iCut][iCharge]->Fill(LowPtJets[0].Phi(),     hweight);
	  hDRLeadingJetLep[iChannel][iCut][iCharge]->Fill(minDeltaRLeadingJetLep, hweight);
	}
    }
}


//------------------------------------------------------------------------------
// GetTriggerWeight
//------------------------------------------------------------------------------
Float_t GetTriggerWeight()
{
  Float_t eL[3];
  Float_t eT[3];

  for (UInt_t i=0; i<3; i++) {

    Lepton lep = AnalysisLeptons[i];

    eL[i] = lep.lead;
    eT[i] = lep.trail;
  }

  Float_t r1 = (1. - eL[0]) * (1. - eL[1]) * (1. - eL[2]);
  Float_t r2 =       eL[0]  * (1. - eT[1]) * (1. - eT[2]);
  Float_t r3 =       eL[1]  * (1. - eT[0]) * (1. - eT[2]);
  Float_t r4 =       eL[2]  * (1. - eT[0]) * (1. - eT[1]); 

  Float_t triggerWeight = 1. - (r1 + r2 + r3 + r4);
  
  return triggerWeight;
}


//------------------------------------------------------------------------------
// LoadHistogram
//------------------------------------------------------------------------------
TH2F* LoadHistogram(TString filename,
		    TString hname,
		    TString cname)
{
  TString path = "../AuxiliaryFilesWZXS8TeV/";

  TFile* inputfile = TFile::Open(path + filename + ".root");

  TH2F* hist = (TH2F*)inputfile->Get(hname)->Clone(cname);
  
  hist->SetDirectory(0);
  
  inputfile->Close();

  return hist;
}


//------------------------------------------------------------------------------
// GetHistogramFromGraph
//------------------------------------------------------------------------------
TH1F* GetHistogramFromGraph(TString hname, TString gname)
{
  TString path = "../AuxiliaryFilesWZXS8TeV/";

  TFile* inputfile = TFile::Open(path + "gScaleSyst-8TeV.root");

  TGraphErrors* graph = (TGraphErrors*)inputfile->Get(gname);

  UInt_t nbins = graph->GetN();

  Double_t* xx = graph->GetX();
  Double_t* yy = graph->GetY();

  Float_t* range = new Float_t[nbins+1];

  for (UInt_t i=0; i<nbins; i++)
    {
      range[i+1] = (xx[i] + xx[i+1]) / 2.;
    }

  range[0]     = xx[0]       - (xx[1]       - xx[0]      ) / 2.;
  range[nbins] = xx[nbins-1] + (xx[nbins-1] - xx[nbins-2]) / 2.;

  TH1F* hist = new TH1F(hname, hname, nbins, range);
  
  hist->SetDirectory(0);

  for (UInt_t i=1; i<=nbins; i++)
    {
      hist->SetBinContent(i, fabs(yy[i-1]));
    }

  inputfile->Close();

  delete [] range;

  return hist;
}


//------------------------------------------------------------------------------
// GetFactor
//------------------------------------------------------------------------------
Float_t GetFactor(TH2F*   h2,
		  Float_t leptonPt,
		  Float_t leptonEta,
		  Float_t leptonPtMax)
{
  Float_t aeta = fabs(leptonEta);

  Int_t nbins = h2->GetNbinsX();

  Float_t ptMax = (leptonPtMax > 0) ? leptonPtMax : h2->GetXaxis()->GetBinCenter(nbins);

  Float_t factor = h2->GetBinContent(h2->FindBin(TMath::Min(leptonPt, ptMax), aeta));

  return factor;
}


//------------------------------------------------------------------------------
// GetDataDrivenWeight
//------------------------------------------------------------------------------
// Weight rules
//
// Tight estimated as PROMPT: p(1-f)
// Tight estimated as   FAKE: f(1-p)
// Fail  estimated as PROMPT: pf
// Fail  estimated as   FAKE: pf
//
// common factor: 1/(p-f)
//------------------------------------------------------------------------------
Float_t GetDataDrivenWeight(UInt_t nPrompt)
{
  Float_t promptProbability[3];
  Float_t fakeProbability[3];

  for (UInt_t i=0; i<3; i++) {
    
    Float_t f = AnalysisLeptons[i].fr;

    Float_t p = AnalysisLeptons[i].pr;

    if (AnalysisLeptons[i].type == Tight)
      {
	promptProbability[i] = p * (1 - f);
	fakeProbability[i]   = f * (1 - p);
      }
    else
      {
	promptProbability[i] = p * f;
	fakeProbability[i]   = p * f;
      }
    
    promptProbability[i] /= (p - f);
    fakeProbability[i]   /= (p - f);
  }


  Float_t result = -999.;

  if (nPrompt == 3)
    {
      result = promptProbability[0] * promptProbability[1] * promptProbability[2];
    }
  else if (nPrompt == 2)
    {
      Float_t PPF = promptProbability[0] * promptProbability[1] * fakeProbability[2];
      Float_t PFP = promptProbability[0] * fakeProbability[1]   * promptProbability[2];
      Float_t FPP = fakeProbability[0]   * promptProbability[1] * promptProbability[2];
      
      result = PPF + PFP + FPP;
    }
  else if (nPrompt == 1)
    {
      Float_t PFF = promptProbability[0] * fakeProbability[1]   * fakeProbability[2];
      Float_t FPF = fakeProbability[0]   * promptProbability[1] * fakeProbability[2];
      Float_t FFP = fakeProbability[0]   * fakeProbability[1]   * promptProbability[2];
      
      result = PFF + FPF + FFP;
    }
  else if (nPrompt == 0)
    {
      result = fakeProbability[0] * fakeProbability[1] * fakeProbability[2];
    }
  
  Bool_t flipSign = abs((Int_t)nTight - (Int_t)nPrompt) % 2;

  if (flipSign) result *= -1.;

  return result;
}


//------------------------------------------------------------------------------
// GetMET
//------------------------------------------------------------------------------
TLorentzVector GetMET(Float_t metModule, Float_t metPhi)
{
  Float_t px = metModule * cos(metPhi);
  Float_t py = metModule * sin(metPhi);

  TLorentzVector metv(px, py, 0., metModule);

  return metv;
}


//------------------------------------------------------------------------------
// ScaleLepton
//------------------------------------------------------------------------------
Float_t ScaleLepton(UInt_t  flavor,
		    Float_t leptonPt,
		    Float_t leptonEta)
{
  if (_systematic != muonUpSyst     &&
      _systematic != muonDownSyst   &&
      _systematic != electronUpSyst &&
      _systematic != electronDownSyst) return leptonPt;

  Float_t scale = 0.0;

  if (flavor == Muon &&
      (_systematic == muonUpSyst || _systematic == muonDownSyst))
    {
      scale = 0.002;
    }
  else if (flavor == Electron &&
	   (_systematic == electronUpSyst || _systematic == electronDownSyst))
    {
      const Float_t InEBMax  = hScaleInEB ->GetXaxis()->GetBinCenter(hScaleInEB ->GetNbinsX());
      const Float_t OutEBMax = hScaleOutEB->GetXaxis()->GetBinCenter(hScaleOutEB->GetNbinsX());
      const Float_t EEMax    = hScaleEE   ->GetXaxis()->GetBinCenter(hScaleEE   ->GetNbinsX());
      
      const Float_t scaleInEB  = hScaleInEB ->GetBinContent(hScaleInEB ->FindBin(min(leptonPt, InEBMax)));
      const Float_t scaleOutEB = hScaleOutEB->GetBinContent(hScaleOutEB->FindBin(min(leptonPt, OutEBMax)));
      const Float_t scaleEE    = hScaleEE   ->GetBinContent(hScaleEE   ->FindBin(min(leptonPt, EEMax)));

      const Float_t aeta = fabs(leptonEta);
      
      if (aeta < 0.8)
      	{
      	  scale = scaleInEB;
      	}
      else if (aeta >= 0.8 && aeta < 1.479)
      	{
      	  scale = scaleOutEB;
      	}
      else
      	{
      	  scale = scaleEE;
      	}
    }
     
  if (_systematic == muonDownSyst || _systematic == electronDownSyst)
    {
      scale = -scale;
    }


  return (1. + scale) * leptonPt;
}


//------------------------------------------------------------------------------
// CounterSummary
//------------------------------------------------------------------------------
void CounterSummary(TString title)
{
  txt_output << Form("\n %30s [%s]\n", " ", title.Data());

  txt_output << Form("\n %23s %10s %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data(),
		     sChannel[4].Data());

  for (UInt_t i=0; i<nCut; i++) {
      
    for (UInt_t k=nComposition; k-- > 0; ) {

      TString cut = (k == nComposition-1) ? sCut[i] : "";

      txt_output << Form(" %19s", cut.Data());

      for (UInt_t j=0; j<nChannel; j++) {

	TString composition = (j == 0) ? sComposition[k] + " " : "";

	Float_t integral = hCounter[j][i][WInclusive][k]->Integral();

	if (title.Contains("No")) integral = hCounterRaw[j][i][WInclusive][k]->Integral();

	txt_output << Form(" %s%10.0f", composition.Data(), integral);
      }
      
      txt_output << "\n";
    }

    txt_output << "\n";
  }
}
