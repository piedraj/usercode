//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// xs ratios
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "TAxis.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TSystem.h"


// Constants
//------------------------------------------------------------------------------
const UInt_t nChannel = 5;

const Float_t xs7eval[] = {23.00, 19.67, 19.81, 21.02, 20.14};
const Float_t xs7stat[] = { 3.10,  2.73,  2.60,  2.30,  1.32};
const Float_t xs7syst[] = { 1.39,  1.50,  1.55,  1.47,  1.13};
const Float_t xs7lumi[] = { 0.51,  0.43,  0.44,  0.46,  0.44};

const Float_t xs8eval[] = {24.80, 22.38, 23.94, 24.93, 24.07};
const Float_t xs8stat[] = { 1.92,  1.62,  1.52,  1.29,  0.84};
const Float_t xs8syst[] = { 1.74,  2.08,  2.39,  2.34,  1.62};
const Float_t xs8lumi[] = { 0.64,  0.58,  0.62,  0.65,  0.63};

const TString lChannel[nChannel] = {
  "eee",
  "ee#mu",
  "#mu#mue",
  "#mu#mu#mu",
  "combined"
};

const Float_t xs_nlo      [] = {17.8, 21.9};
const Float_t xs_nlo_left [] = { 0.5,  0.9};
const Float_t xs_nlo_right[] = { 0.7,  1.2};


// Functions
//------------------------------------------------------------------------------
void     DrawTLatex (Font_t      tfont,
		     Float_t     x,
		     Float_t     y,
		     Float_t     tsize,
		     Short_t     align,
		     const char* text,
		     Bool_t      setndc = true);

TLegend* DrawTLegend(Float_t     x1,
		     Float_t     y1,
		     TObject*    hist,
		     TString     label,
		     TString     option,
		     Float_t     tsize   = 0.03,
		     Float_t     xoffset = 0.3,
		     Float_t     yoffset = 0.05);


