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
const Float_t luminosity = 19.604;  // [fb-1]


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


const UInt_t nCut = 7;

enum {
  Exactly3Leptons,
  InvMass3Lep100,
  HasZ,
  HasW,
  MET30,
  ZJetsRegion,
  TopRegion
};

const TString sCut[nCut] = {
  "Exactly3Leptons",
  "InvMass3Lep100",
  "HasZ",
  "HasW",
  "MET30",
  "ZJetsRegion",
  "TopRegion"
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

enum {Loose, Tight};

enum {RAW, PPF};


struct Lepton
{
  UInt_t         index;
  UInt_t         flavor;  // Muon, Electron
  UInt_t         type;    // Tight, Loose
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
// Histograms
//
//==============================================================================
TH1F*                         hCounterRaw[nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounterEff[nChannel][nCut][nCharge][nComposition];
TH1F*                         hCounter   [nChannel][nCut][nCharge][nComposition];

TH1F*                         hLeptonWeight   [nChannel][nCut][nCharge];
TH1F*                         hTriggerWeight  [nChannel][nCut][nCharge];
TH1F*                         hTotalWeight    [nChannel][nCut][nCharge];
TH1F*                         hNPV            [nChannel][nCut][nCharge];
TH1F*                         hMET            [nChannel][nCut][nCharge];
TH1F*                         hInvMass2Lep    [nChannel][nCut][nCharge];
TH1F*                         hInvMass3Lep    [nChannel][nCut][nCharge];
TH1F*                         hPtLepton1      [nChannel][nCut][nCharge];
TH1F*                         hPtLepton2      [nChannel][nCut][nCharge];
TH1F*                         hPtLepton3      [nChannel][nCut][nCharge];
TH1F*                         hDPhiZLeptons   [nChannel][nCut][nCharge];
TH1F*                         hPtZLepton1     [nChannel][nCut][nCharge];
TH1F*                         hPtZLepton2     [nChannel][nCut][nCharge];
TH1F*                         hPtZ            [nChannel][nCut][nCharge];
TH1F*                         hPtWLepton      [nChannel][nCut][nCharge];
TH1F*                         hPtW            [nChannel][nCut][nCharge];
TH1F*                         hMtW            [nChannel][nCut][nCharge];
TH1F*                         hNJet           [nChannel][nCut][nCharge];
TH1F*                         hNBJet          [nChannel][nCut][nCharge];
TH1F*                         hPtLeadingJet   [nChannel][nCut][nCharge];
TH1F*                         hEtaLeadingJet  [nChannel][nCut][nCharge];
TH1F*                         hPhiLeadingJet  [nChannel][nCut][nCharge];
TH1F*                         hDRLeadingJetLep[nChannel][nCut][nCharge];


//==============================================================================
//
// Data members
//
//==============================================================================
std::vector<Lepton>           AnalysisLeptons;
std::vector<TLorentzVector>   SelectedJets;

Lepton                        WLepton;
Lepton                        ZLepton1;
Lepton                        ZLepton2;
TLorentzVector                EventMET;

Bool_t                        isData;
Float_t                       dphill;
Float_t                       mll;
Float_t                       mlll;
Float_t                       mt;
Float_t                       mindrjetl;
Int_t                         njet;
Int_t                         nbjet;
UInt_t                        nelectron;
UInt_t                        ntight;
UInt_t                        channel;

TFile*                        root_output;
ofstream                      txt_output;


// Input parameters
//------------------------------------------------------------------------------
TString                       _sample;
Int_t                         _systematic;
Int_t                         _mode;
Int_t                         _jetchannel;
TString                       _directory;


// Weights;
//------------------------------------------------------------------------------
Float_t                       effW;
Float_t                       fakeW;
Float_t                       triggW;
Float_t                       xsW;
Float_t                       histW;


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
Float_t                       baseW;
Float_t                       chmet;
Float_t                       dataset;
Float_t                       nextra;
Float_t                       nvtx;
Float_t                       pfmet;
Float_t                       pfmetphi;
Float_t                       puW;
Float_t                       trigger;
Float_t                       bdt          [number_of_leptons];
Float_t                       ch           [number_of_leptons];
Float_t                       eta          [number_of_leptons];
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


//==============================================================================
//
// AnalysisWZ
//
//==============================================================================
void AnalysisWZ(TString sample,
		Int_t   systematic,
		Int_t   mode,
		Int_t   jetchannel,
		TString muonJetPt,
		TString elecJetPt,
		TString directory)
{
  TH1::SetDefaultSumw2();

  _sample     = sample;
  _systematic = systematic;
  _mode       = mode;
  _jetchannel = jetchannel;
  _directory  = directory;

  if (_jetchannel >= 0) _directory = Form("%s/%djet", _directory.Data(), _jetchannel);

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

  gSystem->mkdir(_directory, kTRUE);

  TString filename = _sample;

  if (_mode == PPF) filename += "_PPF";

  root_output = new TFile(_directory + "/" + filename + ".root", "recreate");


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
	}

	hLeptonWeight   [i][j][iCharge] = new TH1F("hLeptonWeight"    + suffix, "", 90,  0.75, 1.05);
	hTriggerWeight  [i][j][iCharge] = new TH1F("hTriggerWeight"   + suffix, "", 90,  0.75, 1.05);
	hTotalWeight    [i][j][iCharge] = new TH1F("hTotalWeight"     + suffix, "", 90,  0.75, 1.05);
	hNPV            [i][j][iCharge] = new TH1F("hNPV"             + suffix, "",  50,    0,   50);
	hMET            [i][j][iCharge] = new TH1F("hMET"             + suffix, "", 200,    0,  200);
	hInvMass2Lep    [i][j][iCharge] = new TH1F("hInvMass2Lep"     + suffix, "", 400,    0,  200);
	hInvMass3Lep    [i][j][iCharge] = new TH1F("hInvMass3Lep"     + suffix, "", 400,    0,  400);
	hPtLepton1      [i][j][iCharge] = new TH1F("hPtLepton1"       + suffix, "", 200,    0,  200);
	hPtLepton2      [i][j][iCharge] = new TH1F("hPtLepton2"       + suffix, "", 200,    0,  200);
	hPtLepton3      [i][j][iCharge] = new TH1F("hPtLepton3"       + suffix, "", 200,    0,  200);    
	hDPhiZLeptons   [i][j][iCharge] = new TH1F("hDPhiZLeptons"    + suffix, "", 320,    0,  3.2);    
	hPtZLepton1     [i][j][iCharge] = new TH1F("hPtZLepton1"      + suffix, "", 200,    0,  200);
	hPtZLepton2     [i][j][iCharge] = new TH1F("hPtZLepton2"      + suffix, "", 200,    0,  200);
	hPtZ            [i][j][iCharge] = new TH1F("hPtZ"             + suffix, "", 400,    0,  400);
	hPtWLepton      [i][j][iCharge] = new TH1F("hPtWLepton"       + suffix, "", 200,    0,  200);    
	hPtW            [i][j][iCharge] = new TH1F("hPtW"             + suffix, "", 400,    0,  400);
	hMtW            [i][j][iCharge] = new TH1F("hMtW"             + suffix, "", 200,    0,  200);    
	hNJet           [i][j][iCharge] = new TH1F("hNJet"            + suffix, "",  10,    0,   10);    
	hNBJet          [i][j][iCharge] = new TH1F("hNBJet"           + suffix, "",  10,    0,   10);
	hPtLeadingJet   [i][j][iCharge] = new TH1F("hPtLeadingJet"    + suffix, "", 200,    0,  200);
	hEtaLeadingJet  [i][j][iCharge] = new TH1F("hEtaLeadingJet"   + suffix, "", 240,   -6,    6);
	hPhiLeadingJet  [i][j][iCharge] = new TH1F("hPhiLeadingJet"   + suffix, "", 256, -3.2,  3.2);
	hDRLeadingJetLep[i][j][iCharge] = new TH1F("hDRLeadingJetLep" + suffix, "", 300,    0,    6);
      }
    }
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

  TChain* tree = new TChain("latino", "latino");

  TString path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17newJEC/";

  if (isData) path += "Data";
  else        path += "MC";

  tree->Add(path + "_LooseLooseTypeI/latino_" + _sample + ".root");


  // Tree leaves
  //----------------------------------------------------------------------------
  tree->SetBranchAddress("run",      &run);
  tree->SetBranchAddress("baseW",    &baseW);
  tree->SetBranchAddress("chmet",    &chmet);
  tree->SetBranchAddress("dataset",  &dataset);
  tree->SetBranchAddress("nextra",   &nextra);
  tree->SetBranchAddress("nvtx",     &nvtx);
  tree->SetBranchAddress("pfmet",    &pfmet);
  tree->SetBranchAddress("pfmetphi", &pfmetphi);
  tree->SetBranchAddress("puW",      &puW);
  tree->SetBranchAddress("trigger",  &trigger);

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
    }

  for (UInt_t i=0; i<number_of_jets; i++)
    {
      tree->SetBranchAddress(Form("jeteta%d",  i+1), &jeteta [i]);
      tree->SetBranchAddress(Form("jetphi%d",  i+1), &jetphi [i]);
      tree->SetBranchAddress(Form("jetpt%d",   i+1), &jetpt  [i]);
      tree->SetBranchAddress(Form("jettche%d", i+1), &jettche[i]);
      tree->SetBranchAddress(Form("jetid%d",   i+1), &jetid  [i]);
    }


  //============================================================================
  //
  // Event loop
  //
  //============================================================================
  for (int ievent=0; ievent<tree->GetEntries(); ievent++) {


    // Initialize data members
    //--------------------------------------------------------------------------
    dphill    = 999;
    mll       = 999;
    mlll      = 999;
    mt        = 999;
    mindrjetl = 999;
    njet      = 0;
    nbjet     = 0;
    nelectron = 0;
    ntight    = 0;


    // GetEntry
    //--------------------------------------------------------------------------
    tree->GetEntry(ievent);

    if (_systematic == pileupSyst) puW = 1.0;

    if (!trigger) continue;

    if (run == 201191) continue;

    xsW = (1.0 + 0.6 * (dataset >= 82 && dataset <= 84)) * baseW * luminosity;

    if (_sample.Contains("WZZJets")) xsW *= (0.01968  / 0.0192);
    if (_sample.Contains("ZZZJets")) xsW *= (0.005527 / 0.00459);
    if (_sample.Contains("WWZJets")) xsW *= (0.05795  / 0.0633);
    if (_sample.Contains("WWWJets")) xsW *= (0.08058  / 0.0822);
    if (_sample.Contains("TTWJets")) xsW *= (0.232    / 0.232);
    if (_sample.Contains("TTZJets")) xsW *= (0.2057   / 0.174);
	
    if (isData) xsW = 1.0;

    Bool_t accept_WGstar = (chmet < (0.75*pt[0]+100) && chmet < (0.75*jetpt[0]+100));

    if (dataset == 82 && !accept_WGstar) continue;


    // Set the MET of the event
    //--------------------------------------------------------------------------
    EventMET = GetMET(pfmet, pfmetphi);


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

      lep.type = (pass2012ICHEP[i]) ? Tight : Loose;

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


    // Require exactly three leptons
    //--------------------------------------------------------------------------
    if (AnalysisLeptons.size() != 3) continue;


    // Fill the selected jets vector
    //--------------------------------------------------------------------------
    SelectedJets.clear();

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
	  
	  njet++;
	  
	  if (jettche[i] > 2.1 && jetid[i] >= 4) nbjet++;  // jetid 4 = MVA LOOSE
	}
    }


    // Inclusive and {0,1,2}-jet cross sections
    //--------------------------------------------------------------------------
    if (_jetchannel >= 0 && njet != _jetchannel) continue;


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
	//	if (AnalysisLeptons[i].flavor == AnalysisLeptons[j].flavor) continue;  // Wrong-flavor study

	if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0.) continue;

	Float_t inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

	if (fabs(inv_mass - Z_MASS) < fabs(mll - Z_MASS)) {

	  mll = inv_mass;

	  ZLepton1 = AnalysisLeptons[i];
	  ZLepton2 = AnalysisLeptons[j];
	  
	  isZee = (AnalysisLeptons[i].flavor == Electron) ? true : false;
	  isZmm = (AnalysisLeptons[i].flavor == Muon)     ? true : false;

	  for (UInt_t k=0; k<3; k++) {
	
	    if (k == i) continue;
	    if (k == j) continue;

	    WLepton = AnalysisLeptons[k];
	  }
	}
      }
    }

    if (!isZee && !isZmm) continue;

    if (fabs(WLepton.charge) < 1.) continue;


    // Classify the channels
    //--------------------------------------------------------------------------
    for (UInt_t i=0; i<3; i++)
      {
	if (AnalysisLeptons[i].flavor == Electron) nelectron++;
	if (AnalysisLeptons[i].type   == Tight)    ntight++;
      }

    if (_mode == RAW && ntight != 3) continue;

    mlll = (AnalysisLeptons[0].v + AnalysisLeptons[1].v + AnalysisLeptons[2].v).M();

    dphill = ZLepton1.v.DeltaPhi(ZLepton2.v);

    if      (nelectron == 0) channel = MMM;
    else if (nelectron == 1) channel = MME;
    else if (nelectron == 2) channel = EEM;
    else if (nelectron == 3) channel = EEE;


    // Apply lepton SF and trigger efficiencies
    //--------------------------------------------------------------------------
    effW   = 1;
    triggW = 1;

    if (!isData)
      {
	for (UInt_t i=0; i<3; i++) effW *= AnalysisLeptons[i].sf;

	triggW = GetTriggerWeight();
      }


    // Data-driven estimates
    //--------------------------------------------------------------------------
    fakeW = 1;

    if (_mode == PPF)
      {
	fakeW =
	  GetDataDrivenWeight(2) +
	  GetDataDrivenWeight(1) +
	  GetDataDrivenWeight(0);
      }


    // Weight for histograms
    //--------------------------------------------------------------------------
    histW = puW * effW * triggW * xsW * fakeW;


    // Compute the transverse mass
    //--------------------------------------------------------------------------
    const Float_t met  = EventMET.Et();
    const Float_t lWEt = WLepton.v.Et();

    mt = lWEt*lWEt + met*met - 2.*lWEt*met*cos(WLepton.v.Angle(EventMET.Vect()));

    mt = sqrt(mt);


    // Compute the minimum deltaR between the leading jet and the leptons
    //--------------------------------------------------------------------------
    if (njet > 0)
      {
	Float_t theDeltaRLeadingJetLep;
      
	for (UInt_t i=0; i<3; i++)
	  {
	    theDeltaRLeadingJetLep = SelectedJets[0].DeltaR(AnalysisLeptons[i].v);
	    
	    if (theDeltaRLeadingJetLep < mindrjetl)
	      {
		mindrjetl = theDeltaRLeadingJetLep;
	      }
	  }
      }


    // Wrong-flavor study
    //--------------------------------------------------------------------------
    //    if (WLepton.flavor == ZLepton1.flavor && (WLepton.charge * ZLepton1.charge) < 0) continue;
    //    if (WLepton.flavor == ZLepton2.flavor && (WLepton.charge * ZLepton2.charge) < 0) continue;


    //==========================================================================
    //
    // Fill histograms
    //
    //==========================================================================
    FillHistograms(channel,  Exactly3Leptons);
    FillHistograms(combined, Exactly3Leptons);

    if (mlll <= 100.) continue;

    FillHistograms(channel,  InvMass3Lep100);
    FillHistograms(combined, InvMass3Lep100);

    if (ZLepton1.v.Pt() <= 20.) continue;



    // Top enriched region
    //--------------------------------------------------------------------------
    if (fabs(mll - Z_MASS) > 10. &&
	nbjet > 0 &&
	EventMET.Et() > 30.)
      {
	FillHistograms(channel,  TopRegion);
	FillHistograms(combined, TopRegion);
      }
    //--------------------------------------------------------------------------



    if (fabs(mll - Z_MASS) >= 20.) continue;

    FillHistograms(channel,  HasZ);
    FillHistograms(combined, HasZ);

    if (WLepton.v.DeltaR(ZLepton1.v) <= 0.1) continue;
    if (WLepton.v.DeltaR(ZLepton2.v) <= 0.1) continue;



    // Z+jets enriched region
    //--------------------------------------------------------------------------
    if (fabs(mll - Z_MASS) < 15. && EventMET.Et() < 20.)
      {
	FillHistograms(channel,  ZJetsRegion);
	FillHistograms(combined, ZJetsRegion);
      }
    //--------------------------------------------------------------------------



    if (WLepton.v.Pt() <= 20.) continue;

    FillHistograms(channel,  HasW);
    FillHistograms(combined, HasW);

    if (EventMET.Et() <= 30.) continue;
    
    FillHistograms(channel,  MET30);
    FillHistograms(combined, MET30);
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


  root_output->cd();

  root_output->Write("", TObject::kOverwrite);

  root_output->Close();
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void FillHistograms(UInt_t iChannel, UInt_t iCut)
{
  for (UInt_t iCharge=0; iCharge<nCharge; iCharge++)
    {
      if (iCharge == WPlus  && WLepton.charge < 0.) continue;
      if (iCharge == WMinus && WLepton.charge > 0.) continue;


      // Counters
      //------------------------------------------------------------------------
      hCounterRaw[iChannel][iCut][iCharge][ntight]->Fill(1);
      hCounterEff[iChannel][iCut][iCharge][ntight]->Fill(1, puW * effW * triggW * fakeW);
      hCounter   [iChannel][iCut][iCharge][ntight]->Fill(1, histW);

      hCounterRaw[iChannel][iCut][iCharge][LLL]->Fill(1);
      hCounterEff[iChannel][iCut][iCharge][LLL]->Fill(1, puW * effW * triggW * fakeW);
      hCounter   [iChannel][iCut][iCharge][LLL]->Fill(1, histW);


      // MC weight histograms
      //------------------------------------------------------------------------
      hLeptonWeight [channel][iCut][iCharge]->Fill(effW,          puW);
      hTriggerWeight[channel][iCut][iCharge]->Fill(triggW,        puW);
      hTotalWeight  [channel][iCut][iCharge]->Fill(effW * triggW, puW);


      // Analysis histograms
      //------------------------------------------------------------------------
      hNPV           [iChannel][iCut][iCharge]->Fill(nvtx,                           histW);
      hMET           [iChannel][iCut][iCharge]->Fill(EventMET.Et(),                  histW);
      hInvMass2Lep   [iChannel][iCut][iCharge]->Fill(mll,                            histW);
      hInvMass3Lep   [iChannel][iCut][iCharge]->Fill(mlll,                           histW);
      hPtLepton1     [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[0].v.Pt(),      histW);
      hPtLepton2     [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[1].v.Pt(),      histW);
      hPtLepton3     [iChannel][iCut][iCharge]->Fill(AnalysisLeptons[2].v.Pt(),      histW);
      hDPhiZLeptons  [iChannel][iCut][iCharge]->Fill(fabs(dphill),                   histW);
      hPtZLepton1    [iChannel][iCut][iCharge]->Fill(ZLepton1.v.Pt(),                histW);
      hPtZLepton2    [iChannel][iCut][iCharge]->Fill(ZLepton2.v.Pt(),                histW);
      hPtZ           [iChannel][iCut][iCharge]->Fill((ZLepton1.v + ZLepton2.v).Pt(), histW);
      hPtWLepton     [iChannel][iCut][iCharge]->Fill(WLepton.v.Pt(),                 histW);
      hPtW           [iChannel][iCut][iCharge]->Fill((WLepton.v + EventMET).Pt(),    histW);
      hMtW           [iChannel][iCut][iCharge]->Fill(mt,                             histW);
      hNJet          [iChannel][iCut][iCharge]->Fill(njet,                           histW);
      hNBJet         [iChannel][iCut][iCharge]->Fill(nbjet,                          histW);

      if (njet > 0)
	{
	  hPtLeadingJet   [iChannel][iCut][iCharge]->Fill(SelectedJets[0].Pt(),  histW);
	  hEtaLeadingJet  [iChannel][iCut][iCharge]->Fill(SelectedJets[0].Eta(), histW);
	  hPhiLeadingJet  [iChannel][iCut][iCharge]->Fill(SelectedJets[0].Phi(), histW);
	  hDRLeadingJetLep[iChannel][iCut][iCharge]->Fill(mindrjetl,             histW);
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
// Loose estimated as PROMPT: pf
// Loose estimated as   FAKE: pf
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
  
  Bool_t flipSign = abs((Int_t)ntight - (Int_t)nPrompt) % 2;

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

  txt_output << Form("\n %23s %13s %13s %13s %13s %13s\n",
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

	txt_output << Form(" %s%13.3f", composition.Data(), integral);
      }
      
      txt_output << "\n";
    }

    txt_output << "\n";
  }
}
