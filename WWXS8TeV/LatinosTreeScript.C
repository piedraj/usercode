#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TSystem.h"
#include "TTree.h"
#include <iomanip>
#include <iostream>


const UInt_t numberMetCuts = 5;

Double_t MetCut[] = {20, 25, 30, 45, 1000};


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
  TH1F *hWTrigger     = new TH1F("hWTrigger"    ,"",3,0.,3.);
  TH1F *hWMetCut      = new TH1F("hWMetCut"     ,"",3,0.,3.);
  TH1F *hWLowMinv     = new TH1F("hWLowMinv"    ,"",3,0.,3.);
  TH1F *hWZVeto       = new TH1F("hWZVeto"      ,"",3,0.,3.);
  TH1F *hWpMetCut     = new TH1F("hWpMetCut"    ,"",3,0.,3.);
  TH1F *hWJetVeto     = new TH1F("hWJetVeto"    ,"",3,0.,3.);
  TH1F *hWDeltaPhiJet = new TH1F("hWDeltaPhiJet","",3,0.,3.);
  TH1F *hWSoftMuVeto  = new TH1F("hWSoftMuVeto" ,"",3,0.,3.); 
  TH1F *hWExtraLepton = new TH1F("hWExtraLepton","",3,0.,3.);
  TH1F *hWPtll        = new TH1F("hWPtll"       ,"",3,0.,3.);
  TH1F *hWTopTagging  = new TH1F("hWTopTagging" ,"",3,0.,3.);

  TH1F *hWeffTrigger     = new TH1F("hWeffTrigger"    ,"",3,0.,3.);
  TH1F *hWeffMetCut      = new TH1F("hWeffMetCut"     ,"",3,0.,3.);
  TH1F *hWeffLowMinv     = new TH1F("hWeffLowMinv"    ,"",3,0.,3.);
  TH1F *hWeffZVeto       = new TH1F("hWeffZVeto"      ,"",3,0.,3.);
  TH1F *hWeffpMetCut     = new TH1F("hWeffpMetCut"    ,"",3,0.,3.);
  TH1F *hWeffJetVeto     = new TH1F("hWeffJetVeto"    ,"",3,0.,3.);
  TH1F *hWeffDeltaPhiJet = new TH1F("hWeffDeltaPhiJet","",3,0.,3.);
  TH1F *hWeffSoftMuVeto  = new TH1F("hWeffSoftMuVeto" ,"",3,0.,3.); 
  TH1F *hWeffExtraLepton = new TH1F("hWeffExtraLepton","",3,0.,3.);
  TH1F *hWeffPtll        = new TH1F("hWeffPtll"       ,"",3,0.,3.);
  TH1F *hWeffTopTagging  = new TH1F("hWeffTopTagging" ,"",3,0.,3.);

  TH1F *hMisTagNum = new TH1F("hMisTagNum","",3,0.,3.);
  TH1F *hMisTagDem = new TH1F("hMisTagDem","",3,0.,3.);

  TH1F *hDPhiPtllJet     = new TH1F("hDPhiPtllJet"    ,"",25,0.,TMath::Pi());


  TH1F *hnvtx     = new TH1F("hnvtx"         ,"",25,0.,25.);
  TH1F *hjetpt1   = new TH1F("hjetpt1"         ,"",25,0.,200.);
  TH1F *hiso      = new TH1F("hiso"            ,"",50,0.,0.15);

  TH1F *hnvtxWWLevel     = new TH1F("hnvtxWWLevel"         ,"",25,0.,25.);
  TH1F *hjetpt1WWLevel   = new TH1F("hjetpt1WWLevel"         ,"",25,0.,200.);
  TH1F *hisoWWLevel      = new TH1F("hisoWWLevel"            ,"",50,0.,0.15);

  TH1F *hPtLepton1WWLevel       = new TH1F("hPtLepton1WWLevel"      ,"",50,0.,200.);
  TH1F *hPtLepton2WWLevel       = new TH1F("hPtLepton2WWLevel"      ,"",32,0.,160.);
  TH1F *hPtDiLeptonWWLevel      = new TH1F("hPtDiLeptonWWLevel"     ,"",50,0.,200.);
  TH1F *hMinvWWLevel            = new TH1F("hMinvWWLevel"           ,"",40,0.,200.);
  TH1F *hRazorWWLevel           = new TH1F("hRazorWWLevel"          ,"",25,0.,4.);
  TH1F *hMtWWLevel              = new TH1F("hMtWWLevel"             ,"",25,0.,250.);
  TH1F *hNJets30WWLevel         = new TH1F("hNJetsPF30WWLevel"      ,"",10,0.,10.);

  TH1F *hpfMetWWLevel           = new TH1F("hpfMetWWLevel"          ,"",25,0.,150.);
  TH1F *hppfMetWWLevel          = new TH1F("hppfMetWWLevel"         ,"",25,0.,150.);
  TH1F *hchMetWWLevel           = new TH1F("hchMetWWLevel"          ,"",25,0.,150.);
  TH1F *hpchMetWWLevel          = new TH1F("hpchMetWWLevel"         ,"",25,0.,150.);
  TH1F *hpminMetWWLevel         = new TH1F("hpminMetWWLevel"        ,"",25,0.,150.);
  TH1F *hDeltaPhiLeptonsWWLevel = new TH1F("hDeltaPhiLeptonsWWLevel","",25,0.,TMath::Pi());
  TH1F *hDeltaRLeptonsWWLevel   = new TH1F("hDeltaRLeptonsWWLevel"  ,"",50,0.,5);
  TH1F *hDPhiPtllJetWWLevel     = new TH1F("hDPhiPtllJetWWLevel"    ,"",25,0.,TMath::Pi());




  //DataDriven methods
  TH1F* hNinZevents      [numberMetCuts];
  TH1F* hNoutZevents     [numberMetCuts];
  TH1F* hNinLooseZevents [numberMetCuts];
  TH1F* hMassInZevents   [numberMetCuts];
  TH1F* hMassOutZevents  [numberMetCuts];
  TH1F *hMinvDYStudies   [numberMetCuts];
  TH1F *hCountedMinvDYStudies = new TH1F("hCountedMinvDYStudies","Counted events for nominal cut",3,0.,3.);


  for( size_t nC = 0; nC < numberMetCuts; nC++){
    hNinZevents      [nC] = new TH1F(Form("hNinZevents%.1f",      MetCut[nC]), "",3,0.,3.);
    hNoutZevents     [nC] = new TH1F(Form("hNoutZevents%.1f",     MetCut[nC]), "",3,0.,3.);
    hNinLooseZevents [nC] = new TH1F(Form("hNinLooseZevents%.1f", MetCut[nC]), "",3,0.,3.);
    hMassInZevents   [nC] = new TH1F(Form("hMassInZevents%.1f",   MetCut[nC]), "",25,0.,200.);
    hMassOutZevents  [nC] = new TH1F(Form("hMassOutZevents%.1f",  MetCut[nC]), "",25,0.,200.);
    hMinvDYStudies   [nC] = new TH1F(Form("hMinvDYStudies%.1f",   MetCut[nC]), "",25,0.,200.);
  }


  TH1F* hTopTaggedEvents            = new TH1F("hTopTaggedEvents"           ,"",3,0.,3.0);
  TH1F* hNTopControlRegion          = new TH1F("hNTopControlRegion"         ,"",3,0.,3.0);
  TH1F* hNTopTaggedTopControlRegion = new TH1F("hNTopTaggedTopControlRegion","",3,0.,3.0);

  TH1F* hbTagDisTopTaggedEvents            = new TH1F("hbTagDisTopTaggedEvents"           , "", 300, -10, 20);
  TH1F* hbTagDisNTopControlRegion          = new TH1F("hbTagDisNTopControlRegion"         , "", 300, -10, 20);
  TH1F* hbTagDisNTopTaggedTopControlRegion = new TH1F("hbTagDisNTopTaggedTopControlRegion", "", 300, -10, 20);


  TH1F* h_softtche = new TH1F("h_softtche", "", 300, -10, 20); 
  TH1F* h_jettche1 = new TH1F("h_jettche1", "", 300, -10, 20); 
  TH1F* h_jetpt1   = new TH1F("h_jetpt1",   "", 400,   0, 40); 



  hWTrigger     ->Sumw2();
  hWMetCut      ->Sumw2();
  hWLowMinv     ->Sumw2();
  hWZVeto       ->Sumw2();
  hWpMetCut     ->Sumw2();
  hWJetVeto     ->Sumw2();
  hWDeltaPhiJet ->Sumw2();
  hWSoftMuVeto  ->Sumw2();
  hWExtraLepton ->Sumw2();
  hWPtll        ->Sumw2();
  hWTopTagging  ->Sumw2();

  hWeffTrigger     ->Sumw2();
  hWeffMetCut      ->Sumw2();
  hWeffLowMinv     ->Sumw2();
  hWeffZVeto       ->Sumw2();
  hWeffpMetCut     ->Sumw2();
  hWeffJetVeto     ->Sumw2();
  hWeffDeltaPhiJet ->Sumw2();
  hWeffSoftMuVeto  ->Sumw2();
  hWeffExtraLepton ->Sumw2();
  hWeffPtll        ->Sumw2();
  hWeffTopTagging  ->Sumw2();

  hCountedMinvDYStudies->Sumw2();
  for( size_t nC = 0; nC < numberMetCuts; nC++){
    hNinZevents      [nC] ->Sumw2();
    hNoutZevents     [nC] ->Sumw2();
    hNinLooseZevents [nC] ->Sumw2();
    hMassInZevents   [nC] ->Sumw2();
    hMassOutZevents  [nC] ->Sumw2();
    hMinvDYStudies   [nC] ->Sumw2();
  }

  hTopTaggedEvents           ->Sumw2();
  hNTopControlRegion         ->Sumw2();
  hNTopTaggedTopControlRegion->Sumw2();

  hbTagDisTopTaggedEvents           ->Sumw2();
  hbTagDisNTopControlRegion         ->Sumw2();
  hbTagDisNTopTaggedTopControlRegion->Sumw2();


  hMisTagNum->Sumw2();
  hMisTagDem->Sumw2();

  hDPhiPtllJet    ->Sumw2();

  hnvtx->Sumw2();
  hjetpt1->Sumw2();
  hiso->Sumw2();

  hnvtxWWLevel->Sumw2();
  hjetpt1WWLevel->Sumw2();
  hisoWWLevel->Sumw2();

  hPtLepton1WWLevel      ->Sumw2();
  hPtLepton2WWLevel      ->Sumw2();
  hPtDiLeptonWWLevel     ->Sumw2();
  hMinvWWLevel           ->Sumw2();
  hRazorWWLevel          ->Sumw2();
  hMtWWLevel             ->Sumw2();
  hNJets30WWLevel        ->Sumw2();
  hpfMetWWLevel          ->Sumw2();
  hppfMetWWLevel         ->Sumw2();
  hpchMetWWLevel         ->Sumw2();
  hpminMetWWLevel        ->Sumw2();
  hDeltaPhiLeptonsWWLevel->Sumw2();
  hDeltaRLeptonsWWLevel  ->Sumw2();  
  hDPhiPtllJetWWLevel    ->Sumw2();


  //----------------------------------------------------------------------------
  // Input files
  //----------------------------------------------------------------------------
  TChain* tree = new TChain("latino", "latino");

  if (theSample == "DataRun2012_Total") {
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_100_SingleElectron2012A.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_101_SingleMuon2012A.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_102_DoubleElectron2012A.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_103_DoubleMuon2012A.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_104_MuEG2012A.root");             
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_200_SingleElectron2012B.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_201_SingleMuon2012B.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_202_DoubleElectron2012B.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_203_DoubleMuon2012B.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_204_MuEG2012B.root");             
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_220_SingleElectron2012B.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_221_SingleMuon2012B.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_222_DoubleElectron2012B.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_223_DoubleMuon2012B.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_224_MuEG2012B.root");             
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_230_SingleElectron2012B.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_231_SingleMuon2012B.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_232_DoubleElectron2012B.root");   
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_233_DoubleMuon2012B.root");       
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_234_MuEG2012B.root");             
  }
  else if (theSample == "WJetsFakes_Total") {
    tree->Add("/nfs/fanae/user/lara/TreesLatinosAndrea/Trees35/TightTight_triggW/latino_step3_RunAB_LooseLoose_3.54fb.root");
  }
  else if (theSample == "ggWWto2L") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_001_GluGluToWWTo4L.root");
  }
  else if (theSample == "WWTo2L2Nu") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_000_WWJets2LMad.root");
  }
  else if (theSample == "WZ") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_074_WZJetsMad.root");
  }
  else if (theSample == "ZZ") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_075_ZZJetsMad.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_079_ZZTo2L2QMad.root"); 
  }
  else if (theSample == "TTbar") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_010_TTJetsMad.root");
  }
  else if (theSample == "TW") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_011_TtWFullDR.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_012_TbartWFullDR.root");
  }
  else if (theSample == "Top") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_010_TTJetsMad.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_011_TtWFullDR.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_012_TbartWFullDR.root");
  }
  else if (theSample == "WJets") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_178_WJetsToLNuMad.root");
  }
  else if (theSample == "DY") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_037_DY50toLLMad.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_036_DY10toLLMad.root");  
  }
  else if (theSample == "DYtautau") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_037_DY50toLLMad.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_step3_036_DY10toLLMad.root");  
  }
  else if (theSample == "WgammaNoStar") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_085_WgammaToLNuG.root");
  }
  else if(theSample == "WgammaStar") {
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_084_WGstarToTauNuMad.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_083_WGstarToMuNuMad.root");
    tree->Add("/hadoop/LatinosSkims/ReducedTrees/R52X_S1_V04_S2_V02_S3_V04_new/latino_082_WGstarToElNuMad.root");
  }
  else {
    return;
  }


  // Declaration of leaf types
  Float_t channel;
  UInt_t run;
  UInt_t lumi;
  UInt_t event;

  Float_t baseW;
  Float_t effW;
  Float_t triggW;
  Float_t kfW;
  Float_t puWsmurf;
  Float_t fake2W;

  Float_t effAW;
  Float_t effBW;
  Float_t triggAW;
  Float_t triggBW;
  Float_t puAW;
  Float_t puBW;

  Float_t nvtx;

  Int_t sameflav;
  Float_t trigger;
  Float_t mll;
  Float_t pt1;
  Float_t pt2;
  Float_t eta1;
  Float_t eta2;
  Float_t ptll;
  Float_t gammaMRStar;
  Float_t mth;
  Float_t dphilljet;
  Float_t njet;
  Float_t jetpt1;
  Float_t jetpt2;
  Float_t jettche1;
  Float_t jettche2;
  Float_t softtche;
  Float_t iso1;
  Float_t iso2;
  Float_t nextra;
  Int_t  bveto;
  Int_t  bveto_nj30;
  Int_t  bveto_mu;
  Int_t  bveto_ip;
  Float_t nbjet;
 
  Float_t pfmet;
  Float_t ppfmet;
  Float_t chmet;
  Float_t pchmet;
  Float_t mpmet; 

  Int_t  zveto;
  Int_t  dphiveto;
  Float_t dphill;
  Float_t drll;
  
  Float_t mctruth;

  

  // Set variables to the branches
  tree->SetBranchAddress("channel", &channel);
  tree->SetBranchAddress("run"    , &run);
  tree->SetBranchAddress("lumi"   , &lumi);
  tree->SetBranchAddress("event"  , &event);

  tree->SetBranchAddress("baseW"  , &baseW);

  if (!theSample.Contains("WJetsFakes") && !theSample.Contains("Data")) tree->SetBranchAddress("puWsmurf", &puWsmurf);

  tree->SetBranchAddress("triggW" , &triggW);
  tree->SetBranchAddress("kfW"    , &kfW);
  if (theSample.Contains("WJetsFakes")) tree->SetBranchAddress("fake2W" , &fake2W);

  tree->SetBranchAddress("puAW"    , &puAW);
  tree->SetBranchAddress("puBW"    , &puBW);
  tree->SetBranchAddress("effW"   , &effW);
  tree->SetBranchAddress("effAW"   , &effAW);
  tree->SetBranchAddress("effBW"   , &effBW);
  tree->SetBranchAddress("triggAW" , &triggAW);
  tree->SetBranchAddress("triggBW" , &triggBW);

  tree->SetBranchAddress("nvtx"   , &nvtx);
    tree->SetBranchAddress("mctruth"   , &mctruth);


  tree->SetBranchAddress("sameflav"    , &sameflav);
  tree->SetBranchAddress("trigger"     , &trigger);
  tree->SetBranchAddress("mll"         , &mll);
  tree->SetBranchAddress("pt1"         , &pt1);
  tree->SetBranchAddress("pt2"         , &pt2);
  tree->SetBranchAddress("eta1",  &eta1);
  tree->SetBranchAddress("eta2",&eta2);
  tree->SetBranchAddress("ptll"        , &ptll);
  tree->SetBranchAddress("dphiveto"        , &dphiveto);
  tree->SetBranchAddress("gammaMRStar" , &gammaMRStar);
  tree->SetBranchAddress("mth"         , &mth);
  tree->SetBranchAddress("dphilljet"   , &dphilljet);
  tree->SetBranchAddress("njet"        , &njet);
  tree->SetBranchAddress("jetpt1"      , &jetpt1);
  tree->SetBranchAddress("jetpt2"      , &jetpt2);
  tree->SetBranchAddress("jettche1"    , &jettche1);
  tree->SetBranchAddress("jettche2"    , &jettche2);
  tree->SetBranchAddress("softtche"    , &softtche);
  tree->SetBranchAddress("iso1"        , &iso1);
  tree->SetBranchAddress("iso2"        , &iso2);
  tree->SetBranchAddress("bveto"       , &bveto);
  tree->SetBranchAddress("bveto_nj30"    , &bveto_nj30);
  tree->SetBranchAddress("bveto_mu"    , &bveto_mu);
  tree->SetBranchAddress("bveto_ip"    , &bveto_ip);
  tree->SetBranchAddress("nbjet"       , &nbjet);

  tree->SetBranchAddress("mpmet"       , &mpmet);
  tree->SetBranchAddress("pfmet"       , &pfmet);
  tree->SetBranchAddress("ppfmet"      , &ppfmet);
  tree->SetBranchAddress("chmet"       , &chmet);
  tree->SetBranchAddress("pchmet"      , &pchmet);


  tree->SetBranchAddress("zveto"       , &zveto);
  tree->SetBranchAddress("nextra"      , &nextra);
  tree->SetBranchAddress("dphill"      , &dphill);
  tree->SetBranchAddress("drll"        , &drll);











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
      efficiencyW = puWsmurf * effW * triggW;
      totalW      = baseW * efficiencyW * luminosity;
    }


    // The selection begins here
    //--------------------------------------------------------------------------
    if ((SelectedChannel == -1)           ||
	(channel == SelectedChannel)      ||
	(flavorChannel == "OF" && !sameflav) ||
	(flavorChannel == "SF" &&  sameflav)) {
      
      Bool_t isData = (theSample.Contains("Data") || theSample.Contains("WJetsFakes")) ? true : false;

      if ((!isData || trigger == 1) && pt2 > 20) {
            
	if (theSample.Contains("DYtautau") && mctruth < 1.5) continue;
	
	hWTrigger   ->Fill(1, totalW); 
	hWeffTrigger->Fill(1, efficiencyW);
	
	Float_t pMetCut = 20 + 25*sameflav;

	Bool_t commonCuts = (ptll > 45 && nextra == 0 && (dphiveto || !sameflav) && mll > 12);


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Data-driven methods: Z+jets
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// Apply all the cuts at WW Level, except the Minv and met ones (later)
	if ( commonCuts && mll > 12 && dphiveto==1 && njet == jetChannel && bveto_mu == 1 && nextra ==0 && bveto_ip == 1 ){
	  

	  
	  if( fabs( mll - 91.1876 ) < 7.5 && (dphilljet<TMath::Pi()/180.*165.) && pfmet > 20 && mpmet > 45) {
	    hCountedMinvDYStudies->Fill(1,totalW);
	  }
	  
	  for(size_t mc = 0; mc < numberMetCuts; mc ++){
	    // IN -- Number of events inside the peak
	    
	    

	    if( pfmet > 20 && mpmet > 20 && mpmet > MetCut[mc] ){
	      hMinvDYStudies[mc]->Fill(mll,totalW);    
	    }
	    

	    if( fabs( mll - 91.1876 ) < 7.5 ){
	    

	      
	     
             // k factor -- for all met cuts ( to be used later the looser ones )                                                                                              
              if( pfmet > 20 && mpmet > MetCut[mc] ) hNinLooseZevents[mc]->Fill(1,totalW);                                                                                      
                                                                                                                                                                                
                                                                                                                                                                                
              // Nin -- for all min met cuts                                                                                                                                    
              if( pfmet > 20 && mpmet > MetCut[mc] && mpmet < MetCut[mc+1] ){   
		hNinZevents[mc]->Fill(1,totalW);
		hMassInZevents[mc]->Fill(mll,totalW);
	      }
	      
	      // OUT 
	    }else if( fabs( mll - 91.1876 ) > 15.0 ) {  
	      // Nout -- for all met cuts
	      if( pfmet > 20 && mpmet > 20 && MetCut[mc] < mpmet && mpmet < MetCut[mc+1] ){
		hNoutZevents[mc]->Fill(1,totalW);
		hMassOutZevents[mc]->Fill(mll,totalW);
	      }
	    }
	    
	  }
	}// R-estimation
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Data-driven methods: Top
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if( pfmet<30 && njet==0 && fabs(mll-91.1876)<7.5 ){
	  hMisTagDem->Fill(1);
	  if(!bveto)  hMisTagNum->Fill(1); 
	}
	
	//Top-tagged events for ttbar estimation
	if( commonCuts && pfmet > 20 && mpmet > 20 && zveto == 1 && mpmet > pMetCut && njet == 1 && nbjet == 1 ){
       
       
	  // eff_btag = Ncontrol_toptagged/Ncontrol    
	  // Final counting
	  // Denominator
	  hNTopControlRegion->Fill(1,totalW);
	  hbTagDisNTopControlRegion->Fill(jettche2,totalW);

	

	
	  if( bveto_nj30 == 0 ) {
	      hNTopTaggedTopControlRegion->Fill(1,totalW);
	      hbTagDisNTopTaggedTopControlRegion->Fill(jettche2,totalW);
	  }
	}
	
	// Top-tagged events for ttbar estimation
	if ( commonCuts && pfmet > 20 && mpmet > 20 && zveto == 1 && mpmet > pMetCut && njet < 1 && ( !bveto ) ) {
	  
	  // Final counting
	  hTopTaggedEvents->Fill(1,totalW);
	  hbTagDisTopTaggedEvents->Fill(jettche2,totalW);

	  h_softtche->Fill(softtche, totalW);
	  h_jettche1->Fill(jettche1, totalW);
	}


	if ( commonCuts && pfmet > 20 && mpmet > 20 && zveto == 1 && mpmet > pMetCut && njet < 1 && ( !bveto_ip ) ) {
	  //	  if (jetpt1 >= 10)
	    h_jetpt1->Fill(jetpt1, totalW);
	    //	  else
	    //	    h_jetpt1->Fill(0.0, totalW);
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Main analysis
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	
	if( pfmet > 20 && mpmet > 20){
	  

	  hWMetCut->Fill(1,totalW);
	  hWeffMetCut->Fill(1,efficiencyW);

	  if(mll > 12  ){
	    

	    hWLowMinv->Fill(1,totalW);
	    hWeffLowMinv->Fill(1,efficiencyW);
	    
	    if ( zveto == 1 ){
	      

	      hWZVeto->Fill(1,totalW); 
	      hWeffZVeto->Fill(1,efficiencyW); 

	      if ( ( !sameflav || mpmet>45)){
		

		hWpMetCut->Fill(1,totalW);
		hWeffpMetCut->Fill(1,efficiencyW);

		if ( njet == jetChannel ){
		  
		  hWJetVeto->Fill(1,totalW);
		  hWeffJetVeto->Fill(1,efficiencyW);

		  if ( (dphiveto || !sameflav ) ){
		    
		    hWDeltaPhiJet->Fill(1,totalW);
		    hWeffDeltaPhiJet->Fill(1,efficiencyW);

		    if ( bveto_mu == 1 ){
		      

		      hWSoftMuVeto->Fill(1,totalW);
		      hWeffSoftMuVeto->Fill(1,efficiencyW);
		      
		      if ( nextra == 0 ){
			
			hWExtraLepton->Fill(1,totalW);
			hWeffExtraLepton->Fill(1,efficiencyW);
			
			if( ptll > 45 ){
			  
			  hWPtll->Fill(1,totalW);
			  hWeffPtll->Fill(1,efficiencyW);

			  if( bveto_ip == 1 ){			    
			    
			    
			    
			    hWTopTagging->Fill(1,totalW);
			    hWeffTopTagging->Fill(1,efficiencyW);
			    hPtLepton1WWLevel      ->Fill(pt1,totalW);
			    hPtLepton2WWLevel      ->Fill(pt2,totalW);
			    hPtDiLeptonWWLevel     ->Fill(ptll,totalW);
			    hMinvWWLevel           ->Fill(mll,totalW);
			    hMtWWLevel             ->Fill(mth,totalW);
			    hNJets30WWLevel        ->Fill(njet,totalW);
			    hpfMetWWLevel          ->Fill(pfmet,totalW);
			    hppfMetWWLevel         ->Fill(ppfmet,totalW);
			    hchMetWWLevel          ->Fill(chmet,totalW);
			    hpchMetWWLevel         ->Fill(pchmet,totalW);
			    hpminMetWWLevel        ->Fill(mpmet,totalW);
			    hDeltaPhiLeptonsWWLevel->Fill(dphill,totalW);
			    hDeltaRLeptonsWWLevel  ->Fill(drll,totalW);
			    hDPhiPtllJetWWLevel    ->Fill(dphilljet,totalW);

			  }//kinematical cuts
			  
			}// b-tagging
			
		      }// nextra < 1
		      
		    }// bveto_mu = true
		    
		  }// dphilljet < 165
		  
		}// njet = jetChannel
		
	      }// mpmet > 20/40
	      
	    }// zveto = true
	    
	  }// mll > 12
	  
	}// pfmet > 20

      }// Channel IF
      
    }// trigger


    
  }// Loop Tree


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

  if( !theSample.Contains("Data") ) {
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


  output->cd();
  
  output->Write("",TObject::kOverwrite);
  output->Close();




}// void



