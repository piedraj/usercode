#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TSystem.h"
#include "TTree.h"
#include <iomanip>
#include <iostream>


// Constants
//------------------------------------------------------------------------------
const Bool_t runAtOviedo = true;

const Double_t ZMASS = 91.1876;  // GeV

const UInt_t numberMetCuts = 5;

const Double_t MetCut[numberMetCuts] = {20, 25, 30, 45, 1000};  // GeV

const UInt_t nLevels = 11;

enum {
  TriggerLevel,
  MetCutLevel,
  LowMinvLevel,
  ZVetoLevel,
  pMetCutLevel,
  JetVetoLevel,
  DeltaPhiJetLevel,
  SoftMuVetoLevel,
  ExtraLeptonLevel,
  PtllLevel,
  TopTaggingLevel
};

const TString sLevel[nLevels] = {
  "Trigger",
  "MetCut",
  "LowMinv",
  "ZVeto",
  "pMetCut",
  "JetVeto",
  "DeltaPhiJet",
  "SoftMuVeto",
  "ExtraLepton",
  "Ptll",
  "TopTagging"
};


// Declare functions and variables for computing fakes weight
//------------------------------------------------------------------------------
TH2F*          LoadHistogram        (TString filename,
				     TString hname,
				     TString cname);

Float_t        GetFactor            (TH2F*   h2,
				     Float_t leptonPt,
				     Float_t leptonEta,
				     Float_t leptonPtMax = -999.);

enum {Muon, Electron};

enum {Tight, Fail};

UInt_t  lepton_flavor     [2];  // Muon, Electron
UInt_t  lepton_type       [2];  // Tight, Fail
Float_t lepton_fake_rate  [2];
Float_t lepton_prompt_rate[2];


// Member functions
//------------------------------------------------------------------------------
void           FillHistograms       (UInt_t  level,
				     UInt_t  check);


// Data members
//------------------------------------------------------------------------------
TH1F*   hWeff           [nLevels];
TH1F*   hW              [nLevels];
TH1F*   hPtLepton1      [nLevels];
TH1F*   hPtLepton2      [nLevels];
TH1F*   hPtDiLepton     [nLevels];
TH1F*   hMinv           [nLevels];
TH1F*   hMt             [nLevels];
TH1F*   hNJets30        [nLevels];
TH1F*   hpfMet          [nLevels];
TH1F*   hppfMet         [nLevels];
TH1F*   hchMet          [nLevels];
TH1F*   hpchMet         [nLevels];
TH1F*   hpminMet        [nLevels];
TH1F*   hDeltaRLeptons  [nLevels];
TH1F*   hDeltaPhiLeptons[nLevels];
TH1F*   hDPhiPtllJet    [nLevels];

TH1F*   hWeff_NM1           [nLevels];
TH1F*   hW_NM1              [nLevels];
TH1F*   hPtLepton1_NM1      [nLevels];
TH1F*   hPtLepton2_NM1      [nLevels];
TH1F*   hPtDiLepton_NM1     [nLevels];
TH1F*   hMinv_NM1           [nLevels];
TH1F*   hMt_NM1             [nLevels];
TH1F*   hNJets30_NM1        [nLevels];
TH1F*   hpfMet_NM1          [nLevels];
TH1F*   hppfMet_NM1         [nLevels];
TH1F*   hchMet_NM1          [nLevels];
TH1F*   hpchMet_NM1         [nLevels];
TH1F*   hpminMet_NM1        [nLevels];
TH1F*   hDeltaRLeptons_NM1  [nLevels];
TH1F*   hDeltaPhiLeptons_NM1[nLevels];
TH1F*   hDPhiPtllJet_NM1    [nLevels];

Float_t efficiencyW;
Float_t totalW;


// Tree variables
//------------------------------------------------------------------------------
Float_t bdt1;
Float_t bdt2;
Float_t eta1;
Float_t eta2;
Float_t pass2012ICHEP1;
Float_t pass2012ICHEP2;

