#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"


const Double_t _yoffset = 0.045;

TString xtitle;
TString ytitle;
Float_t xmin;
Float_t xmax;
Float_t ymin;
Float_t ymax;


//------------------------------------------------------------------------------
// drawFigure4
//------------------------------------------------------------------------------
void drawFigure4(TString parameter = "lam0")
{
  gInterpreter->ExecuteMacro("WZPaperStyle.C");

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);


  // Individual settings
  //----------------------------------------------------------------------------
  if (parameter.Contains("dg0"))
    {
      xtitle = "#Delta#kappa^{Z}";
      ytitle = "#lambda";
      xmin   = -0.5;
      xmax   =  0.5;
      ymin   = -0.039;
      ymax   =  0.053;
    }
  else if (parameter.Contains("lam0"))
    {
      xtitle = "#Delta#kappa^{Z}";
      ytitle = "#Deltag^{Z}_{1}";
      xmin   = -0.53;
      xmax   =  0.53;
      ymin   = -0.05;
      ymax   =  0.09;
    }
  else if (parameter.Contains("dk0"))
    {
      xtitle = "#Deltag^{Z}_{1}";
      ytitle = "#lambda";
      xmin   = -0.059;
      xmax   =  0.059;
      ymin   = -0.037;
      ymax   =  0.052;
    }


  // Read the input file
  //----------------------------------------------------------------------------
  TFile* file = new TFile("rootfiles/contours_" + parameter + "_2Dpol2.root", "read");

  TGraph* cont_exp_68 = (TGraph*)file->Get("cont_exp_68");
  TGraph* cont_exp_95 = (TGraph*)file->Get("cont_exp_95");
  TGraph* cont_exp_99 = (TGraph*)file->Get("cont_exp_99");
  TGraph* cont_obs_95 = (TGraph*)file->Get("cont_obs_95");
  TGraph* bestFit     = (TGraph*)file->Get("bestFit");

  bestFit->SetMarkerSize(1.5);
  bestFit->SetMarkerStyle(34);

  TGraph* g_large = new TGraph(2);

  g_large->SetPoint(0, -1.0, 1.0);
  g_large->SetPoint(1,  1.0, 1.0);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("canvas", "canvas");

  TMultiGraph* mg = new TMultiGraph();

  mg->Add(cont_exp_68);
  mg->Add(cont_exp_95);
  mg->Add(cont_exp_99);
  mg->Add(cont_obs_95);
  mg->Add(bestFit, "p");
  mg->Add(g_large);

  mg->Draw("ac");

  mg->SetMinimum(ymin);
  mg->SetMaximum(ymax);


  // Axis labels
  //----------------------------------------------------------------------------
  TAxis* xaxis = mg->GetXaxis();
  TAxis* yaxis = mg->GetYaxis();
  
  xaxis->SetLabelFont  (    42);
  xaxis->SetLabelOffset(  0.01);
  xaxis->SetLabelSize  (  0.05);
  xaxis->SetNdivisions (   505);
  xaxis->SetTitle      (xtitle);
  xaxis->SetTitleFont  (    42);
  xaxis->SetTitleOffset(   1.2);
  xaxis->SetTitleSize  (  0.05);

  xaxis->SetLimits(xmin, xmax);

  yaxis->SetLabelFont  (    42);
  yaxis->SetLabelOffset(  0.01);
  yaxis->SetLabelSize  (  0.05);
  yaxis->SetNdivisions (   505);
  yaxis->SetTitle      (ytitle);
  yaxis->SetTitleFont  (    42);
  yaxis->SetTitleOffset(   1.6);
  yaxis->SetTitleSize  (  0.05);

  canvas->Modified();


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0 = 0.580;
  Double_t y0 = 0.755;

  DrawTLegend(x0 - 0.36, y0 + 2.*(_yoffset+0.001), (TH1F*)cont_exp_68, " Expected 68% C.L.", "l");
  DrawTLegend(x0 - 0.36, y0 + 1.*(_yoffset+0.001), (TH1F*)cont_exp_95, " Expected 95% C.L.", "l");
  DrawTLegend(x0,        y0 + 2.*(_yoffset+0.001), (TH1F*)cont_exp_99, " Expected 99% C.L.", "l");
  DrawTLegend(x0,        y0 + 1.*(_yoffset+0.001), (TH1F*)cont_obs_95, " Observed 95% C.L.", "l");
  DrawTLegend(x0,        y0,                       (TH1F*)bestFit,     " Best fit",          "p");


  // Finish it
  //----------------------------------------------------------------------------
  DrawTLatex(_cmsTextFont,   0.190, 0.94, 0.055, 11, "CMS");
  DrawTLatex(_extraTextFont, 0.315, 0.94, 0.030, 11, "Preliminary");
  DrawTLatex(_lumiTextFont,  0.940, 0.94, 0.040, 31, "19.6 fb^{-1} (8 TeV)");

  canvas->SaveAs("pdf/lZ_dkg_2dlimit_" + parameter + "_2Dpol2_deltaNLL.pdf");
  canvas->SaveAs("png/lZ_dkg_2dlimit_" + parameter + "_2Dpol2_deltaNLL.png");
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
		     Float_t xoffset = 0.34,
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
