#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"


// Constants
//------------------------------------------------------------------------------
const Font_t   _cmsTextFont   = 61;
const Font_t   _extraTextFont = 52;
const Font_t   _lumiTextFont  = 42;
const Double_t _yoffset       = 0.042;

const Float_t xs7eval[] = {22.46, 19.04, 19.13, 20.36, 20.14};
const Float_t xs7stat[] = { 3.12,  2.75,  2.60,  2.31,  1.32};
const Float_t xs7syst[] = { 1.40,  1.54,  1.61,  1.53,  1.13};
const Float_t xs7lumi[] = { 0.49,  0.42,  0.42,  0.45,  0.44};

const Float_t xs8eval[] = {24.80, 22.38, 23.94, 24.93, 24.09};
const Float_t xs8stat[] = { 1.92,  1.62,  1.52,  1.29,  0.87};
const Float_t xs8syst[] = { 1.74,  1.92,  1.85,  2.29,  1.62};
const Float_t xs8lumi[] = { 0.64,  0.58,  0.62,  0.65,  0.63};


// Functions
//------------------------------------------------------------------------------
Float_t  GetMaximumIncludingErrors(TH1F*       h);

void     DrawTLatex               (Font_t      tfont,
				   Double_t    x,
				   Double_t    y,
				   Double_t    tsize,
				   Short_t     align,
				   const char* text,
				   Bool_t      setndc = true);

TLegend* DrawTLegend              (Float_t     x1,
				   Float_t     y1,
				   TH1*        hist,
				   TString     label,
				   TString     option,
				   Float_t     tsize   = 0.03,
				   Float_t     xoffset = 0.18,
				   Float_t     yoffset = _yoffset);

void     DrawZPeak                (TString     energy);


// Data members
//------------------------------------------------------------------------------
TString _lumiText;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// drawFigure2
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void drawFigure2()
{
  gInterpreter->ExecuteMacro("WZPaperStyle.C");

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);

  DrawZPeak("7TeV");
  DrawZPeak("8TeV");
}


