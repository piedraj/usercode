//------------------------------------------------------------------------------
//
// drawFakes by Jonatan Piedra
//
//------------------------------------------------------------------------------
#include "DrawFunctions.h"


const Float_t _bigLabelSize = 0.04;


TFile* inputfile;


// Member functions
//------------------------------------------------------------------------------
void DrawIt       (Int_t   jetet);

void AxisFonts    (TAxis*  axis,
		   TString title);

void TH2FAxisFonts(TH2F*   h,
		   TString coordinate,
		   TString title);


//------------------------------------------------------------------------------
// drawFakes
//------------------------------------------------------------------------------
void drawFakes()
{
  inputfile = new TFile("rootfiles/muonFakeRate.root", "read");

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);

  DrawIt(20);
  DrawIt(40);
  DrawIt(60);
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(Int_t jetet)
{
  TH2F* hnum = (TH2F*)inputfile->Get(Form("h_pt_eta_tight_%d", jetet));
  TH2F* hden = (TH2F*)inputfile->Get(Form("h_pt_eta_loose_%d", jetet));

  hnum->Divide(hden);

  TString hname = Form("h_pt_eta_fr_%d", jetet);

  TH2F* h = (TH2F*)hnum->Clone(hname);

  h->SetDirectory(0);

  TCanvas* canvas = new TCanvas(hname, hname, 600, 600);

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TH2FAxisFonts(h, "x", "p_{T} (GeV)");
  TH2FAxisFonts(h, "y", "#eta");

  h->Draw("colz");

  h->SetTitle("");

  DrawTLatex(42, 0.940, 0.976, _bigLabelSize, 33, Form("muon FR jet%d (8 TeV)", jetet));

  Double_t hmin = h->GetMinimum();
  Double_t hmax = h->GetMaximum();

  for (Int_t i=1; i<=h->GetNbinsX(); i++) {
    for (Int_t j=1; j<=h->GetNbinsY(); j++) {

      Double_t value = h->GetBinContent(i,j);
      
      Double_t ypos = h->GetYaxis()->GetBinCenter(j);
      Double_t xpos = h->GetXaxis()->GetBinCenter(i);
      
      TLatex* latex = new TLatex(xpos, ypos, Form("%.2f", value));

      latex->SetTextAlign(   22);
      latex->SetTextFont (   42);
      latex->SetTextSize (0.027);

      if (value < hmin + 0.3*(hmax - hmin)) latex->SetTextColor(kWhite);
	
      latex->Draw();
    }
  }


  // Set the palette font
  //----------------------------------------------------------------------------
  canvas->Update();

  TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");

  palette->SetLabelFont(42);


  // Save the plot
  //----------------------------------------------------------------------------
  canvas->Update();
  
  canvas->Modified();

  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("pdf/" + hname + ".pdf");
  canvas->SaveAs("png/" + hname + ".png");
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis*  axis,
	       TString title)
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
// TH2FAxisFonts
//------------------------------------------------------------------------------
void TH2FAxisFonts(TH2F*   h,
		   TString coordinate,
		   TString title)
{
  TAxis* axis = NULL;

  if (coordinate.Contains("x")) axis = h->GetXaxis();
  if (coordinate.Contains("y")) axis = h->GetYaxis();

  AxisFonts(axis, title);
}
