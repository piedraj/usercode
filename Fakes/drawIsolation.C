//------------------------------------------------------------------------------
//
// drawIsolation by Jonatan Piedra
//
//------------------------------------------------------------------------------
#include "DrawFunctions.h"
#include <vector>


vector<UInt_t>  jetet;
vector<Color_t> color;
vector<TH1F*>   fakes;

Float_t         ndelta;
Float_t         xdelta;


// Member functions
//------------------------------------------------------------------------------
void AxisLabels(TH1F*    hist);

void SaveCanvas(TCanvas* canvas,
		TString  cname);


//------------------------------------------------------------------------------
// drawIsolation
//------------------------------------------------------------------------------
void drawIsolation(TString hname = "h_mva_loose")
{
  Float_t x0    = 0.220;
  Float_t y0    = 0.845;
  Float_t delta = 0.049;

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);

  jetet.clear();
  color.clear();
  fakes.clear();

  jetet.push_back(30);
  jetet.push_back(50);
  jetet.push_back(65);

  color.push_back(kGreen+1);
  color.push_back(kBlue);
  color.push_back(kGray);


  // Read top and Z+jets files
  //----------------------------------------------------------------------------
  TFile* fileFakes = new TFile("rootfiles/muonFakeRate.root",    "read");
  TFile* fileTop   = new TFile("rootfiles/019_TTTo2L2Nu2B.root", "read");
  TFile* fileZJets = new TFile("rootfiles/037_DY50toLLMad.root", "read");

  TH1F* h_mva_top   = (TH1F*)fileTop  ->Get(hname);
  TH1F* h_mva_zjets = (TH1F*)fileZJets->Get(hname);

  h_mva_top  ->Scale(1. / h_mva_top  ->Integral());
  h_mva_zjets->Scale(1. / h_mva_zjets->Integral());

  h_mva_top->SetMarkerStyle(kFullCircle);

  h_mva_zjets->SetMarkerStyle(kFullCircle);
  h_mva_zjets->SetMarkerColor(kRed+1);
  h_mva_zjets->SetLineColor  (kRed+1);


  // Read fakes file
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<jetet.size(); i++) {

    TH1F* h = (TH1F*)fileFakes->Get(Form("%s_%d", hname.Data(), jetet.at(i)));

    h->Scale(1. / h->Integral());
    
    h->SetLineColor(color.at(i));

    h->SetLineWidth(2);

    fakes.push_back(h);
  }


  // Draw top and Z+jets
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("c1", "c1");

  h_mva_top->Draw("ep");

  AxisLabels(h_mva_top);

  h_mva_zjets->Draw("ep,same");

  ndelta = 0;
  xdelta = 0;

  DrawLegend(x0 + xdelta, y0 - ndelta, (TObject*)h_mva_top,   " top",    "p"); ndelta += delta;
  DrawLegend(x0 + xdelta, y0 - ndelta, (TObject*)h_mva_zjets, " Z+jets", "p"); ndelta += delta;

  SaveCanvas(c1, "isolation-top-zjets");


  // Draw top and fakes
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("c2", "c2");

  h_mva_top->Draw("ep");

  AxisLabels(h_mva_top);

  ndelta = 0;
  xdelta = 0;

  DrawLegend(x0 + xdelta, y0 - ndelta, (TObject*)h_mva_top, " top", "p"); ndelta += delta;

  ndelta = 0;
  xdelta = 0.17;

  for (UInt_t i=0; i<jetet.size(); i++)
    {
      (fakes.at(i))->Draw("hist,same");

      DrawLegend(x0 + xdelta, y0 - ndelta, (TObject*)fakes.at(i), Form(" jet%d", jetet.at(i)), "l"); ndelta += delta;
    }

  h_mva_top->Draw("ep,same");

  SaveCanvas(c2, "isolation-top-fakes");


  // Draw Z+jets and fakes
  //----------------------------------------------------------------------------
  TCanvas* c3 = new TCanvas("c3", "c3");

  h_mva_zjets->Draw("ep");

  AxisLabels(h_mva_zjets);

  ndelta = 0;
  xdelta = 0;

  DrawLegend(x0 + xdelta, y0 - ndelta, (TObject*)h_mva_zjets, " Z+jets", "p"); ndelta += delta;

  ndelta = 0;
  xdelta = 0.17;

  for (UInt_t i=0; i<jetet.size(); i++)
    {
      (fakes.at(i))->Draw("hist,same");

      DrawLegend(x0 + xdelta, y0 - ndelta, (TObject*)fakes.at(i), Form(" jet%d", jetet.at(i)), "l"); ndelta += delta;
    }

  h_mva_zjets->Draw("ep,same");

  SaveCanvas(c3, "isolation-zjets-fakes");
}


//------------------------------------------------------------------------------
// AxisLabels
//------------------------------------------------------------------------------
void AxisLabels(TH1F* hist)
{
  TAxis* xaxis = hist->GetXaxis();
  TAxis* yaxis = hist->GetYaxis();

  xaxis->SetLabelFont  (  42);
  xaxis->SetLabelOffset(0.01);
  xaxis->SetLabelSize  (0.05);
  xaxis->SetNdivisions ( 505);
  xaxis->SetTitleFont  (  42);
  xaxis->SetTitleOffset( 1.3);
  xaxis->SetTitleSize  (0.05);

  yaxis->SetLabelFont  (  42);
  yaxis->SetLabelOffset(0.01);
  yaxis->SetLabelSize  (0.05);
  yaxis->SetNdivisions ( 505);
  yaxis->SetTitleFont  (  42);
  yaxis->SetTitleOffset( 1.8);
  yaxis->SetTitleSize  (0.05);

  hist->SetMaximum(0.08);

  xaxis->SetTitle("MVA isolation");
  yaxis->SetTitle("normalized entries");
}


//------------------------------------------------------------------------------
// SaveCanvas
//------------------------------------------------------------------------------
void SaveCanvas(TCanvas* canvas, TString cname)
{
  canvas->cd();

  DrawTLatex(61, 0.190, 0.94, 0.055, 11, "CMS");
  DrawTLatex(52, 0.310, 0.94, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.940, 0.94, 0.040, 31, "(8TeV)");
  
  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("pdf/" + cname + ".pdf");
  canvas->SaveAs("png/" + cname + ".png");
}
