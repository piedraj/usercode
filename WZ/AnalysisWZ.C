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


const UInt_t nobjects = 4;


const Float_t METELECPTSYS = 0.025;
const Float_t METMUONPTSYS = 0.015;
const Float_t METJETSPTSYS = 0.05;


const Float_t E_MASS =  0.000511;  // [GeV]
const Float_t M_MASS =  0.106;     // [GeV]
const Float_t Z_MASS = 91.1876;    // [GeV]


const UInt_t nChannel = 4;

enum {
  MMM,
  EEE,
  MME,
  EEM
};

const TString sChannel[nChannel] = {
  "MMM",
  "EEE",
  "MME",
  "EEM"
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


const UInt_t nCut = 5;

enum {
  Exactly3Leptons,
  HasZ,
  MET30,
  MET30AntiBtag,
  ClosureTest
};

const TString sCut[nCut] = {
  "Exactly3Leptons",
  "HasZ",
  "MET30",
  "MET30AntiBtag",
  "ClosureTest"
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

enum {RAW, PPF};


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
  electronDownSyst
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

Float_t        FakeWeight           ();

TLorentzVector GetMET               ();

Float_t        ScaleLepton          (UInt_t  flavor,
				     Float_t leptonPt,
				     Float_t leptonEta);

void           CounterSummary       (TString title);


//==============================================================================
//
// Data members
//
//==============================================================================
TH1F*                         hFakeDifference[nChannel];

TH1F*                         hCounterRaw[nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounterPU [nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounterEff[nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounter   [nChannel][nCut][nCharge][nComposition];

TH1F*                         hLeptonWeight [nChannel][nCut][nCharge];
TH1F*                         hTriggerWeight[nChannel][nCut][nCharge];
TH1F*                         hTotalWeight  [nChannel][nCut][nCharge];

TH1F*                         hNPV         [nChannel][nCut][nCharge];
TH1F*                         hMET         [nChannel][nCut][nCharge];
TH1F*                         hSumCharges  [nChannel][nCut][nCharge];
TH1F*                         hInvMass2Lep [nChannel][nCut][nCharge];
TH1F*                         hInvMass3Lep [nChannel][nCut][nCharge];
TH1F*                         hPtLepton1   [nChannel][nCut][nCharge];
TH1F*                         hPtLepton2   [nChannel][nCut][nCharge];
TH1F*                         hPtLepton3   [nChannel][nCut][nCharge];
TH1F*                         hPtLeadingJet[nChannel][nCut][nCharge];
TH1F*                         hDPhiZLeptons[nChannel][nCut][nCharge];
TH1F*                         hPtZLepton1  [nChannel][nCut][nCharge];
TH1F*                         hPtZLepton2  [nChannel][nCut][nCharge];
TH1F*                         hPtZ         [nChannel][nCut][nCharge];
TH1F*                         hPtWLepton   [nChannel][nCut][nCharge];
TH1F*                         hDRWZLepton1 [nChannel][nCut][nCharge];
TH1F*                         hDRWZLepton2 [nChannel][nCut][nCharge];
TH1F*                         hMtW         [nChannel][nCut][nCharge];
TH1F*                         hNJet30      [nChannel][nCut][nCharge];
TH1F*                         hNBJet30     [nChannel][nCut][nCharge];

TH1F*                         hInvMass2LepBB[2];
TH1F*                         hInvMass2LepBE[2];
TH1F*                         hInvMass2LepEE[2];

std::vector<Lepton>           AnalysisLeptons;
std::vector<TLorentzVector>   SelectedJets;

TLorentzVector                EventMET;
TLorentzVector                WLepton;
TLorentzVector                ZLepton1;
TLorentzVector                ZLepton2;

Bool_t                        isData;

Float_t                       invMass2Lep;
Float_t                       invMass3Lep;
Float_t                       sumCharges;
Float_t                       transverseMass;
Float_t                       WCharge;

UInt_t                        nElectron;
UInt_t                        nTight;
UInt_t                        theChannel;

TString                       directory;

TFile*                        root_output;
ofstream                      txt_output;


// Input parameters
//------------------------------------------------------------------------------
TString                       _sample;
Int_t                         _systematic;
Int_t                         _mode;


// Weights;
//------------------------------------------------------------------------------
Float_t                       efficiency_weight;
Float_t                       dd_weight;
Float_t                       mc_lepton_weight;
Float_t                       mc_trigger_weight;
Float_t                       mc_total_weight;
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
Int_t                         run;
Float_t                       baseW;
Float_t                       channel;
Float_t                       chmet;
Float_t                       dataset;
Float_t                       fakeW;
Float_t                       nbjet;
Float_t                       njet;
Float_t                       nextra;
Float_t                       nvtx;
Float_t                       pfmet;
Float_t                       pfmetphi;
Float_t                       puW;
Float_t                       trigger;
Float_t                       bdt          [nobjects];
Float_t                       ch           [nobjects];
Float_t                       eta          [nobjects];
Float_t                       jeteta       [nobjects];
Float_t                       jetphi       [nobjects];
Float_t                       jetpt        [nobjects];
Int_t                         pass2012ICHEP[nobjects];
Float_t                       phi          [nobjects];
Float_t                       pt           [nobjects];


//==============================================================================
//
// AnalysisWZ
//
//==============================================================================
void AnalysisWZ(TString sample,
		Int_t   systematic,
		Int_t   mode)
{
  _sample     = sample;
  _systematic = systematic;
  _mode       = mode;

  directory = "results";

  if      (_systematic == noSyst)           directory += "/analysis";
  else if (_systematic == metUpSyst)        directory += "/systematics/metUp";
  else if (_systematic == metDownSyst)      directory += "/systematics/metDown";
  else if (_systematic == muonUpSyst)       directory += "/systematics/muonUp";
  else if (_systematic == muonDownSyst)     directory += "/systematics/muonDown";
  else if (_systematic == electronUpSyst)   directory += "/systematics/electronUp";
  else if (_systematic == electronDownSyst) directory += "/systematics/electronDown";

  gSystem->mkdir(directory, kTRUE);

  TString filename = _sample;

  if (_mode == PPF) filename += "_PPF";

  root_output = new TFile(directory + "/" + filename + ".root", "recreate");


  // Histogram definition
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();
  
  for (UInt_t i=0; i<nChannel; i++) {
    
    hFakeDifference[i] = new TH1F(Form("hFakeDifference_%d", i), "", 160, -0.4, 0.4);

    for (UInt_t j=0; j<nCut; j++) {

      for (UInt_t iCharge=0; iCharge<nCharge; iCharge++) {

	TString suffix = "_" + sChannel[i] + "_" + sCut[j] + "_" + sCharge[iCharge];
	
	for (UInt_t k=0; k<nComposition; k++) {

	  hCounterRaw[i][j][iCharge][k] = new TH1F("hCounterRaw" + suffix + "_" + sComposition[k], "", 3, 0, 3);
	  hCounterPU [i][j][iCharge][k] = new TH1F("hCounterPU"  + suffix + "_" + sComposition[k], "", 3, 0, 3);
	  hCounterEff[i][j][iCharge][k] = new TH1F("hCounterEff" + suffix + "_" + sComposition[k], "", 3, 0, 3);
	  hCounter   [i][j][iCharge][k] = new TH1F("hCounter"    + suffix + "_" + sComposition[k], "", 3, 0, 3);
	}

	hLeptonWeight [i][j][iCharge] = new TH1F("hLeptonWeight"  + suffix, "", 90, 0.75, 1.05);
	hTriggerWeight[i][j][iCharge] = new TH1F("hTriggerWeight" + suffix, "", 90, 0.75, 1.05);
	hTotalWeight  [i][j][iCharge] = new TH1F("hTotalWeight"   + suffix, "", 90, 0.75, 1.05);

	hNPV         [i][j][iCharge] = new TH1F("hNPV"          + suffix, "",  50,  0,  50);
	hMET         [i][j][iCharge] = new TH1F("hMET"          + suffix, "", 200,  0, 200);
	hSumCharges  [i][j][iCharge] = new TH1F("hSumCharges"   + suffix, "",   9, -4,   5);
	hInvMass2Lep [i][j][iCharge] = new TH1F("hInvMass2Lep"  + suffix, "", 200,  0, 200);
	hInvMass3Lep [i][j][iCharge] = new TH1F("hInvMass3Lep"  + suffix, "", 400,  0, 400);
	hPtLepton1   [i][j][iCharge] = new TH1F("hPtLepton1"    + suffix, "", 200,  0, 200);
	hPtLepton2   [i][j][iCharge] = new TH1F("hPtLepton2"    + suffix, "", 200,  0, 200);
	hPtLepton3   [i][j][iCharge] = new TH1F("hPtLepton3"    + suffix, "", 200,  0, 200);    
	hPtLeadingJet[i][j][iCharge] = new TH1F("hPtLeadingJet" + suffix, "", 200,  0, 200);    
	hDPhiZLeptons[i][j][iCharge] = new TH1F("hDPhiZLeptons" + suffix, "", 320,  0, 3.2);    
	hPtZLepton1  [i][j][iCharge] = new TH1F("hPtZLepton1"   + suffix, "", 200,  0, 200);
	hPtZLepton2  [i][j][iCharge] = new TH1F("hPtZLepton2"   + suffix, "", 200,  0, 200);
	hPtZ         [i][j][iCharge] = new TH1F("hPtZ"          + suffix, "", 400,  0, 400);
	hPtWLepton   [i][j][iCharge] = new TH1F("hPtWLepton"    + suffix, "", 200,  0, 200);    
	hDRWZLepton1 [i][j][iCharge] = new TH1F("hDRWZLepton1"  + suffix, "", 200,  0,   6);    
	hDRWZLepton2 [i][j][iCharge] = new TH1F("hDRWZLepton2"  + suffix, "", 200,  0,   6);    
	hMtW         [i][j][iCharge] = new TH1F("hMtW"          + suffix, "", 200,  0, 200);    
	hNJet30      [i][j][iCharge] = new TH1F("hNJet30"       + suffix, "",  10,  0,  10);    
	hNBJet30     [i][j][iCharge] = new TH1F("hNBJet30"      + suffix, "",  10,  0,  10);    
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

  MuonFR = LoadHistogram("MuFR_Moriond13_jet15_EWKcorr",  "FR_pT_eta_EWKcorr", "MuonFR_Jet15");
  ElecFR = LoadHistogram("EleFR_Moriond13_jet15_EWKcorr", "fakeElH2",          "ElecFR_Jet15");
  //  ElecFR = LoadHistogram("EleFR_Moriond13_jet35_EWKcorr", "fakeElH2",          "ElecFR_Jet35");

  DoubleElLead  = LoadHistogram("triggerEfficiencies", "DoubleElLead",  "DoubleElLead");
  DoubleMuLead  = LoadHistogram("triggerEfficiencies", "DoubleMuLead",  "DoubleMuLead");
  DoubleElTrail = LoadHistogram("triggerEfficiencies", "DoubleElTrail", "DoubleElTrail");
  DoubleMuTrail = LoadHistogram("triggerEfficiencies", "DoubleMuTrail", "DoubleMuTrail");


  // Electron energy scale systematics
  //----------------------------------------------------------------------------
  hScaleInEB  = GetHistogramFromGraph("hScaleInEB",  "gScaleInEB");
  hScaleOutEB = GetHistogramFromGraph("hScaleOutEB", "gScaleOutEB");
  hScaleEE    = GetHistogramFromGraph("hScaleEE",    "gScaleEE");


  // Input files
  //----------------------------------------------------------------------------
  TChain* tree = new TChain("latino", "latino");

  TString path;

  isData = false;

  if (_sample.Contains("DoubleElectron") ||
      _sample.Contains("DoubleMuon") ||
      _sample.Contains("MuEG")) isData = true;

  if (isData)
    {
      path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Data_LooseLoose/4L";  // nominal
      //      path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17/Data_LooseLoose";     // ReReco
    }
  else
    {
      path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17/MC_LooseLoose";            // ReReco
      //      path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Data_LooseLoose_fakeW/4L"; // fakes
    }

  tree->Add(path + "/latino_" + _sample + ".root");


  // Tree leaves
  //----------------------------------------------------------------------------
  tree->SetBranchAddress("run",      &run);
  tree->SetBranchAddress("baseW",    &baseW);
  tree->SetBranchAddress("channel",  &channel);
  tree->SetBranchAddress("chmet",    &chmet);
  tree->SetBranchAddress("dataset",  &dataset);
  tree->SetBranchAddress("fakeW",    &fakeW);
  tree->SetBranchAddress("nbjet",    &nbjet);
  tree->SetBranchAddress("njet",     &njet);
  tree->SetBranchAddress("nextra",   &nextra);
  tree->SetBranchAddress("nvtx",     &nvtx);
  tree->SetBranchAddress("pfmet",    &pfmet);
  tree->SetBranchAddress("pfmetphi", &pfmetphi);
  tree->SetBranchAddress("puW",      &puW);
  tree->SetBranchAddress("trigger",  &trigger);

  for (UInt_t i=0; i<nobjects; i++)
    {
      tree->SetBranchAddress(Form("bdt%d",           i+1), &bdt          [i]);
      tree->SetBranchAddress(Form("ch%d",            i+1), &ch           [i]);
      tree->SetBranchAddress(Form("eta%d",           i+1), &eta          [i]);
      tree->SetBranchAddress(Form("jeteta%d",        i+1), &jeteta       [i]);
      tree->SetBranchAddress(Form("jetphi%d",        i+1), &jetphi       [i]);
      tree->SetBranchAddress(Form("jetpt%d",         i+1), &jetpt        [i]);
      tree->SetBranchAddress(Form("pass2012ICHEP%d", i+1), &pass2012ICHEP[i]);
      tree->SetBranchAddress(Form("phi%d",           i+1), &phi          [i]);
      tree->SetBranchAddress(Form("pt%d",            i+1), &pt           [i]);
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
    WCharge        = 0.;
    sumCharges     = 0.;
    nElectron      = 0;
    nTight         = 0;


    // GetEntry
    //--------------------------------------------------------------------------
    tree->GetEntry(ievent);

    if (!trigger) continue;

    if (run == 201191) continue;

    xs_weight = (1.0 + 0.6 * (dataset >= 82 && dataset <= 84)) * baseW * luminosity;

    if (isData) xs_weight = 1.0;

    Bool_t accept_WGstar = (chmet < (0.75*pt[0]+100) && chmet < (0.75*jetpt[0]+100));

    if (dataset == 82 && !accept_WGstar) continue;


    // Loop over leptons
    //--------------------------------------------------------------------------
    AnalysisLeptons.clear();

    for (UInt_t i=0; i<nobjects; i++) {

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
	  lep.fr    = GetFactor(ElecFR,        spt, eta[i], 35.);
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
	  lep.fr    = GetFactor(MuonFR,        spt, eta[i], 35.);
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

    for (UInt_t i=0; i<nobjects; i++) {

      if (jetpt[i] <= 0.) continue;

      TLorentzVector tlv;

      tlv.SetPtEtaPhiM(jetpt[i], jeteta[i], jetphi[i], 0);

      SelectedJets.push_back(tlv);
    }


    // Set the MET of the event
    //--------------------------------------------------------------------------
    EventMET = GetMET();


    // MET systematic uncertainty
    //--------------------------------------------------------------------------
    if (_systematic == metUpSyst || _systematic == metDownSyst)
      {
	TLorentzVector approxMET;
	TLorentzVector smearMET;

	for (UInt_t i=0; i<SelectedJets.size(); i++)
	  {
	    Float_t scale = METJETSPTSYS;

	    if (_systematic == metDownSyst) scale = -scale;

	    TLorentzVector Jet = SelectedJets[i];
	    
	    if (Jet.Pt() <= 30. || fabs(Jet.Eta() >= 5.))
	      {
		approxMET -= Jet;
		
		Jet.SetPx(Jet.Px() * (1. + scale));
		Jet.SetPy(Jet.Py() * (1. + scale));
		
		smearMET -= Jet;

		continue;
	      }

	    Bool_t leptoninsideJet = false;

	    for (UInt_t j=0; j<AnalysisLeptons.size(); j++)
	      {
		if (fabs(Jet.DeltaR(AnalysisLeptons[j].v)) <= 0.3)
		  {
		    leptoninsideJet = true;

		    break;
		  }
	      }

	    if (leptoninsideJet) continue;

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


    ////////////////////////////////////////////////////////////////////////////
    //
    // Recompute fakes weight
    //
    ////////////////////////////////////////////////////////////////////////////
    if (0)
      {
	Float_t dilepton = 999.;

	if      (AnalysisLeptons[0].flavor == Muon     && AnalysisLeptons[1].flavor == Muon)     dilepton = 0.;
	else if (AnalysisLeptons[0].flavor == Electron && AnalysisLeptons[1].flavor == Electron) dilepton = 1.;
	else if (AnalysisLeptons[0].flavor == Electron && AnalysisLeptons[1].flavor == Muon)     dilepton = 2.;
	else if (AnalysisLeptons[0].flavor == Muon     && AnalysisLeptons[1].flavor == Electron) dilepton = 3.;
	
	hFakeDifference[Int_t(dilepton)]->Fill(FakeWeight() - fakeW);

	if (fabs(dilepton - channel) > 0)
	  printf(" dilepton: %.0f, channel: %.0f\n", dilepton, channel);
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

    Bool_t foundDileptonWithLowMll = false;

    for (UInt_t i=0; i<AnalysisLeptons.size(); i++) {

      for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++) {
      
	if (AnalysisLeptons[i].flavor != AnalysisLeptons[j].flavor) continue;

	if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0.) continue;

	Float_t inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

	if (inv_mass < 12.) foundDileptonWithLowMll = true;

	if (fabs(inv_mass - Z_MASS) < fabs(invMass2Lep - Z_MASS)) {

	  invMass2Lep = inv_mass;

	  ZLepton1 = AnalysisLeptons[i].v;
	  ZLepton2 = AnalysisLeptons[j].v;
	  
	  isZee = (AnalysisLeptons[i].flavor == Electron) ? true : false;
	  isZmm = (AnalysisLeptons[i].flavor == Muon)     ? true : false;

	  if (AnalysisLeptons.size() != 3) continue;

	  for (UInt_t k=0; k<3; k++) {
	
	    if (k == i) continue;
	    if (k == j) continue;
	    
	    WLepton = AnalysisLeptons[k].v;

	    WCharge = AnalysisLeptons[k].charge;
	  }
	}
      }
    }

    //    if (foundDileptonWithLowMll) continue;

    if (!isZee && !isZmm) continue;

    if (fabs(WCharge) < 1.) continue;


    // Fill Z invariant mass at two-lepton level
    //--------------------------------------------------------------------------
    UInt_t index = (isZee) ? Electron : Muon;

    if (fabs(ZLepton1.Eta()) < 1.479 && fabs(ZLepton2.Eta()) < 1.479)
      {
	hInvMass2LepBB[index]->Fill(invMass2Lep, puW * xs_weight);
      }
    else if (fabs(ZLepton1.Eta()) < 1.479 || fabs(ZLepton2.Eta()) < 1.479)
      {
	hInvMass2LepBE[index]->Fill(invMass2Lep, puW * xs_weight);
      }
    else
      {
	hInvMass2LepEE[index]->Fill(invMass2Lep, puW * xs_weight);
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

    if      (nElectron == 0) theChannel = MMM;
    else if (nElectron == 1) theChannel = MME;
    else if (nElectron == 2) theChannel = EEM;
    else if (nElectron == 3) theChannel = EEE;


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
    const Float_t lWEt = WLepton.Et();

    transverseMass = lWEt*lWEt + met*met - 2.*lWEt*met*cos(WLepton.Angle(EventMET.Vect()));

    transverseMass = sqrt(transverseMass);


    // Fill histograms
    //--------------------------------------------------------------------------
    FillHistograms(theChannel, Exactly3Leptons);

    if (ZLepton1.Pt() <= 20.) continue;

    if (fabs(invMass2Lep - Z_MASS) >= 20.) continue;
    
    FillHistograms(theChannel, HasZ);

    if (WLepton.DeltaR(ZLepton1) <= 0.1) continue;
    if (WLepton.DeltaR(ZLepton2) <= 0.1) continue;

    if (fabs(invMass2Lep - Z_MASS) < 15. && EventMET.Et() < 20.)
      {
	FillHistograms(theChannel, ClosureTest);
      }

    if (WLepton.Pt() <= 20.) continue;

    if (EventMET.Et() <= 30.) continue;

    FillHistograms(theChannel, MET30);

    if (nbjet > 0) continue;

    FillHistograms(theChannel, MET30AntiBtag);
  }


  //============================================================================
  //
  // Summary
  //
  //============================================================================
  txt_output.open(Form("%s/%s.txt", directory.Data(), filename.Data()));

  txt_output << Form("\n %39s results with %7.1f pb\n", filename.Data(), luminosity);

  CounterSummary("No weights");
  CounterSummary("All weights");

  txt_output.close();

  root_output->cd();
  root_output->Write("", TObject::kOverwrite);
  root_output->Close();
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void FillHistograms(UInt_t iChannel, UInt_t iCut)
{
  Float_t hweight  = puW * efficiency_weight * xs_weight * dd_weight;
  Float_t deltaPhi = ZLepton1.DeltaPhi(ZLepton2);
  Float_t deltaR1  = WLepton.DeltaR(ZLepton1);
  Float_t deltaR2  = WLepton.DeltaR(ZLepton2);

  for (UInt_t iCharge=0; iCharge<nCharge; iCharge++)
    {
      if (iCharge == WPlus  && WCharge < 0.) continue;
      if (iCharge == WMinus && WCharge > 0.) continue;


      // Counters
      //------------------------------------------------------------------------
      hCounterRaw[iChannel][iCut][iCharge][nTight]->Fill(1);
      hCounterPU [iChannel][iCut][iCharge][nTight]->Fill(1,       efficiency_weight * xs_weight * dd_weight);
      hCounterEff[iChannel][iCut][iCharge][nTight]->Fill(1, puW * efficiency_weight             * dd_weight);
      hCounter   [iChannel][iCut][iCharge][nTight]->Fill(1, puW * efficiency_weight * xs_weight * dd_weight);

      hCounterRaw[iChannel][iCut][iCharge][LLL]->Fill(1);
      hCounterPU [iChannel][iCut][iCharge][LLL]->Fill(1,       efficiency_weight * xs_weight * dd_weight);
      hCounterEff[iChannel][iCut][iCharge][LLL]->Fill(1, puW * efficiency_weight             * dd_weight);
      hCounter   [iChannel][iCut][iCharge][LLL]->Fill(1, puW * efficiency_weight * xs_weight * dd_weight);


      // MC weight histograms
      //------------------------------------------------------------------------
      hLeptonWeight [theChannel][iCut][iCharge]->Fill(mc_lepton_weight,  puW);
      hTriggerWeight[theChannel][iCut][iCharge]->Fill(mc_trigger_weight, puW);
      hTotalWeight  [theChannel][iCut][iCharge]->Fill(mc_total_weight,   puW);


      // Analysis histograms
      //------------------------------------------------------------------------
      hNPV         [iChannel][iCut][iCharge]->Fill(nvtx,                       hweight);
      hMET         [iChannel][iCut][iCharge]->Fill(EventMET.Et(),              hweight);
      hSumCharges  [iChannel][iCut][iCharge]->Fill(sumCharges,                 hweight);
      hInvMass2Lep [iChannel][iCut][iCharge]->Fill(invMass2Lep,                hweight);
      hInvMass3Lep [iChannel][iCut][iCharge]->Fill(invMass3Lep,                hweight);
      hPtLepton1   [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[0].v.Pt(),  hweight);
      hPtLepton2   [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[1].v.Pt(),  hweight);
      hPtLepton3   [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[2].v.Pt(),  hweight);
      hPtLeadingJet[iChannel][iCut][iCharge]->Fill(jetpt[0],                   hweight);
      hDPhiZLeptons[iChannel][iCut][iCharge]->Fill(fabs(deltaPhi),             hweight);
      hPtZLepton1  [iChannel][iCut][iCharge]->Fill(ZLepton1.Pt(),              hweight);
      hPtZLepton2  [iChannel][iCut][iCharge]->Fill(ZLepton2.Pt(),              hweight);
      hPtZ         [iChannel][iCut][iCharge]->Fill((ZLepton1 + ZLepton2).Pt(), hweight);
      hPtWLepton   [iChannel][iCut][iCharge]->Fill(WLepton.Pt(),               hweight);
      hDRWZLepton1 [iChannel][iCut][iCharge]->Fill(deltaR1,                    hweight);
      hDRWZLepton2 [iChannel][iCut][iCharge]->Fill(deltaR2,                    hweight);
      hMtW         [iChannel][iCut][iCharge]->Fill(transverseMass,             hweight);
      hNJet30      [iChannel][iCut][iCharge]->Fill(njet,                       hweight);
      hNBJet30     [iChannel][iCut][iCharge]->Fill(nbjet,                      hweight);
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
  TString path = "/nfs/fanae/user/piedra/work/WZ/AuxiliaryFilesWZXS8TeV/";

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
  TString path = "/nfs/fanae/user/piedra/work/WZ/AuxiliaryFilesWZXS8TeV/";

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
// FakeWeight
//------------------------------------------------------------------------------
Float_t FakeWeight()
{
  Float_t promptProbability[2];
  Float_t fakeProbability[2];

  Int_t tightCounter = 0;

  for (UInt_t i=0; i<2; i++) {
    
    Float_t f = AnalysisLeptons[i].fr;

    Float_t p = AnalysisLeptons[i].pr;

    if (AnalysisLeptons[i].type == Tight)
      {
	tightCounter++;

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

  //  Float_t PP_weight = promptProbability[0] * promptProbability[1];
  Float_t PF_weight = promptProbability[0] * fakeProbability[1] + fakeProbability[0] * promptProbability[1];
  Float_t FF_weight = fakeProbability[0]   * fakeProbability[1];

  //  Int_t PP_sign = (abs(tightCounter - 2) % 2) ? -1 : 1;
  Int_t PF_sign = (abs(tightCounter - 1) % 2) ? -1 : 1;
  Int_t FF_sign = (abs(tightCounter - 0) % 2) ? -1 : 1;

  Float_t fakes_weight = 0;

  //  fakes_weight += (PP_sign * PP_weight);
  fakes_weight += (PF_sign * PF_weight);
  fakes_weight += (FF_sign * FF_weight);

  return fakes_weight;
}


//------------------------------------------------------------------------------
// GetMET
//------------------------------------------------------------------------------
TLorentzVector GetMET()
{
  Float_t px = pfmet * cos(pfmetphi);
  Float_t py = pfmet * sin(pfmetphi);

  TLorentzVector metv(px, py, 0., pfmet);

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

  txt_output << Form("\n %23s %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  for (UInt_t i=0; i<nCut; i++) {
      
    for (UInt_t k=nComposition; k-- > 0; ) {

      TString cut = (k == nComposition-1) ? sCut[i] : "";

      txt_output << Form(" %19s", cut.Data());

      for (UInt_t j=0; j<nChannel; j++) {

	TString composition = (j == 0) ? sComposition[k] + " " : "";

	Float_t integral = hCounter[j][i][WInclusive][k]->Integral();

	if      (title.Contains("No")) integral = hCounterRaw[j][i][WInclusive][k]->Integral();
	else if (title.Contains("PU")) integral = hCounterPU [j][i][WInclusive][k]->Integral();

	txt_output << Form(" %s%10.0f", composition.Data(), integral);
      }
      
      txt_output << "\n";
    }

    txt_output << "\n";
  }
}
