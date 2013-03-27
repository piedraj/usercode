//------------------------------------------------------------------------------
//
// drawWeights
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TSystem.h"


// Member functions
//------------------------------------------------------------------------------
void     DrawIt            (TString     title);

void     AxisFonts         (TAxis*      axis,
			    TString     title);

TLegend* DrawLegend        (Float_t     x1,
			    Float_t     y1,
			    TH1*        hist,
			    TString     label,
			    TString     option,
			    Float_t     tsize,
			    Float_t     xoffset,
			    Float_t     yoffset);

void    MakeOutputDirectory(TString     format);


// Data members
//------------------------------------------------------------------------------
TFile* f;


void drawWeights()
{
  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  gInterpreter->ExecuteMacro("./HiggsPaperStyle.C");

  f = new TFile("results/Summer12_53X/WH/analysis/WZTo3LNu.root");

  DrawIt("hLeptonWeight");
  DrawIt("hTriggerWeight");
  DrawIt("hTotalWeight");
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TString name)
{
  TH1F* hMMM = (TH1F*)f->Get(name + "_MMM_Exactly3Leptons");
  TH1F* hMME = (TH1F*)f->Get(name + "_MME_Exactly3Leptons");
  TH1F* hEEE = (TH1F*)f->Get(name + "_EEE_Exactly3Leptons");
  TH1F* hEEM = (TH1F*)f->Get(name + "_EEM_Exactly3Leptons");

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

  if (name.Contains("Lepton"))  xtitle = "lepton";
  if (name.Contains("Trigger")) xtitle = "trigger";
  if (name.Contains("Total"))   xtitle = "lepton #upoint trigger";

  AxisFonts(hEEE->GetXaxis(), xtitle + " weight per event");
  AxisFonts(hEEE->GetYaxis(), "");

  DrawLegend(0.23, 0.82 - 0. * 0.063, hMMM, " #mu#mu#mu", "l", 0.04, 0.2, 0.07);
  DrawLegend(0.23, 0.82 - 1. * 0.063, hEEE, " eee",       "l", 0.04, 0.2, 0.07);
  DrawLegend(0.23, 0.82 - 2. * 0.063, hMME, " #mu#mue",   "l", 0.04, 0.2, 0.07);
  DrawLegend(0.23, 0.82 - 3. * 0.063, hEEM, " ee#mu",     "l", 0.04, 0.2, 0.07);
  
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
  axis->SetLabelSize  (0.050);
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetTitleOffset(  1.5);
  axis->SetTitleSize  (0.050);

  axis->SetTitle(title);
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
TLegend* DrawLegend(Float_t x1,
		    Float_t y1,
		    TH1*    hist,
		    TString label,
		    TString option,
		    Float_t tsize,
		    Float_t xoffset,
		    Float_t yoffset)
{
  TLegend* legend = new TLegend(x1,
				y1,
				x1 + xoffset,
				y1 + yoffset);
  
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (tsize);

  legend->AddEntry(hist, label.Data(), option.Data());
  legend->Draw();

  return legend;
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
