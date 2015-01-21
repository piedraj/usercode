//------------------------------------------------------------------------------
//
// drawWeights
//
//------------------------------------------------------------------------------
#include "DrawFunctions.h"


const Float_t _bigLabelSize = 0.04;


// Member functions
//------------------------------------------------------------------------------
void      DrawIt             (TString title);

void      AxisFonts          (TAxis*  axis,
			      TString title);

void      MakeOutputDirectory(TString format);


// Data members
//------------------------------------------------------------------------------
TFile* wzfile;


void drawWeights()
{
  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  wzfile = new TFile("results/analysis/074_WZJetsMad.root");

  DrawIt("hLeptonWeight");
  DrawIt("hTriggerWeight");
  DrawIt("hTotalWeight");
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TString name)
{
  TH1F* hMMM = (TH1F*)wzfile->Get(name + "_MMM_Exactly3Leptons_WInclusive");
  TH1F* hMME = (TH1F*)wzfile->Get(name + "_MME_Exactly3Leptons_WInclusive");
  TH1F* hEEE = (TH1F*)wzfile->Get(name + "_EEE_Exactly3Leptons_WInclusive");
  TH1F* hEEM = (TH1F*)wzfile->Get(name + "_EEM_Exactly3Leptons_WInclusive");

  printf("\n [%s] eee: %.1f%s \t eem: %.1f%s \t mme: %.1f%s \t mmm: %.1f%s\n\n",
	 name.Data(),
	 1e2 * hEEE->GetMean(), "%",
	 1e2 * hEEM->GetMean(), "%",
	 1e2 * hMME->GetMean(), "%",
	 1e2 * hMMM->GetMean(), "%");

  hMMM->SetLineColor(kRed);
  hMME->SetLineColor(kBlack);
  hEEE->SetLineColor(kBlue);
  hEEM->SetLineColor(kGreen+1);

  hMMM->SetLineWidth(2);
  hMME->SetLineWidth(2);
  hEEE->SetLineWidth(2);
  hEEM->SetLineWidth(2);

  TCanvas* c1 = new TCanvas(name, name);

  hEEE->Draw("hist");
  hMME->Draw("hist, same");
  hMMM->Draw("hist, same");
  hEEM->Draw("hist, same");
  hEEE->Draw("hist, same");

  if (hMME->GetMaximum() > hEEE->GetMaximum()) hEEE->SetMaximum(hMME->GetMaximum());
  if (hMMM->GetMaximum() > hEEE->GetMaximum()) hEEE->SetMaximum(hMMM->GetMaximum());
  if (hEEM->GetMaximum() > hEEE->GetMaximum()) hEEE->SetMaximum(hEEM->GetMaximum());

  hEEE->SetMaximum(1.1 * hEEE->GetMaximum());

  TString xtitle;

  if (name.Contains("Lepton"))  xtitle = "lepton identification SF";
  if (name.Contains("Trigger")) xtitle = "trigger #varepsilon";
  if (name.Contains("Total"))   xtitle = "lepton identification SF #upoint trigger #varepsilon";

  AxisFonts(hEEE->GetXaxis(), xtitle + " per event");
  AxisFonts(hEEE->GetYaxis(), "");

  DrawLegend(0.23, 0.82 - 0. * 0.063, (TObject*)hMMM, " #mu#mu#mu", "l", 0.04, 0.2, 0.07);
  DrawLegend(0.23, 0.82 - 1. * 0.063, (TObject*)hEEE, " eee",       "l", 0.04, 0.2, 0.07);
  DrawLegend(0.23, 0.82 - 2. * 0.063, (TObject*)hMME, " #mu#mue",   "l", 0.04, 0.2, 0.07);
  DrawLegend(0.23, 0.82 - 3. * 0.063, (TObject*)hEEM, " ee#mu",     "l", 0.04, 0.2, 0.07);
  
  c1->GetFrame()->DrawClone();

  c1->SaveAs("pdf/scale_factors/" + name + ".pdf");
  c1->SaveAs("png/scale_factors/" + name + ".png");
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis* axis, TString title)
{
  axis->SetLabelFont  (   42);
  axis->SetLabelOffset(0.015);
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetTitleOffset(  1.5);
  axis->SetLabelSize  (_bigLabelSize);
  axis->SetTitleSize  (_bigLabelSize);

  axis->SetTitle(title);
}


//------------------------------------------------------------------------------
// MakeOutputDirectory
//------------------------------------------------------------------------------
void MakeOutputDirectory(TString format)
{
  gSystem->mkdir(Form("%s/scale_factors", format.Data()), kTRUE);

  gSystem->Exec(Form("cp index.php %s/.", format.Data()));

  gSystem->Exec(Form("cp index.php %s/scale_factors/.", format.Data()));
}