Float_t baseW;
Float_t channel;
Float_t chmet;
Float_t dataset;
Float_t dphill;
Float_t dphilljet;
Float_t dphilljetjet;
Float_t drll;
Float_t effW;
Float_t fakeW;
Float_t jeteta1;
Float_t jeteta2;
Float_t jeteta3;
Float_t jetpt1;
Float_t jetpt2;
Float_t jetpt3;
Float_t jettche1;
Float_t jettche2;
Float_t mctruth;
Float_t mll;
Float_t mpmet;
Float_t mth;
Float_t nbjet;
Float_t nbjettche;
Float_t nextra;
Float_t njet;
Float_t nvtx;
Float_t pchmet;
Float_t pfmet;
Float_t ppfmet;
Float_t pt1;
Float_t pt2;
Float_t puW;
Float_t ch1;
Float_t ch2;
Float_t ptll;
Float_t softtche;
Float_t trigger;
Float_t triggW;
Int_t   bveto;
Int_t   bveto_ip;
Int_t   bveto_mu;
Int_t   bveto_nj30;
Int_t   dphiveto;
Int_t   sameflav;
Int_t   zveto;
UInt_t  event;
UInt_t  lumi;
UInt_t  run;


//------------------------------------------------------------------------------
// LatinosTreeScript
//------------------------------------------------------------------------------
void LatinosTreeScript(Float_t luminosity,
		       Int_t   jetChannel,
		       TString flavorChannel,
		       TString theSample,
		       Bool_t  verbose)
{
  TH1::SetDefaultSumw2();

  TString path = Form("rootfiles/%djet/%s/", jetChannel, flavorChannel.Data());

  gSystem->mkdir(path, kTRUE);

  TFile* output = new TFile(path + theSample + ".root", "recreate");


  // Histograms
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nLevels; i++) {
    hWeff           [i] = new TH1F("hWeff"            + sLevel[i], "",   3, 0,   3);
    hW              [i] = new TH1F("hW"               + sLevel[i], "",   3, 0,   3);
    hPtLepton1      [i] = new TH1F("hPtLepton1"       + sLevel[i], "", 200, 0, 200);
    hPtLepton2      [i] = new TH1F("hPtLepton2"       + sLevel[i], "", 200, 0, 200);
    hPtDiLepton     [i] = new TH1F("hPtDiLepton"      + sLevel[i], "", 200, 0, 200);
    hMinv           [i] = new TH1F("hMinv"            + sLevel[i], "", 200, 0, 200);
    hMt             [i] = new TH1F("hMt"              + sLevel[i], "", 250, 0, 250);
    hNJets30        [i] = new TH1F("hNJetsPF30"       + sLevel[i], "",  10, 0,  10);
    hpfMet          [i] = new TH1F("hpfMet"           + sLevel[i], "", 150, 0, 150);
    hppfMet         [i] = new TH1F("hppfMet"          + sLevel[i], "", 150, 0, 150);
    hchMet          [i] = new TH1F("hchMet"           + sLevel[i], "", 150, 0, 150);
    hpchMet         [i] = new TH1F("hpchMet"          + sLevel[i], "", 150, 0, 150);
    hpminMet        [i] = new TH1F("hpminMet"         + sLevel[i], "", 150, 0, 150);
    hDeltaRLeptons  [i] = new TH1F("hDeltaRLeptons"   + sLevel[i], "",  50, 0,   5);
    hDeltaPhiLeptons[i] = new TH1F("hDeltaPhiLeptons" + sLevel[i], "",  32, 0, 3.2);
    hDPhiPtllJet    [i] = new TH1F("hDPhiPtllJet"     + sLevel[i], "",  32, 0, 3.2);

    hWeff_NM1           [i] = new TH1F("hWeff"            + sLevel[i] + "_NM1", "",   3, 0,   3);
    hW_NM1              [i] = new TH1F("hW"               + sLevel[i] + "_NM1", "",   3, 0,   3);
    hPtLepton1_NM1      [i] = new TH1F("hPtLepton1"       + sLevel[i] + "_NM1", "", 200, 0, 200);
    hPtLepton2_NM1      [i] = new TH1F("hPtLepton2"       + sLevel[i] + "_NM1", "", 200, 0, 200);
    hPtDiLepton_NM1     [i] = new TH1F("hPtDiLepton"      + sLevel[i] + "_NM1", "", 200, 0, 200);
    hMinv_NM1           [i] = new TH1F("hMinv"            + sLevel[i] + "_NM1", "", 200, 0, 200);
    hMt_NM1             [i] = new TH1F("hMt"              + sLevel[i] + "_NM1", "", 250, 0, 250);
    hNJets30_NM1        [i] = new TH1F("hNJetsPF30"       + sLevel[i] + "_NM1", "",  10, 0,  10);
    hpfMet_NM1          [i] = new TH1F("hpfMet"           + sLevel[i] + "_NM1", "", 150, 0, 150);
    hppfMet_NM1         [i] = new TH1F("hppfMet"          + sLevel[i] + "_NM1", "", 150, 0, 150);
    hchMet_NM1          [i] = new TH1F("hchMet"           + sLevel[i] + "_NM1", "", 150, 0, 150);
    hpchMet_NM1         [i] = new TH1F("hpchMet"          + sLevel[i] + "_NM1", "", 150, 0, 150);
    hpminMet_NM1        [i] = new TH1F("hpminMet"         + sLevel[i] + "_NM1", "", 150, 0, 150);
    hDeltaRLeptons_NM1  [i] = new TH1F("hDeltaRLeptons"   + sLevel[i] + "_NM1", "",  50, 0,   5);
    hDeltaPhiLeptons_NM1[i] = new TH1F("hDeltaPhiLeptons" + sLevel[i] + "_NM1", "",  32, 0, 3.2);
    hDPhiPtllJet_NM1    [i] = new TH1F("hDPhiPtllJet"     + sLevel[i] + "_NM1", "",  32, 0, 3.2);
  }


  // Data-driven methods: Z+jets
  //----------------------------------------------------------------------------
  TH1F* hNinZevents     [numberMetCuts];
  TH1F* hNoutZevents    [numberMetCuts];
  TH1F* hNinLooseZevents[numberMetCuts];
  TH1F* hMassInZevents  [numberMetCuts];
  TH1F* hMassOutZevents [numberMetCuts];

  for (size_t nC=0; nC<numberMetCuts; nC++) {
    hNinZevents     [nC] = new TH1F(Form("hNinZevents%.2f",      MetCut[nC]), "",   3, 0,   3);
    hNoutZevents    [nC] = new TH1F(Form("hNoutZevents%.2f",     MetCut[nC]), "",   3, 0,   3);
    hNinLooseZevents[nC] = new TH1F(Form("hNinLooseZevents%.2f", MetCut[nC]), "",   3, 0,   3);
    hMassInZevents  [nC] = new TH1F(Form("hMassInZevents%.2f",   MetCut[nC]), "", 200, 0, 200);
    hMassOutZevents [nC] = new TH1F(Form("hMassOutZevents%.2f",  MetCut[nC]), "", 200, 0, 200);
  }


  // Data-driven methods: Top
  //----------------------------------------------------------------------------
  TH1F* hTopTaggedEvents            = new TH1F("hTopTaggedEvents",            "", 3, 0, 3);
  TH1F* hNTopControlRegion          = new TH1F("hNTopControlRegion",          "", 3, 0, 3);
  TH1F* hNTopTaggedTopControlRegion = new TH1F("hNTopTaggedTopControlRegion", "", 3, 0, 3);

  TH1F* hbTagDisTopTaggedEvents            = new TH1F("hbTagDisTopTaggedEvents",            "", 300, -10, 20);
  TH1F* hbTagDisNTopControlRegion          = new TH1F("hbTagDisNTopControlRegion",          "", 300, -10, 20);
  TH1F* hbTagDisNTopTaggedTopControlRegion = new TH1F("hbTagDisNTopTaggedTopControlRegion", "", 300, -10, 20);


  //----------------------------------------------------------------------------
  // Input files
  //----------------------------------------------------------------------------
  TString filesPath;

  if (runAtOviedo) filesPath = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/";
  else             filesPath = "/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/";

  TChain* tree = new TChain("latino", "latino");

  if (theSample == "DataRun2012_Total") {
    tree->Add(filesPath + "Data_TightTight/2L/latino_RunA_892pbinv.root");
    tree->Add(filesPath + "Data_TightTight/2L/latino_RunB_4404pbinv.root");
    tree->Add(filesPath + "Data_TightTight/2L/latino_RunC_7032pbinv.root");
    tree->Add(filesPath + "Data_TightTight/2L/latino_RunD_7274pbinv.root");
  }
  else if (theSample == "WJetsFakes_Total") {
    tree->Add(filesPath + "Moriond13/latino_LooseLoose_19.5fb.root");
  }
  else if (theSample == "ggWWto2L") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_001_GluGluToWWTo4L.root");
  }
  else if (theSample == "WWTo2L2Nu") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_000_WWJets2LMad.root");
  }
  else if (theSample == "WZ") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_074_WZJetsMad.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_078_WZTo2L2QMad.root");
  }
  else if (theSample == "ZZ") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_075_ZZJetsMad.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_079_ZZTo2L2QMad.root"); 
  }
  else if (theSample == "TTbar") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_019_TTTo2L2Nu2B.root");
  }
  else if (theSample == "TW") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_011_TtWFullDR.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_012_TbartWFullDR.root");
  }
  else if (theSample == "DY") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_036_DY10toLLMad.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_037_DY50toLLMad.root");
  }
  else if (theSample == "DYtautau") {
    tree->Add(filesPath + "MoriondeffW/TauTau/latino_DYtt_19.5fb.root");
  }
  else if (theSample == "WgammaNoStar") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_085_WgammaToLNuG.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_008_GamGamWW.root");
  }
  else if (theSample == "WgammaStar") {
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_082_WGstarToElNuMad.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_083_WGstarToMuNuMad.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_084_WGstarToTauNuMad.root");
  }
  else if (theSample == "HWW125") { 
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_1125_ggToH125toWWTo2LAndTau2Nu.root");
    tree->Add(filesPath + "MoriondeffW/MC_TightTight_DABCABC/latino_2125_vbfToH125toWWTo2LAndTau2Nu.root");
  }
  else if (theSample == "Zgamma") { 
    tree->Add(filesPath +"MoriondeffW/MC_TightTight_DABCABC/latino_086_ZgammaToLLuG.root");
  }
  else {
    return;
  }


  // Declaration of leaf types
  //----------------------------------------------------------------------------
  tree->SetBranchAddress("bdt1",           &bdt1);
  tree->SetBranchAddress("bdt2",           &bdt2);
  tree->SetBranchAddress("eta1",           &eta1);
  tree->SetBranchAddress("eta2",           &eta2);
  tree->SetBranchAddress("pass2012ICHEP1", &pass2012ICHEP1);
  tree->SetBranchAddress("pass2012ICHEP2", &pass2012ICHEP2);

  tree->SetBranchAddress("baseW",        &baseW);
  tree->SetBranchAddress("channel",      &channel);
  tree->SetBranchAddress("chmet",        &chmet);
  tree->SetBranchAddress("dataset",      &dataset);
  tree->SetBranchAddress("dphill",       &dphill);
  tree->SetBranchAddress("dphilljet",    &dphilljet);
  tree->SetBranchAddress("dphilljetjet", &dphilljetjet);
  tree->SetBranchAddress("drll",         &drll);
  tree->SetBranchAddress("effW",         &effW);
  tree->SetBranchAddress("jeteta1",      &jeteta1);
  tree->SetBranchAddress("jeteta2",      &jeteta2);
  tree->SetBranchAddress("jeteta3",      &jeteta3);
  tree->SetBranchAddress("jetpt1",       &jetpt1);
  tree->SetBranchAddress("jetpt2",       &jetpt2);
  tree->SetBranchAddress("jetpt3",       &jetpt3);
  tree->SetBranchAddress("jettche1",     &jettche1);
  tree->SetBranchAddress("jettche2",     &jettche2);
  tree->SetBranchAddress("mctruth",      &mctruth);
  tree->SetBranchAddress("mll",          &mll);
  tree->SetBranchAddress("mpmet",        &mpmet); 
  tree->SetBranchAddress("mth",          &mth);
  tree->SetBranchAddress("nbjet",        &nbjet);
  tree->SetBranchAddress("nbjettche",    &nbjettche);
  tree->SetBranchAddress("nextra",       &nextra);
  tree->SetBranchAddress("njet",         &njet);
  tree->SetBranchAddress("nvtx",         &nvtx);
  tree->SetBranchAddress("pchmet",       &pchmet);
  tree->SetBranchAddress("pfmet",        &pfmet);
  tree->SetBranchAddress("ppfmet",       &ppfmet);
  tree->SetBranchAddress("pt1",          &pt1);
  tree->SetBranchAddress("pt2",          &pt2);
  tree->SetBranchAddress("ch1",          &ch1);
  tree->SetBranchAddress("ch2",          &ch2);
  tree->SetBranchAddress("ptll",         &ptll);
  tree->SetBranchAddress("softtche",     &softtche);
  tree->SetBranchAddress("trigger",      &trigger);
  tree->SetBranchAddress("triggW",       &triggW);
  tree->SetBranchAddress("bveto",        &bveto);
  tree->SetBranchAddress("bveto_ip",     &bveto_ip);
  tree->SetBranchAddress("bveto_mu",     &bveto_mu);
  tree->SetBranchAddress("bveto_nj30",   &bveto_nj30);
  tree->SetBranchAddress("dphiveto",     &dphiveto);
  tree->SetBranchAddress("sameflav",     &sameflav);
  tree->SetBranchAddress("zveto",        &zveto);
  tree->SetBranchAddress("event",        &event);
  tree->SetBranchAddress("lumi",         &lumi);
  tree->SetBranchAddress("run",          &run);

  if (theSample.Contains("WJetsFakes"))
    tree->SetBranchAddress("fakeW", &fakeW);
  
  if (!theSample.Contains("WJetsFakes") && !theSample.Contains("Data"))
    tree->SetBranchAddress("puW", &puW);


  // Set the channel
  //----------------------------------------------------------------------------
  Float_t SelectedChannel = -999;

  if      (flavorChannel == "MuMu") SelectedChannel =  0;
  else if (flavorChannel == "EE"  ) SelectedChannel =  1;
  else if (flavorChannel == "EMu" ) SelectedChannel =  2;
  else if (flavorChannel == "MuE" ) SelectedChannel =  3;
  else if (flavorChannel == "All" ) SelectedChannel = -1;


  // Read fake rates and prompt rates
  //----------------------------------------------------------------------------
  TH2F* MuonPR = LoadHistogram("MuPR_Moriond13_2012",           "h2inverted",        "MuonPR");
  TH2F* ElecPR = LoadHistogram("ElePR_Moriond13_2012",          "h2inverted",        "ElecPR");
  TH2F* MuonFR = LoadHistogram("MuFR_Moriond13_jet50_EWKcorr",  "FR_pT_eta_EWKcorr", "MuonFR_Jet50");
  TH2F* ElecFR = LoadHistogram("EleFR_Moriond13_jet50_EWKcorr", "fakeElH2",          "ElecFR_Jet50");


  //----------------------------------------------------------------------------
  // Loop
  //----------------------------------------------------------------------------
  for (int ievent=0; ievent<tree->GetEntries(); ievent++) {
    
    tree->GetEntry(ievent);
    
    efficiencyW = effW * triggW;
    totalW      = -999;

    if (theSample.Contains("Data"))
      {
	totalW = 1.0;
      }
    else if (theSample.Contains("WJetsFakes"))
      {
	totalW = fakeW;
      }
    else
      {
	efficiencyW = puW * effW * triggW;

	totalW = (1 + 0.6 * (dataset >= 82 && dataset <= 84)) * baseW * efficiencyW * luminosity;
      }


    // Recompute fakes weight
    //--------------------------------------------------------------------------
    Float_t fakes_weight = -999.;

    if (theSample.Contains("WJetsFakes"))
      {
	lepton_flavor[0] = (bdt1 < 100.) ? Electron : Muon;
	lepton_flavor[1] = (bdt2 < 100.) ? Electron : Muon;
    
	lepton_type[0] = (pass2012ICHEP1) ? Tight : Fail;
	lepton_type[1] = (pass2012ICHEP2) ? Tight : Fail;

	if (lepton_flavor[0] == Electron)
	  {
	    lepton_fake_rate  [0] = GetFactor(ElecFR, pt1, eta1);
	    lepton_prompt_rate[0] = GetFactor(ElecPR, pt1, eta1);
	  }
	else
	  {
	    lepton_fake_rate  [0] = GetFactor(MuonFR, pt1, eta1, 34.);
	    lepton_prompt_rate[0] = GetFactor(MuonPR, pt1, eta1);
	  }

	if (lepton_flavor[1] == Electron)
	  {
	    lepton_fake_rate  [1] = GetFactor(ElecFR, pt2, eta2);
	    lepton_prompt_rate[1] = GetFactor(ElecPR, pt2, eta2);
	  }
	else
	  {
	    lepton_fake_rate  [1] = GetFactor(MuonFR, pt2, eta2, 34.);
	    lepton_prompt_rate[1] = GetFactor(MuonPR, pt2, eta2);
	  }

	Float_t promptProbability[2];
	Float_t fakeProbability[2];

	Int_t tightCounter = 0;

	for (UInt_t i=0; i<2; i++) {
    
	  Float_t f = lepton_fake_rate[i];

	  Float_t p = lepton_prompt_rate[i];

	  if (lepton_type[i] == Tight)
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

	//	Float_t PP_weight = promptProbability[0] * promptProbability[1];
	Float_t PF_weight = promptProbability[0] * fakeProbability[1] + fakeProbability[0] * promptProbability[1];
	Float_t FF_weight = fakeProbability[0]   * fakeProbability[1];

	//	Int_t PP_sign = (abs(tightCounter - 2) % 2) ? -1 : 1;
	Int_t PF_sign = (abs(tightCounter - 1) % 2) ? -1 : 1;
	Int_t FF_sign = (abs(tightCounter - 0) % 2) ? -1 : 1;

	fakes_weight = 0;

	//	fakes_weight += (PP_sign * PP_weight);
	fakes_weight += (PF_sign * PF_weight);
	fakes_weight += (FF_sign * FF_weight);
      }


    // Help variables
    //--------------------------------------------------------------------------
    Int_t dphiv = ((njet <= 1 && dphiveto) || (njet > 1 && dphilljetjet < 165.*TMath::DegToRad()));

    Float_t metvar = (njet <= 1) ? mpmet : pfmet;

    Float_t jetbin = njet;
	    
    if (njet == 3) jetbin = 2;

    Int_t vbfsel = ((jetpt3 <= 30 || !(jetpt3 > 30 && ((jeteta1-jeteta3 > 0 && jeteta2-jeteta3 < 0) || (jeteta2-jeteta3 > 0 && jeteta1-jeteta3 < 0)))));


    // The selection begins here
    //--------------------------------------------------------------------------
    Bool_t accept_WGstar = (chmet < (0.75*pt1+100) && chmet < (0.75*jetpt1+100));

    if ((dataset == 36 || dataset == 37) && mctruth == 2)                    continue;
    if (dataset == 82 && !accept_WGstar)                                     continue;
    if (dataset == 86 && (flavorChannel == "MuMu" || flavorChannel == "EE")) continue;
    if ((SelectedChannel != -1) && (channel != SelectedChannel))             continue;
    if (run == 201191)                                                       continue;
    if (!trigger)                                                            continue;
    if (pt2 <= 20)                                                           continue;
    if (ch1*ch2 > 0)                                                         continue;


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //
    // Data-driven methods
    //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (pfmet > 20 && mpmet > 20 && mll > 12 && ptll > 45 && nextra == 0 && (dphiv || !sameflav)) {


      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //
      // Z+jets
      //
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if (dphiv && jetbin == jetChannel && bveto_mu && (bveto_ip && (nbjettche == 0 || njet > 3))) {


	// Loop over the metvar bins
	//----------------------------------------------------------------------
	for (size_t mc=0; mc<numberMetCuts; mc ++) {
	    
	  if (metvar > MetCut[mc] && fabs(mll - ZMASS) < 7.5) {
	    hNinLooseZevents[mc]->Fill(1,totalW);
	  }

	  if (metvar > MetCut[mc] && metvar < MetCut[mc+1]) {   
	    if (fabs(mll - ZMASS) < 7.5) {
	      hNinZevents[mc]   ->Fill(  1, totalW);
	      hMassInZevents[mc]->Fill(mll, totalW);
	    }
	    else if (fabs(mll - ZMASS) > 15) {  
	      hNoutZevents[mc]   ->Fill(  1, totalW);
	      hMassOutZevents[mc]->Fill(mll, totalW);
	    }
	  }
	}
      }


      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //
      // Top
      //
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if (zveto && metvar > (20 + 25*sameflav)) {
	  
	// btag_eff denominator
	if ((jetChannel == 0 && njet == 1 && nbjet == 1) ||
	    (jetChannel == 1 && njet == 2 && jettche2 > 2.1) ||
	    (jetChannel == 2)) {
	  
	  hNTopControlRegion->Fill(1, totalW);
	  hbTagDisNTopControlRegion->Fill(jettche2, totalW);
	    
	  // btag_eff numerator
	  if ((jetChannel == 0 && !bveto_nj30) ||
	      (jetChannel == 1 && jettche1 > 2.1) ||
	      (jetChannel == 2)) {
	    
	    hNTopTaggedTopControlRegion->Fill(1, totalW);
	    hbTagDisNTopTaggedTopControlRegion->Fill(jettche2, totalW);
	  }
	}
      }
	
      // Top-tagged events for ttbar estimation
      //----------------------------------------------------------------------~~
      if (zveto && metvar > (20 + 25*sameflav)) {

	if ((jetChannel == 0 && njet == 0 && !bveto) ||
	    (jetChannel == 1 && njet == 1 && bveto && jettche1 > 2.1) ||
	    (jetChannel == 2)) {
	  
	  hTopTaggedEvents->Fill(1, totalW);
	  hbTagDisTopTaggedEvents->Fill(jettche2, totalW);
	}
      }
    }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //
    // Main analysis
    //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    for (UInt_t ilevel=0; ilevel<nLevels; ilevel++) {

      FillHistograms(TriggerLevel, ilevel);

      if ((pfmet > 20 && mpmet > 20) || (ilevel == MetCutLevel)) {
      
	FillHistograms(MetCutLevel, ilevel);

	if ((mll > 12) || (ilevel == LowMinvLevel)) {
    
	  FillHistograms(LowMinvLevel, ilevel);

	  if (zveto || (ilevel == ZVetoLevel)) {
    
	    FillHistograms(ZVetoLevel, ilevel);
	    
	    if ((metvar > (20 + 25*sameflav)) || (ilevel == pMetCutLevel)) {
    
	      FillHistograms(pMetCutLevel, ilevel);

	      if ((jetbin == jetChannel) || (ilevel == JetVetoLevel)) {
    
		FillHistograms(JetVetoLevel, ilevel);

		if (dphiv || !sameflav || (ilevel == DeltaPhiJetLevel)) {
    
		  FillHistograms(DeltaPhiJetLevel, ilevel);

		  if (bveto_mu || (ilevel == SoftMuVetoLevel)) {
    
		    FillHistograms(SoftMuVetoLevel, ilevel);

		    if ((nextra == 0) || (ilevel == ExtraLeptonLevel)) {
    
		      FillHistograms(ExtraLeptonLevel, ilevel);

		      if ((ptll > 45) || (ilevel == PtllLevel)) {
    
			FillHistograms(PtllLevel, ilevel);

			Bool_t top_veto = (bveto_ip && (nbjettche == 0 || njet > 3) && (njet <= 1 || vbfsel));
			
			if (top_veto || (ilevel == TopTaggingLevel)) {
    
			  FillHistograms(TopTaggingLevel, ilevel);
			  
			  hWeff_NM1           [ilevel]->Fill(1,         efficiencyW);
			  hW_NM1              [ilevel]->Fill(1,         totalW);
			  hPtLepton1_NM1      [ilevel]->Fill(pt1,       totalW);
			  hPtLepton2_NM1      [ilevel]->Fill(pt2,       totalW);
			  hPtDiLepton_NM1     [ilevel]->Fill(ptll,      totalW);
			  hMinv_NM1           [ilevel]->Fill(mll,       totalW);
			  hMt_NM1             [ilevel]->Fill(mth,       totalW);
			  hNJets30_NM1        [ilevel]->Fill(njet,      totalW);
			  hpfMet_NM1          [ilevel]->Fill(pfmet,     totalW);
			  hppfMet_NM1         [ilevel]->Fill(ppfmet,    totalW);
			  hchMet_NM1          [ilevel]->Fill(chmet,     totalW);
			  hpchMet_NM1         [ilevel]->Fill(pchmet,    totalW);
			  hpminMet_NM1        [ilevel]->Fill(mpmet,     totalW);
			  hDeltaRLeptons_NM1  [ilevel]->Fill(drll,      totalW);
			  hDeltaPhiLeptons_NM1[ilevel]->Fill(dphill,    totalW);
			  hDPhiPtllJet_NM1    [ilevel]->Fill(dphilljet, totalW);
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  
  // Print
  //----------------------------------------------------------------------------
  if (verbose) {
    
    printf("\n Expected number of RAW events for %s\n", theSample.Data());
    printf(" -------------------+-----------\n");

    for (UInt_t i=0; i<nLevels; i++)
      printf(" %18s | %.0f\n", sLevel[i].Data(), hW[i]->GetEntries());

    printf("\n");

    if (!theSample.Contains("Data")) {

      printf("\n Normalized to %.3f 1/fb\n", luminosity);
      printf(" -------------------+-----------\n");
      
      for (UInt_t i=0; i<nLevels; i++)
	printf(" %18s | %.0f\n", sLevel[i].Data(), hW[i]->GetSumOfWeights());
      
      printf("\n");
    }
  }


  // Save the histograms
  //----------------------------------------------------------------------------
  output->cd();
  output->Write("", TObject::kOverwrite);
  output->Close();
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void FillHistograms(UInt_t level, UInt_t check)
{
  if (check != 0) return;

  hWeff           [level]->Fill(1,         efficiencyW);
  hW              [level]->Fill(1,         totalW);
  hPtLepton1      [level]->Fill(pt1,       totalW);
  hPtLepton2      [level]->Fill(pt2,       totalW);
  hPtDiLepton     [level]->Fill(ptll,      totalW);
  hMinv           [level]->Fill(mll,       totalW);
  hMt             [level]->Fill(mth,       totalW);
  hNJets30        [level]->Fill(njet,      totalW);
  hpfMet          [level]->Fill(pfmet,     totalW);
  hppfMet         [level]->Fill(ppfmet,    totalW);
  hchMet          [level]->Fill(chmet,     totalW);
  hpchMet         [level]->Fill(pchmet,    totalW);
  hpminMet        [level]->Fill(mpmet,     totalW);
  hDeltaRLeptons  [level]->Fill(drll,      totalW);
  hDeltaPhiLeptons[level]->Fill(dphill,    totalW);
  hDPhiPtllJet    [level]->Fill(dphilljet, totalW);
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
