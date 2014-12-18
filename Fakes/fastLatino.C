#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TSystem.h"


void fastLatino(TString sample = "019_TTTo2L2Nu2B")
{
  gROOT->SetBatch();

  TH1::SetDefaultSumw2();

  TH1F* h_mva_loose = new TH1F("h_mva_loose", "", 50, -1, 1);

  TString path = "/pool/ciencias/LatinosSkims/ReducedTrees/R53X_S1_V09_S2_V10_S3_V17newJEC/MC_LooseLooseTypeI/";

  TChain* tree = new TChain("latino", "latino");

  tree->Add(path + "latino_" + sample + ".root");

  TCut muon1 = "pt1>10 && bdt1>100 && pass2012ICHEP1";
  TCut muon2 = "pt2>10 && bdt2>100 && pass2012ICHEP2";
  TCut muon3 = "pt3>10 && bdt3>100";
  TCut gen1  = "leptonGenpid1==13";
  TCut gen2  = "leptonGenpid2==13";
  TCut dr11  = "sqrt((eta1-leptonGeneta1)*(eta1-leptonGeneta1)+(phi1-leptonGenphi1)*(phi1-leptonGenphi1))<0.3";
  TCut dr12  = "sqrt((eta1-leptonGeneta2)*(eta1-leptonGeneta2)+(phi1-leptonGenphi2)*(phi1-leptonGenphi2))<0.3";
  TCut dr21  = "sqrt((eta2-leptonGeneta1)*(eta2-leptonGeneta1)+(phi2-leptonGenphi1)*(phi2-leptonGenphi1))<0.3";
  TCut dr22  = "sqrt((eta2-leptonGeneta2)*(eta2-leptonGeneta2)+(phi2-leptonGenphi2)*(phi2-leptonGenphi2))<0.3";

  TCut cut1 = muon1 && muon2 && muon3;
  TCut cut2 = gen1 && gen2;
  TCut cut3 = (dr11 && dr22) || (dr12 && dr21);

  TCut selection = cut1 && cut2 && cut3;

  TCanvas* c1 = new TCanvas(sample, sample);

  c1->cd();
  
  tree->Draw("isomva3>>h_mva_loose", selection);


  // Define output
  //----------------------------------------------------------------------------
  gSystem->mkdir("rootfiles", kTRUE);

  TFile* file = new TFile("rootfiles/" + sample + ".root", "recreate");

  h_mva_loose->Write();

  file->Close();
}