//------------------------------------------------------------------------------
// DrawZPeak
//------------------------------------------------------------------------------
void DrawZPeak(TString energy)
{
  if (energy.Contains("7TeV"))
    {
      _lumiText = "4.9 fb^{-1} (7 TeV)";
    }
  else
    {
      _lumiText = "19.6 fb^{-1} (8 TeV)";
    }


  // Read the input file
  //----------------------------------------------------------------------------
  TString name = "invMass2Lep_";

  TFile* file = new TFile("rootfiles/" + name + energy + ".root", "read");

  TH1F* data;
  TH1F* WZ;
  TH1F* fakes;
  TH1F* ZZ;
  TH1F* Zgamma;
  TH1F* WV;
  TH1F* VVV;
  TH1F* allmc;

  if (energy.Contains("7TeV"))
    {
      data   = (TH1F*)file->Get("hZMass_Sel_datahist_COMB");
      WZ     = (TH1F*)file->Get("hZMass_Sel_wz_COMB");
      fakes  = (TH1F*)file->Get("hZMass_Sel_datadriven_COMB");
      ZZ     = (TH1F*)file->Get("hZMass_Sel_zz_COMB");
      Zgamma = (TH1F*)file->Get("hZMass_Sel_zg_COMB");
      WV     = (TH1F*)file->Get("hZMass_Sel_wz_COMB");
      VVV    = (TH1F*)file->Get("hZMass_Sel_wz_COMB");
      allmc  = (TH1F*)file->Get("all_estimates_with_error_COMB");
    }
  else if (energy.Contains("8TeV"))
    {
      data   = (TH1F*)file->Get("h_data");
      WZ     = (TH1F*)file->Get("h_WZ");
      fakes  = (TH1F*)file->Get("h_Fakes");
      ZZ     = (TH1F*)file->Get("h_ZZ");
      Zgamma = (TH1F*)file->Get("h_ZGamma");
      WV     = (TH1F*)file->Get("h_WV");
      VVV    = (TH1F*)file->Get("h_VVV");
      allmc  = (TH1F*)file->Get("h_All");
    }

  WZ->SetFillColor(kOrange-2);
  WZ->SetLineColor(kOrange-2);

  Zgamma->SetFillColor(kRed+1);  // kRed+2
  Zgamma->SetLineColor(kRed+1);  // kRed+2

  ZZ->SetFillColor(kRed+1);
  ZZ->SetLineColor(kRed+1);

  fakes->SetFillColor(kGray+1);
  fakes->SetLineColor(kGray+1);
  
  data->SetMarkerStyle(kFullCircle);

  allmc->SetFillColor  (kBlack);
  allmc->SetFillStyle  (3345);
  allmc->SetLineColor  (kWhite);
  allmc->SetLineWidth  (0);
  allmc->SetMarkerColor(kOrange-2);
  allmc->SetMarkerSize (0);

  THStack* hs = new THStack();

  if (energy.Contains("8TeV"))
    {
      WV->SetFillColor(kRed+1);  // kAzure
      WV->SetLineColor(kRed+1);  // kAzure

      VVV->SetFillColor(kRed+1);  // kBlack
      VVV->SetLineColor(kRed+1);  // kBlack

      hs->Add(VVV);
      hs->Add(WV);
    }

  hs->Add(Zgamma);
  hs->Add(ZZ);
  hs->Add(fakes);
  hs->Add(WZ);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(energy, energy);

  data->Draw("ep");


  // Axis labels
  //----------------------------------------------------------------------------
  TAxis* xaxis = data->GetXaxis();
  TAxis* yaxis = data->GetYaxis();
  
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
  yaxis->SetTitleOffset( 1.6);
  yaxis->SetTitleSize  (0.05);

  xaxis->SetRangeUser(68, 112);
  xaxis->SetTitle("m_{#font[12]{ll}} (GeV)");
  yaxis->SetTitle(Form("Events /  %.0f GeV", data->GetBinWidth(0)));


  // Adjust scale
  //----------------------------------------------------------------------------
  Float_t theMax   = GetMaximumIncludingErrors(data);
  Float_t theMaxMC = GetMaximumIncludingErrors(allmc);

  if (theMaxMC > theMax) theMax = theMaxMC;

  data->SetMaximum(1.15 * theMax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0 = 0.635;
  Double_t y0 = 0.770;

  DrawTLegend(x0, y0 + 2.*(_yoffset+0.001), data,  " Data",               "ep");
  DrawTLegend(x0, y0 + 1.*(_yoffset+0.001), WZ,    " WZ",                 "f");
  DrawTLegend(x0, y0,                       fakes, " Nonprompt leptons",  "f");
  DrawTLegend(x0, y0 - 1.*(_yoffset+0.001), ZZ,    " MC background",      "f");
  DrawTLegend(x0, y0 - 2.*(_yoffset+0.001), allmc, " stat. #oplus syst.", "f");


  // Finish it
  //----------------------------------------------------------------------------
  data->SetTitle("");

  DrawTLatex(_cmsTextFont,   0.215, 0.880, 0.055, 13, "CMS");
  //  DrawTLatex(_extraTextFont, 0.215, 0.826, 0.030, 13, "Preliminary");
  DrawTLatex(_lumiTextFont,  0.940, 0.940, 0.040, 31, _lumiText);

  hs   ->Draw("hist,same");
  allmc->Draw("e2,same");
  data ->Draw("ep,same");

  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();
  canvas->Update();
  
  canvas->SaveAs("pdf/" + name + energy + ".pdf");
  canvas->SaveAs("png/" + name + energy + ".png");
}


//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Float_t GetMaximumIncludingErrors(TH1F* h)
{
  Float_t maxWithErrors = 0;

  for (Int_t i=1; i<=h->GetNbinsX(); i++) {

    Float_t binHeight = h->GetBinContent(i) + h->GetBinError(i);

    if (binHeight > maxWithErrors) maxWithErrors = binHeight;
  }

  return maxWithErrors;
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Font_t      tfont,
		Double_t    x,
		Double_t    y,
		Double_t    tsize,
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
TLegend* DrawTLegend(Float_t x1,
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
