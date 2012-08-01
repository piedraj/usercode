#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TSystem.h"
#include "TTree.h"
#include <iomanip>
#include <iostream>


const Double_t ZMASS = 91.1876;

const UInt_t numberMetCuts = 5;

Double_t MetCut[] = {20, 25, 30, 45, 1000};

Bool_t runAtOviedo = true;
Bool_t runAtIfca   = !runAtOviedo;


//------------------------------------------------------------------------------
// LatinosTreeScript
//------------------------------------------------------------------------------
void LatinosTreeScript(Float_t luminosity,
		       Int_t   jetChannel,
		       TString flavorChannel,
		       TString theSample,
		       Bool_t  verbose)
{
  TString path = Form("rootfiles/%djet/%s/", jetChannel, flavorChannel.Data());

  gSystem->mkdir(path, kTRUE);

  TFile* output = new TFile(path + theSample + ".root", "recreate");


  // Counting histograms
  //----------------------------------------------------------------------------
  TH1F *hWTrigger     = new TH1F("hWTrigger",     "", 3, 0, 3);
  TH1F *hWMetCut      = new TH1F("hWMetCut",      "", 3, 0, 3);
  TH1F *hWLowMinv     = new TH1F("hWLowMinv",     "", 3, 0, 3);
  TH1F *hWZVeto       = new TH1F("hWZVeto",       "", 3, 0, 3);
  TH1F *hWpMetCut     = new TH1F("hWpMetCut",     "", 3, 0, 3);
  TH1F *hWJetVeto     = new TH1F("hWJetVeto",     "", 3, 0, 3);
  TH1F *hWDeltaPhiJet = new TH1F("hWDeltaPhiJet", "", 3, 0, 3);
  TH1F *hWSoftMuVeto  = new TH1F("hWSoftMuVeto",  "", 3, 0, 3); 
  TH1F *hWExtraLepton = new TH1F("hWExtraLepton", "", 3, 0, 3);
  TH1F *hWPtll        = new TH1F("hWPtll",        "", 3, 0, 3);
  TH1F *hWTopTagging  = new TH1F("hWTopTagging",  "", 3, 0, 3);

  hWTrigger    ->Sumw2();
  hWMetCut     ->Sumw2();
  hWLowMinv    ->Sumw2();
  hWZVeto      ->Sumw2();
  hWpMetCut    ->Sumw2();
  hWJetVeto    ->Sumw2();
  hWDeltaPhiJet->Sumw2();
  hWSoftMuVeto ->Sumw2();
  hWExtraLepton->Sumw2();
  hWPtll       ->Sumw2();
  hWTopTagging ->Sumw2();


  TH1F *hWeffTrigger     = new TH1F("hWeffTrigger",     "", 3, 0, 3);
  TH1F *hWeffMetCut      = new TH1F("hWeffMetCut",      "", 3, 0, 3);
  TH1F *hWeffLowMinv     = new TH1F("hWeffLowMinv",     "", 3, 0, 3);
  TH1F *hWeffZVeto       = new TH1F("hWeffZVeto",       "", 3, 0, 3);
  TH1F *hWeffpMetCut     = new TH1F("hWeffpMetCut",     "", 3, 0, 3);
  TH1F *hWeffJetVeto     = new TH1F("hWeffJetVeto",     "", 3, 0, 3);
  TH1F *hWeffDeltaPhiJet = new TH1F("hWeffDeltaPhiJet", "", 3, 0, 3);
  TH1F *hWeffSoftMuVeto  = new TH1F("hWeffSoftMuVeto",  "", 3, 0, 3); 
  TH1F *hWeffExtraLepton = new TH1F("hWeffExtraLepton", "", 3, 0, 3);
  TH1F *hWeffPtll        = new TH1F("hWeffPtll",        "", 3, 0, 3);
  TH1F *hWeffTopTagging  = new TH1F("hWeffTopTagging",  "", 3, 0, 3);

  hWeffTrigger    ->Sumw2();
  hWeffMetCut     ->Sumw2();
  hWeffLowMinv    ->Sumw2();
  hWeffZVeto      ->Sumw2();
  hWeffpMetCut    ->Sumw2();
  hWeffJetVeto    ->Sumw2();
  hWeffDeltaPhiJet->Sumw2();
  hWeffSoftMuVeto ->Sumw2();
  hWeffExtraLepton->Sumw2();
  hWeffPtll       ->Sumw2();
  hWeffTopTagging ->Sumw2();


  // WW level histograms
  //----------------------------------------------------------------------------
  TH1F *hPtLepton1WWLevel       = new TH1F("hPtLepton1WWLevel",       "", 160, 0, 160);  // 200, 0, 200
  TH1F *hPtLepton2WWLevel       = new TH1F("hPtLepton2WWLevel",       "",  80, 0,  80);  // 200, 0, 200
  TH1F *hPtDiLeptonWWLevel      = new TH1F("hPtDiLeptonWWLevel",      "", 120, 0, 120);  // 200, 0, 200
  TH1F *hMinvWWLevel            = new TH1F("hMinvWWLevel",            "", 200, 0, 200);
  TH1F *hMtWWLevel              = new TH1F("hMtWWLevel",              "", 250, 0, 250);
  TH1F *hNJets30WWLevel         = new TH1F("hNJetsPF30WWLevel",       "",  10, 0,  10);
  TH1F *hpfMetWWLevel           = new TH1F("hpfMetWWLevel",           "", 150, 0, 150);
  TH1F *hppfMetWWLevel          = new TH1F("hppfMetWWLevel",          "", 150, 0, 150);
  TH1F *hchMetWWLevel           = new TH1F("hchMetWWLevel",           "", 150, 0, 150);
  TH1F *hpchMetWWLevel          = new TH1F("hpchMetWWLevel",          "", 150, 0, 150);
  TH1F *hpminMetWWLevel         = new TH1F("hpminMetWWLevel",         "", 150, 0, 150);
  TH1F *hDeltaRLeptonsWWLevel   = new TH1F("hDeltaRLeptonsWWLevel",   "",  50, 0,   5);
  TH1F *hDeltaPhiLeptonsWWLevel = new TH1F("hDeltaPhiLeptonsWWLevel", "",  32, 0, 3.2);
  TH1F *hDPhiPtllJetWWLevel     = new TH1F("hDPhiPtllJetWWLevel",     "",  32, 0, 3.2);

  hPtLepton1WWLevel      ->Sumw2();
  hPtLepton2WWLevel      ->Sumw2();
  hPtDiLeptonWWLevel     ->Sumw2();
  hMinvWWLevel           ->Sumw2();
  hMtWWLevel             ->Sumw2();
  hNJets30WWLevel        ->Sumw2();
  hpfMetWWLevel          ->Sumw2();
  hppfMetWWLevel         ->Sumw2();
  hpchMetWWLevel         ->Sumw2();
  hpminMetWWLevel        ->Sumw2();
  hDeltaRLeptonsWWLevel  ->Sumw2();  
  hDeltaPhiLeptonsWWLevel->Sumw2();
  hDPhiPtllJetWWLevel    ->Sumw2();


  // Data-driven methods: Z+jets
  //----------------------------------------------------------------------------
  TH1F* hCountedMinvDYStudies = new TH1F("hCountedMinvDYStudies", "", 3, 0,3);

  hCountedMinvDYStudies->Sumw2();

  TH1F* hNinZevents     [numberMetCuts];
  TH1F* hNoutZevents    [numberMetCuts];
  TH1F* hNinLooseZevents[numberMetCuts];
  TH1F* hMassInZevents  [numberMetCuts];
  TH1F* hMassOutZevents [numberMetCuts];
  TH1F* hMinvDYStudies  [numberMetCuts];

  for (size_t nC=0; nC<numberMetCuts; nC++) {
    hNinZevents     [nC] = new TH1F(Form("hNinZevents%.1f",      MetCut[nC]), "",   3, 0,   3);
    hNoutZevents    [nC] = new TH1F(Form("hNoutZevents%.1f",     MetCut[nC]), "",   3, 0,   3);
    hNinLooseZevents[nC] = new TH1F(Form("hNinLooseZevents%.1f", MetCut[nC]), "",   3, 0,   3);
    hMassInZevents  [nC] = new TH1F(Form("hMassInZevents%.1f",   MetCut[nC]), "", 200, 0, 200);
    hMassOutZevents [nC] = new TH1F(Form("hMassOutZevents%.1f",  MetCut[nC]), "", 200, 0, 200);
    hMinvDYStudies  [nC] = new TH1F(Form("hMinvDYStudies%.1f",   MetCut[nC]), "", 200, 0, 200);
    
    hNinZevents     [nC]->Sumw2();
    hNoutZevents    [nC]->Sumw2();
    hNinLooseZevents[nC]->Sumw2();
    hMassInZevents  [nC]->Sumw2();
    hMassOutZevents [nC]->Sumw2();
    hMinvDYStudies  [nC]->Sumw2();
  }


  // Data-driven methods: Top
  //----------------------------------------------------------------------------
  TH1F* hTopTaggedEvents            = new TH1F("hTopTaggedEvents",            "", 3, 0, 3);
  TH1F* hNTopControlRegion          = new TH1F("hNTopControlRegion",          "", 3, 0, 3);
  TH1F* hNTopTaggedTopControlRegion = new TH1F("hNTopTaggedTopControlRegion", "", 3, 0, 3);

  TH1F* hbTagDisTopTaggedEvents            = new TH1F("hbTagDisTopTaggedEvents",            "", 300, -10, 20);
  TH1F* hbTagDisNTopControlRegion          = new TH1F("hbTagDisNTopControlRegion",          "", 300, -10, 20);
  TH1F* hbTagDisNTopTaggedTopControlRegion = new TH1F("hbTagDisNTopTaggedTopControlRegion", "", 300, -10, 20);
  
  hTopTaggedEvents           ->Sumw2();
  hNTopControlRegion         ->Sumw2();
  hNTopTaggedTopControlRegion->Sumw2();

  hbTagDisTopTaggedEvents           ->Sumw2();
  hbTagDisNTopControlRegion         ->Sumw2();
  hbTagDisNTopTaggedTopControlRegion->Sumw2();


  // Top checks
  //----------------------------------------------------------------------------
  TH1F* h_softtche = new TH1F("h_softtche", "", 300, -10, 20); 
  TH1F* h_jetpt1   = new TH1F("h_jetpt1",   "", 400,   0, 40); 

  h_softtche->Sumw2();
  h_jetpt1  ->Sumw2();


  //----------------------------------------------------------------------------
  // Input files
  //----------------------------------------------------------------------------
  TString filesPath;

  if (runAtOviedo) filesPath = "/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V05_S2_V05_S3_V05/TightTight_JEC2012/";
  if (runAtIfca)   filesPath = "/gpfs/csic_projects/cms/data/LatinosSkims/ReducedTrees/R52X_S1_V05_S2_V05_S3_V05/ForICHEP2/";

  TChain* tree = new TChain("latino", "latino");

  if (theSample == "DataRun2012_Total") {
    tree->Add(filesPath + "latino_100_SingleElectron2012A.root");
    tree->Add(filesPath + "latino_101_SingleMuon2012A.root");
    tree->Add(filesPath + "latino_102_DoubleElectron2012A.root");
    tree->Add(filesPath + "latino_103_DoubleMuon2012A.root");
    tree->Add(filesPath + "latino_104_MuEG2012A.root");
    tree->Add(filesPath + "latino_200_SingleElectron2012B.root");
    tree->Add(filesPath + "latino_201_SingleMuon2012B.root");
    tree->Add(filesPath + "latino_202_DoubleElectron2012B.root");
    tree->Add(filesPath + "latino_203_DoubleMuon2012B.root");
    tree->Add(filesPath + "latino_220_SingleElectron2012B.root");
    tree->Add(filesPath + "latino_221_SingleMuon2012B.root");
    tree->Add(filesPath + "latino_222_DoubleElectron2012B.root");
    tree->Add(filesPath + "latino_223_DoubleMuon2012B.root");
    tree->Add(filesPath + "latino_230_SingleElectron2012B.root");
    tree->Add(filesPath + "latino_224_MuEG2012B.root");
    tree->Add(filesPath + "latino_231_SingleMuon2012B.root");
    tree->Add(filesPath + "latino_232_DoubleElectron2012B.root");
    tree->Add(filesPath + "latino_233_DoubleMuon2012B.root");
    tree->Add(filesPath + "latino_234_MuEG2012B.root");
    tree->Add(filesPath + "latino_240_SingleElectron2012B.root");
    tree->Add(filesPath + "latino_241_SingleMuon2012B.root");
    tree->Add(filesPath + "latino_242_DoubleElectron2012B.root");
    tree->Add(filesPath + "latino_243_DoubleMuon2012B.root");
    tree->Add(filesPath + "latino_244_MuEG2012B.root");
    tree->Add(filesPath + "latino_204_MuEG2012B.root");
  }
  else if (theSample == "WJetsFakes_Total") {
    tree->Add(filesPath + "latino_step3_10X_LooseLoose_JEC2012.root"); 
    tree->Add(filesPath + "latino_step3_20X_LooseLoose_JEC2012.root");
    tree->Add(filesPath + "latino_step3_22X_LooseLoose_JEC2012.root");
    tree->Add(filesPath + "latino_step3_23X_LooseLoose_JEC2012.root");
    tree->Add(filesPath + "latino_step3_24X_LooseLoose_JEC2012.root");
  }
  else if (theSample == "ggWWto2L") {
    tree->Add(filesPath + "latino_001_GluGluToWWTo4L.root");
  }
  else if (theSample == "WWTo2L2Nu") {
    tree->Add(filesPath + "latino_000_WWJets2LMad.root");
  }
  else if (theSample == "WZ") {
    tree->Add(filesPath + "latino_074_WZJetsMad.root");
    tree->Add(filesPath + "latino_078_WZTo2L2QMad.root");
  }
  else if (theSample == "ZZ") {
    tree->Add(filesPath + "latino_075_ZZJetsMad.root");
    tree->Add(filesPath + "latino_079_ZZTo2L2QMad.root"); 
  }
  else if (theSample == "TTbar") {
    tree->Add(filesPath + "latino_019_TTTo2L2Nu2B.root");
  }
  else if (theSample == "TW") {
    tree->Add(filesPath + "latino_011_TtWFullDR.root");
    tree->Add(filesPath + "latino_012_TbartWFullDR.root");
  }
  else if (theSample == "Top") {
    tree->Add(filesPath + "latino_019_TTTo2L2Nu2B.root");
    tree->Add(filesPath + "latino_011_TtWFullDR.root");
    tree->Add(filesPath + "latino_012_TbartWFullDR.root");
  }
  else if (theSample == "WJets") {
    tree->Add(filesPath + "latino_178_WJetsToLNuMad.root");
  }
  else if (theSample == "DY") {
    tree->Add(filesPath + "latino_036_DY10toLLMad.root");
    tree->Add(filesPath + "latino_037_DY50toLLMad.root");
  }
  else if (theSample == "DYtautau") {
    tree->Add(filesPath + "latino_036_DY10toLLMad.root");  
    tree->Add(filesPath + "latino_037_DY50toLLMad.root");
  }
  else if (theSample == "WgammaNoStar") {
    tree->Add(filesPath + "latino_085_WgammaToLNuG.root");
  }
  else if (theSample == "WgammaStar") {
    tree->Add(filesPath + "latino_082_WGstarToElNuMad.root");
    tree->Add(filesPath + "latino_083_WGstarToMuNuMad.root");
    tree->Add(filesPath + "latino_084_WGstarToTauNuMad.root");
  }
  else if (theSample == "HWW125") { 
    tree->Add(filesPath + "latino_1125_ggToH125toWWTo2LAndTau2Nu.root");
    tree->Add(filesPath + "latino_2125_vbfToH125toWWTo2LAndTau2Nu.root");
  }
  else {
    return;
  }


  // Declaration of leaf types
  //----------------------------------------------------------------------------
  Float_t channel;     tree->SetBranchAddress("channel"   , &channel);
  UInt_t  run;         tree->SetBranchAddress("run"       , &run);
  UInt_t  lumi;        tree->SetBranchAddress("lumi"      , &lumi);
  UInt_t  event;       tree->SetBranchAddress("event"     , &event);

  Float_t baseW;       tree->SetBranchAddress("baseW"     , &baseW);
  Float_t effW;        tree->SetBranchAddress("effW"      , &effW);
  Float_t triggW;      tree->SetBranchAddress("triggW"    , &triggW);
  Float_t nvtx;        tree->SetBranchAddress("nvtx"      , &nvtx);

  Int_t   sameflav;    tree->SetBranchAddress("sameflav"  , &sameflav);
  Float_t trigger;     tree->SetBranchAddress("trigger"   , &trigger);
  Float_t mll;         tree->SetBranchAddress("mll"       , &mll);
  Float_t pt1;         tree->SetBranchAddress("pt1"       , &pt1);
  Float_t pt2;         tree->SetBranchAddress("pt2"       , &pt2);
  Float_t ptll;        tree->SetBranchAddress("ptll"      , &ptll);
  Float_t mth;         tree->SetBranchAddress("mth"       , &mth);
  Float_t dphilljet;   tree->SetBranchAddress("dphilljet" , &dphilljet);
  Float_t njet;        tree->SetBranchAddress("njet"      , &njet);
  Float_t jetpt1;      tree->SetBranchAddress("jetpt1"    , &jetpt1);
  Float_t jetpt2;      tree->SetBranchAddress("jetpt2"    , &jetpt2);
  Float_t jettche1;    tree->SetBranchAddress("jettche1"  , &jettche1);
  Float_t jettche2;    tree->SetBranchAddress("jettche2"  , &jettche2);
  Float_t softtche;    tree->SetBranchAddress("softtche"  , &softtche);
  Float_t nextra;      tree->SetBranchAddress("nextra"    , &nextra);
  Int_t   bveto;       tree->SetBranchAddress("bveto"     , &bveto);
  Int_t   bveto_nj30;  tree->SetBranchAddress("bveto_nj30", &bveto_nj30);
  Int_t   bveto_mu;    tree->SetBranchAddress("bveto_mu"  , &bveto_mu);
  Int_t   bveto_ip;    tree->SetBranchAddress("bveto_ip"  , &bveto_ip);
  Float_t nbjet;       tree->SetBranchAddress("nbjet"     , &nbjet);
 
  Float_t pfmet;       tree->SetBranchAddress("pfmet"     , &pfmet);
  Float_t ppfmet;      tree->SetBranchAddress("ppfmet"    , &ppfmet);
  Float_t chmet;       tree->SetBranchAddress("chmet"     , &chmet);
  Float_t pchmet;      tree->SetBranchAddress("pchmet"    , &pchmet);
  Float_t mpmet;       tree->SetBranchAddress("mpmet"     , &mpmet); 

  Int_t   zveto;       tree->SetBranchAddress("zveto"     , &zveto);
  Int_t   dphiveto;    tree->SetBranchAddress("dphiveto"  , &dphiveto);
  Float_t dphill;      tree->SetBranchAddress("dphill"    , &dphill);
  Float_t drll;        tree->SetBranchAddress("drll"      , &drll);
  Float_t mctruth;     tree->SetBranchAddress("mctruth"   , &mctruth);

  Float_t puWobs;
  Float_t fake2W;

  if (!theSample.Contains("WJetsFakes") && !theSample.Contains("Data"))
    tree->SetBranchAddress("puWobs", &puWobs);

  if (theSample.Contains("WJetsFakes"))
    tree->SetBranchAddress("fake2W" , &fake2W);


  // Set the channel
  //----------------------------------------------------------------------------
  Float_t SelectedChannel = -999;

  if      (flavorChannel == "MuMu") SelectedChannel =  0;
  else if (flavorChannel == "EE"  ) SelectedChannel =  1;
  else if (flavorChannel == "EMu" ) SelectedChannel =  2;
  else if (flavorChannel == "MuE" ) SelectedChannel =  3;
  else if (flavorChannel == "All" ) SelectedChannel = -1;


  //----------------------------------------------------------------------------
  // Loop
  //----------------------------------------------------------------------------
  for (int ievent=0; ievent<tree->GetEntries(); ievent++) {
    
    tree->GetEntry(ievent);
    
    Double_t efficiencyW = effW * triggW;
    Double_t totalW      = -999;

    if (theSample.Contains("Data"))
      totalW = 1.0;
    else if (theSample.Contains("WJetsFakes"))
      totalW = fake2W;
    else {
      efficiencyW = puWobs * effW * triggW;
      totalW      = baseW * efficiencyW * luminosity;
    }


    // The selection begins here
    //--------------------------------------------------------------------------
    if (theSample.Contains("DYtautau") && mctruth < 1.5) continue;

    if ((SelectedChannel == -1)           ||
	(channel == SelectedChannel)      ||
	(flavorChannel == "OF" && !sameflav) ||
	(flavorChannel == "SF" &&  sameflav)) {
      
      if (trigger == 1 && pt2 > 20) {
	
	hWTrigger   ->Fill(1, totalW); 
	hWeffTrigger->Fill(1, efficiencyW);
	
	Bool_t commonCuts = (mll > 12 && ptll > 45 && nextra == 0 && (dphiveto || !sameflav));


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Data-driven methods: Z+jets
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (commonCuts && dphiveto && njet == jetChannel && bveto) {
	  
	  if (fabs(mll - ZMASS) < 7.5 && pfmet > 20 && mpmet > 45) {
	    hCountedMinvDYStudies->Fill(1, totalW);
	  }
	  

	  // Loop over the mpmet cuts
	  //--------------------------------------------------------------------
	  for (size_t mc=0; mc<numberMetCuts; mc ++) {
	    
	    if (pfmet > 20 && mpmet > MetCut[mc]) {
	      hMinvDYStudies[mc]->Fill(mll, totalW);    
	    }
	    
	    if (fabs(mll - ZMASS) < 7.5) {
	      if (pfmet > 20 && mpmet > MetCut[mc]) hNinLooseZevents[mc]->Fill(1,totalW);
              if (pfmet > 20 && mpmet > MetCut[mc] && mpmet < MetCut[mc+1]) {   
		hNinZevents[mc]   ->Fill(  1, totalW);
		hMassInZevents[mc]->Fill(mll, totalW);
	      }
	    }
	    else if (fabs(mll - ZMASS) > 15.0) {  
	      if (pfmet > 20 && mpmet > MetCut[mc] && mpmet < MetCut[mc+1]) {
		hNoutZevents[mc]   ->Fill(  1, totalW);
		hMassOutZevents[mc]->Fill(mll, totalW);
	      }
	    }
	  }
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Data-driven methods: Top
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (commonCuts && zveto && pfmet > 20 && mpmet > (20 + 25*sameflav)) {
       
	  // btag_eff denominator
	  if ((jetChannel == 0 && njet == 1 && nbjet == 1) ||
	      (jetChannel == 1 && njet == 2 && jettche2 > 2.1)) {

	    hNTopControlRegion->Fill(1, totalW);
	    hbTagDisNTopControlRegion->Fill(jettche2, totalW);
	    
	    // btag_eff numerator
	    if ((jetChannel == 0 && !bveto_nj30) ||
		(jetChannel == 1 && jettche1 > 2.1)) {

	      hNTopTaggedTopControlRegion->Fill(1, totalW);
	      hbTagDisNTopTaggedTopControlRegion->Fill(jettche2, totalW);
	    }
	  }
	}
	
	// Top-tagged events for ttbar estimation
	//----------------------------------------------------------------------
	if (commonCuts && zveto && pfmet > 20 && mpmet > (20 + 25*sameflav)) {

	  if ((jetChannel == 0 && njet == 0 && !bveto) ||
	      (jetChannel == 1 && njet == 1 && bveto && jettche1 > 2.1)) {
	  
	    hTopTaggedEvents->Fill(1, totalW);
	    hbTagDisTopTaggedEvents->Fill(jettche2, totalW);
	  }
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Top checks
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (commonCuts && zveto && pfmet > 20 && mpmet > (20 + 25*sameflav) && njet < 1) {
	  
	  if (!bveto)    h_softtche->Fill(softtche, totalW);
	  if (!bveto_ip) h_jetpt1  ->Fill(jetpt1,   totalW);
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Main analysis
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (pfmet > 20 && mpmet > 20) {
	  
	  hWMetCut->Fill(1, totalW);
	  hWeffMetCut->Fill(1, efficiencyW);
	  
	  if (mll > 12) {
	    
	    hWLowMinv->Fill(1, totalW);
	    hWeffLowMinv->Fill(1, efficiencyW);
	    
	    if (zveto) {
	      
	      hWZVeto->Fill(1, totalW); 
	      hWeffZVeto->Fill(1, efficiencyW); 

	      if (mpmet > (20 + 25*sameflav)){
		
		hWpMetCut->Fill(1, totalW);
		hWeffpMetCut->Fill(1, efficiencyW);

		if (njet == jetChannel) {

		  hWJetVeto->Fill(1, totalW);
		  hWeffJetVeto->Fill(1, efficiencyW);

		  if (dphiveto || !sameflav) {
		    
		    hWDeltaPhiJet->Fill(1, totalW);
		    hWeffDeltaPhiJet->Fill(1, efficiencyW);

		    if (bveto_mu) {
		      
		      hWSoftMuVeto->Fill(1, totalW);
		      hWeffSoftMuVeto->Fill(1, efficiencyW);
		      
		      if (nextra == 0) {
			
			hWExtraLepton->Fill(1, totalW);
			hWeffExtraLepton->Fill(1, efficiencyW);
			
			if (ptll > 45) {
			  
			  hWPtll->Fill(1, totalW);
			  hWeffPtll->Fill(1, efficiencyW);

			  if (bveto_ip) {			    
			    
			    hWTopTagging->Fill(1, totalW);
			    hWeffTopTagging->Fill(1, efficiencyW);

			    hPtLepton1WWLevel      ->Fill(pt1,       totalW);
			    hPtLepton2WWLevel      ->Fill(pt2,       totalW);
			    hPtDiLeptonWWLevel     ->Fill(ptll,      totalW);
			    hMinvWWLevel           ->Fill(mll,       totalW);
			    hMtWWLevel             ->Fill(mth,       totalW);
			    hNJets30WWLevel        ->Fill(njet,      totalW);
			    hpfMetWWLevel          ->Fill(pfmet,     totalW);
			    hppfMetWWLevel         ->Fill(ppfmet,    totalW);
			    hchMetWWLevel          ->Fill(chmet,     totalW);
			    hpchMetWWLevel         ->Fill(pchmet,    totalW);
			    hpminMetWWLevel        ->Fill(mpmet,     totalW);
			    hDeltaRLeptonsWWLevel  ->Fill(drll,      totalW);
			    hDeltaPhiLeptonsWWLevel->Fill(dphill,    totalW);
			    hDPhiPtllJetWWLevel    ->Fill(dphilljet, totalW);
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
  }


  // Print
  //----------------------------------------------------------------------------
  if (verbose) {
    cout << " -------------------------------------" << endl;
    cout << " --- Expected number of RAW events ---" << endl;
    cout << " -------------------------------------" << endl;
    cout << "Trigger          -- " << hWTrigger    ->GetEntries() << endl;
    cout << "Met Cut          -- " << hWMetCut     ->GetEntries() << endl;
    cout << "Low Minv Cut     -- " << hWLowMinv    ->GetEntries() << endl;
    cout << "Z Veto           -- " << hWZVeto      ->GetEntries() << endl;
    cout << "projected MetCut -- " << hWpMetCut    ->GetEntries() << endl;
    cout << "Jet Veto         -- " << hWJetVeto    ->GetEntries() << endl;
    cout << "DeltaPhiJet Veto -- " << hWDeltaPhiJet->GetEntries() << endl;
    cout << "Soft Muon Veto   -- " << hWSoftMuVeto ->GetEntries() << endl;
    cout << "Extra Lepton     -- " << hWExtraLepton->GetEntries() << endl;
    cout << "Top Tagging      -- " << hWTopTagging ->GetEntries() << endl;

    if (!theSample.Contains("Data")) {
      cout << " -------------------------------------" << endl;
      cout << " --- Normalized to " << luminosity << " fb-1 --------" << endl;
      cout << " -------------------------------------" << endl;
      cout << "Trigger          -- " << hWTrigger    ->GetSumOfWeights() << endl;
      cout << "Met Cut          -- " << hWMetCut     ->GetSumOfWeights() << endl;
      cout << "Low Minv Cut     -- " << hWLowMinv    ->GetSumOfWeights() << endl;
      cout << "Z Veto           -- " << hWZVeto      ->GetSumOfWeights() << endl;
      cout << "projected MetCut -- " << hWpMetCut    ->GetSumOfWeights() << endl;
      cout << "Jet Veto         -- " << hWJetVeto    ->GetSumOfWeights() << endl;
      cout << "DeltaPhiJet Veto -- " << hWDeltaPhiJet->GetSumOfWeights() << endl;
      cout << "Soft Muon Veto   -- " << hWSoftMuVeto ->GetSumOfWeights() << endl;
      cout << "Extra Lepton     -- " << hWExtraLepton->GetSumOfWeights() << endl;
      cout << "Top Tagging      -- " << hWTopTagging ->GetSumOfWeights() << endl; 
    }    
  }


  // Save the histograms
  //----------------------------------------------------------------------------
  output->cd();
  output->Write("", TObject::kOverwrite);
  output->Close();
}