//------------------------------------------------------------------------------
// drawFigure2
//------------------------------------------------------------------------------
void drawFigure2()
{
  gInterpreter->ExecuteMacro("WZPaperStyle.C");
  
  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);


  TGraphErrors* g7stat = new TGraphErrors(nChannel);
  TGraphErrors* g7syst = new TGraphErrors(nChannel);
  TGraphErrors* g7lumi = new TGraphErrors(nChannel);

  TGraphErrors* g8stat = new TGraphErrors(nChannel);
  TGraphErrors* g8syst = new TGraphErrors(nChannel);
  TGraphErrors* g8lumi = new TGraphErrors(nChannel);
  

  // Fill the 7 TeV graphs
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++)
    {
      Float_t errorSquared = (xs7stat[i] * xs7stat[i]);
      
      Float_t xs = xs_nlo[0];

      g7stat->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
      
      errorSquared += (xs7syst[i] * xs7syst[i]);

      g7syst->SetPointError(i, sqrt(errorSquared) / xs, 0.25);

      errorSquared += (xs7lumi[i] * xs7lumi[i]);

      g7lumi->SetPointError(i, sqrt(errorSquared) / xs, 0.25);

      g7stat->SetPoint(i, xs7eval[i] / xs, nChannel-i);
      g7syst->SetPoint(i, xs7eval[i] / xs, nChannel-i);
      g7lumi->SetPoint(i, xs7eval[i] / xs, nChannel-i);
    }


  // Fill the 8 TeV graphs
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++)
    {
      Float_t errorSquared = (xs8stat[i] * xs8stat[i]);
      
      Float_t xs = xs_nlo[1];

      g8stat->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
      
      errorSquared += (xs8syst[i] * xs8syst[i]);

      g8syst->SetPointError(i, sqrt(errorSquared) / xs, 0.25);

      errorSquared += (xs8lumi[i] * xs8lumi[i]);

      g8lumi->SetPointError(i, sqrt(errorSquared) / xs, 0.25);

      g8stat->SetPoint(i, xs8eval[i] / xs, 2*nChannel-i);
      g8syst->SetPoint(i, xs8eval[i] / xs, 2*nChannel-i);
      g8lumi->SetPoint(i, xs8eval[i] / xs, 2*nChannel-i);
    }


  // 7 TeV cosmetics
  //----------------------------------------------------------------------------
  g7stat->SetFillColor  (kRed+1);
  g7stat->SetLineColor  (kRed+1);
  g7stat->SetLineWidth  (0);
  g7stat->SetMarkerColor(kWhite);
  g7stat->SetMarkerSize (0.8);
  g7stat->SetMarkerStyle(kFullCircle);

  g7syst->SetFillColor(kOrange+7);
  g7syst->SetLineColor(kOrange+7);
  g7syst->SetLineWidth(0);

  g7lumi->SetFillColor(kOrange+7);
  g7lumi->SetLineColor(kOrange+7);
  g7lumi->SetLineWidth(0);


  // 8 TeV cosmetics
  //----------------------------------------------------------------------------
  g8stat->SetFillColor  (kBlue);
  g8stat->SetLineColor  (kBlue);
  g8stat->SetLineWidth  (0);
  g8stat->SetMarkerColor(kWhite);
  g8stat->SetMarkerSize (0.8);
  g8stat->SetMarkerStyle(kFullTriangleUp);

  g8syst->SetFillColor(kAzure-4);
  g8syst->SetLineWidth(0);
  g8syst->SetLineColor(kAzure-4);

  g8lumi->SetFillColor(kAzure-4);
  g8lumi->SetLineWidth(0);
  g8lumi->SetLineColor(kAzure-4);


  // Draw
  //----------------------------------------------------------------------------
  TString cname = "xs_ratios";

  TCanvas* canvas = new TCanvas(cname, cname);

  canvas->SetLeftMargin(canvas->GetRightMargin());

  Float_t xmin = 0.2;
  Float_t xmax = 2.0;
  Float_t ymin = 0.2;
  Float_t ymax = 2*nChannel + ymin + 0.6;
  
  TH2F* h2 = new TH2F("h2", "", 100, xmin, xmax, 100, ymin, ymax);

  h2->Draw();
  
  
  // NLO WZ cross-section
  //----------------------------------------------------------------------------
  TBox* nlo7tev = new TBox(1 - xs_nlo_left [0] / xs_nlo[0], (ymax-ymin)/2,
			   1 + xs_nlo_right[0] / xs_nlo[0], ymax);

  TBox* nlo8tev = new TBox(1 - xs_nlo_left [1] / xs_nlo[1], ymin,
			   1 + xs_nlo_right[1] / xs_nlo[1], (ymax-ymin)/2);

  nlo7tev->SetLineColor(0);
  nlo7tev->SetFillColor(kGray);
  nlo7tev->SetFillStyle(1001);

  nlo8tev->SetLineColor(0);
  nlo8tev->SetFillColor(kGray);
  nlo8tev->SetFillStyle(1001);

  nlo7tev->Draw("e2,same");
  nlo8tev->Draw("e2,same");


  // Cross sections
  //----------------------------------------------------------------------------
  g7lumi->Draw("p2,same");
  g7syst->Draw("p2,same");
  g7stat->Draw("p2,same");

  g8lumi->Draw("p2,same");
  g8syst->Draw("p2,same");
  g8stat->Draw("p2,same");


  // 7 TeV labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {

    Float_t x = g7stat->GetX()[i];
    Float_t y = g7stat->GetY()[i];

    Float_t g7lumiError = g7lumi->GetErrorX(i);

    DrawTLatex(42, xmin+0.07, y, 0.03, 12, Form("%s %.2f #pm %.2f",
						lChannel[i].Data(), x, g7lumiError), 0);

  }


  // 8 TeV labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {

    Float_t x = g8stat->GetX()[i];
    Float_t y = g8stat->GetY()[i];

    Float_t g8lumiError = g8lumi->GetErrorX(i);

    DrawTLatex(42, xmin+0.06, y, 0.03, 12, Form("%s %.2f #pm %.2f",
						lChannel[i].Data(), x, g8lumiError), 0);
  }


  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(61, 0.065, 0.94, 0.055, 11, "CMS");
  DrawTLatex(52, 0.190, 0.94, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.940, 0.94, 0.040, 31, "4.9 fb^{-1} (7 TeV) + 19.6 fb^{-1} (8 TeV)");

  h2->GetXaxis()->CenterTitle();
  h2->GetXaxis()->SetTitleOffset(1.4);
  h2->GetXaxis()->SetTitle("#sigma_{WZ}^{exp} / #sigma_{WZ}^{NLO}");
  h2->GetYaxis()->SetTitle("");


  // Remove y-axis labels
  //----------------------------------------------------------------------------
  TAxis* yaxis = h2->GetYaxis();
  
  for (Int_t j=1; j<yaxis->GetNbins(); j++) yaxis->SetBinLabel(j, "");


  // Additional legend
  //----------------------------------------------------------------------------
  DrawTLatex(42, 0.775, 0.805, 0.03, 11, "8 TeV");
  DrawTLegend(0.76, 0.750, nlo8tev, "theory", "f");
  DrawTLegend(0.76, 0.700, g8stat,  "stat",   "fp");
  DrawTLegend(0.76, 0.650, g8syst,  "syst",   "f");

  DrawTLatex(42, 0.775, 0.455, 0.03, 11, "7 TeV");
  DrawTLegend(0.76, 0.400, nlo7tev, "theory", "f");
  DrawTLegend(0.76, 0.350, g7stat,  "stat",   "fp");
  DrawTLegend(0.76, 0.300, g7syst,  "syst",   "f");


  // Save
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();

  canvas->SaveAs("pdf/" + cname + ".pdf");
  canvas->SaveAs("png/" + cname + ".png");
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Font_t      tfont,
		Float_t     x,
		Float_t     y,
		Float_t     tsize,
		Short_t     align,
		const char* text,
		Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}


//------------------------------------------------------------------------------
// DrawTLegend
//------------------------------------------------------------------------------
TLegend* DrawTLegend(Float_t  x1,
		     Float_t  y1,
		     TObject* hist,
		     TString  label,
		     TString  option,
		     Float_t  tsize,
		     Float_t  xoffset,
		     Float_t  yoffset)
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
