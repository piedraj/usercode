#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"


const Double_t _yoffset = 0.045;


//------------------------------------------------------------------------------
// drawFigure3
//------------------------------------------------------------------------------
void drawFigure3(Int_t logy = 0)
{
  gInterpreter->ExecuteMacro("WZPaperStyle.C");

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);


  // Read the input file
  //----------------------------------------------------------------------------
  TString name = "WZ_PtZ_plot_allCh_largeATGC";

  TFile* file = new TFile("rootfiles/" + name + ".root", "read");

  TCanvas* c1 = (TCanvas*)file->Get("c1_allCh");

  TH1F* data     = (TH1F*)c1->FindObject("histo_data_3e");
  TH1F* aTGC_dk  = (TH1F*)c1->FindObject("histo_aTGC_dk_3e");
  TH1F* aTGC_lam = (TH1F*)c1->FindObject("histo_aTGC_lam_3e");
  TH1F* aTGC_dg  = (TH1F*)c1->FindObject("histo_aTGC_dg_3e");
  TH1F* WZ       = (TH1F*)c1->FindObject("histo_SM_3e");
  TH1F* fakes    = (TH1F*)c1->FindObject("fake_0");
  TH1F* ZZ       = (TH1F*)c1->FindObject("total_bkg_rebined_ZZ_0");
  TH1F* Zgamma   = (TH1F*)c1->FindObject("total_bkg_rebined_Zgamma_0");
  TH1F* WV       = (TH1F*)c1->FindObject("total_bkg_rebined_WV_0");
  TH1F* VVV      = (TH1F*)c1->FindObject("total_bkg_rebined_VVV_0");

  WZ->SetFillColor(kOrange-2);
  WZ->SetLineColor(kOrange-2);

  Zgamma->SetFillColor(kRed+2);
  Zgamma->SetLineColor(kRed+2);

  ZZ->SetFillColor(kRed+1);
  ZZ->SetLineColor(kRed+1);

  fakes->SetFillColor(kGray+1);
  fakes->SetLineColor(kGray+1);

  WV->SetFillColor(kAzure);
  WV->SetLineColor(kAzure);

  VVV->SetFillColor(kBlack);
  VVV->SetLineColor(kBlack);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("canvas", "canvas");

  canvas->SetLogy(logy);

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
  xaxis->SetTitleOffset( 1.2);
  xaxis->SetTitleSize  (0.05);

  yaxis->SetLabelFont  (  42);
  yaxis->SetLabelOffset(0.01);
  yaxis->SetLabelSize  (0.05);
  yaxis->SetNdivisions ( 505);
  yaxis->SetTitleFont  (  42);
  yaxis->SetTitleOffset( 1.6);
  yaxis->SetTitleSize  (0.05);

  xaxis->SetTitle("p_{T}^{Z} (GeV)");
  yaxis->SetTitle(Form("Events /  %.0f GeV", data->GetBinWidth(0)));


  // Adjust scale
  //----------------------------------------------------------------------------
  Float_t theMax   = GetMaximumIncludingErrors(data);
  Float_t theMaxMC = GetMaximumIncludingErrors(aTGC_dk);

  if (theMaxMC > theMax) theMax = theMaxMC;

  if (canvas->GetLogy()) {
    data->SetMaximum(15 * theMax);
    data->SetMinimum(1);
  } else {
    data->SetMaximum(1.2 * theMax);
  }


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0;
  Double_t y0 = 0.755;

  if (logy)
    {
      x0 = 0.68;

      DrawTLegend(x0 - 0.37, y0 + 2.*(_yoffset+0.001), data,     " data",                              "ep");
      DrawTLegend(x0 - 0.37, y0 + 1.*(_yoffset+0.001), aTGC_dk,  " WZ aTGC (#Delta#kappa^{Z} = 0.6)",  "l");
      DrawTLegend(x0 - 0.37, y0,                       aTGC_dg,  " WZ aTGC (#Deltag^{Z}_{1} = -0.06)", "l");
      DrawTLegend(x0 - 0.37, y0 - 1.*(_yoffset+0.001), aTGC_lam, " WZ aTGC (#lambda = 0.04)",          "l");
      DrawTLegend(x0,        y0 + 2.*(_yoffset+0.001), WZ,       " WZ",                                "f");
      DrawTLegend(x0,        y0 + 1.*(_yoffset+0.001), fakes,    " top and Z+jets",                    "f");
      DrawTLegend(x0,        y0,                       ZZ,       " ZZ",                                "f");
      DrawTLegend(x0,        y0 - 1.*(_yoffset+0.001), Zgamma,   " Z#gamma",                           "f");
      DrawTLegend(x0,        y0 - 2.*(_yoffset+0.001), WV,       " W#gamma*",                          "f");
      DrawTLegend(x0,        y0 - 3.*(_yoffset+0.001), VVV,      " VVV",                               "f");
    }
  else
    {
      x0 = 0.57;

      DrawTLegend(x0, y0 + 2.*(_yoffset+0.001), data,     " data",                              "ep");
      DrawTLegend(x0, y0 + 1.*(_yoffset+0.001), aTGC_dk,  " WZ aTGC (#Delta#kappa^{Z} = 0.6)",  "l");
      DrawTLegend(x0, y0,                       aTGC_dg,  " WZ aTGC (#Deltag^{Z}_{1} = -0.06)", "l");
      DrawTLegend(x0, y0 - 1.*(_yoffset+0.001), aTGC_lam, " WZ aTGC (#lambda = 0.04)",          "l");
      DrawTLegend(x0, y0 - 2.*(_yoffset+0.001), WZ,       " WZ",                                "f");
      DrawTLegend(x0, y0 - 3.*(_yoffset+0.001), fakes,    " top and Z+jets",                    "f");
      DrawTLegend(x0, y0 - 4.*(_yoffset+0.001), ZZ,       " ZZ",                                "f");
      DrawTLegend(x0, y0 - 5.*(_yoffset+0.001), Zgamma,   " Z#gamma",                           "f");
      DrawTLegend(x0, y0 - 6.*(_yoffset+0.001), WV,       " W#gamma*",                          "f");
      DrawTLegend(x0, y0 - 7.*(_yoffset+0.001), VVV,      " VVV",                               "f");
    }


  // Finish it
  //----------------------------------------------------------------------------
  data->SetTitle("");

  if (logy)
    {
      DrawTLatex(_cmsTextFont,   0.190, 0.94, 0.055, 11, "CMS");
      DrawTLatex(_extraTextFont, 0.315, 0.94, 0.030, 11, "Preliminary");
    }
  else
    {
      DrawTLatex(_cmsTextFont,   0.215, 0.891, 0.055, 13, "CMS");
      DrawTLatex(_extraTextFont, 0.215, 0.837, 0.030, 13, "Preliminary");
    }

  DrawTLatex(_lumiTextFont, 0.940, 0.94, 0.040, 31, "19.6 fb^{-1} (8 TeV)");

  WZ      ->Draw("hist,same");
  aTGC_dk ->Draw("hist,same");
  aTGC_lam->Draw("hist,same");
  aTGC_dg ->Draw("hist,same");
  fakes   ->Draw("hist,same");
  ZZ      ->Draw("hist,same");
  Zgamma  ->Draw("hist,same");
  WV      ->Draw("hist,same");
  VVV     ->Draw("hist,same");
  data    ->Draw("ep,same");

  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();
  canvas->Update();
  
  TString cname = name;

  if (logy) cname += "_log_range";

  canvas->SaveAs("pdf/" + cname + ".pdf");
  canvas->SaveAs("png/" + cname + ".png");
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
		const char* text)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC();
  tl->SetTextAlign(align);
  tl->SetTextFont (tfont);
  tl->SetTextSize (tsize);

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
		     Float_t tsize   = 0.03,
		     Float_t xoffset = 0.20,
		     Float_t yoffset = _yoffset)
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
