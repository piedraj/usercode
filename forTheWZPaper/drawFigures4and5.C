#include "TROOT.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TPad.h"
#include "TMath.h"
#include "TColor.h"


// Constants
//------------------------------------------------------------------------------
const Font_t   _cmsTextFont   =  61;
const Font_t   _extraTextFont =  52;
const Font_t   _lumiTextFont  =  42;
const Size_t   _msize         = 1.0;
const Double_t _yoffset       = 0.042;


// Functions
//------------------------------------------------------------------------------
void     DrawLatex               (Font_t      tfont,
				  Double_t    x,
				  Double_t    y,
				  Double_t    tsize,
				  Short_t     align,
				  const char* text,
				  Bool_t      setndc = true);

TLegend* DrawLegend              (Float_t     x1,
				  Float_t     y1,
				  TH1*        hist,
				  TString     label,
				  TString     option,
				  Float_t     tsize   = 0.05,
				  Float_t     xoffset = 0.18,
				  Float_t     yoffset = _yoffset);

void     AxisFonts               (TAxis*      axis,
				  TString     title);

void AxisFontsRatio              (TAxis*  axis,
				  TString which,
				  TString title);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// drawFigures4and5
//
// Zpt
// LeadingJetPt
// Njets
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void drawFigures4and5(TString var = "LeadingJetPt") 
{
  Bool_t drawMcfm = (var.EqualTo("Zpt")) ? true : false;

  TString variable;

  if (var.EqualTo("Zpt"))          variable = "Z";
  if (var.EqualTo("LeadingJetPt")) variable = "leading jet";
  if (var.EqualTo("Njets"))        variable = "N_{jets}";

  TString xtitle = (var.EqualTo("Njets")) ? variable : "p_{T}^{" + variable + "} (GeV)";
  TString ytitle = "d#sigma(WZ#rightarrow3l#nu)/dp_{T}^{" + variable + "}";

  gInterpreter->ExecuteMacro("WZPaperStyle.C");

  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);

  TFile* file = new TFile("rootfiles/all_unfolding_" + var + ".root", "read");

  TH1F* xsValue          = (TH1F*)file->Get("hComb_diff");
  TH1F* xsValue_Madgraph = (TH1F*)file->Get("hGenXs" + var + "_1");
  TH1F* xsValue_MCnlo;

  if (drawMcfm) xsValue_MCnlo = (TH1F*)file->Get("mcfm_tot");


  // Data cosmetics
  //----------------------------------------------------------------------------
  xsValue->SetFillColor  (     kWhite);
  xsValue->SetFillStyle  (       1001);
  xsValue->SetLineColor  (     kBlack);
  xsValue->SetLineWidth  (          1);
  xsValue->SetMarkerColor(     kBlack);
  xsValue->SetMarkerSize (     _msize);
  xsValue->SetMarkerStyle(kFullCircle);


  // Madgraph cosmetics
  //----------------------------------------------------------------------------
  xsValue_Madgraph->SetFillColor  (    kOrange);
  xsValue_Madgraph->SetFillStyle  (       1001);
  xsValue_Madgraph->SetLineColor  (  kOrange+7);
  xsValue_Madgraph->SetLineWidth  (          1);
  xsValue_Madgraph->SetMarkerColor(  kOrange+7);
  xsValue_Madgraph->SetMarkerSize (     _msize);
  xsValue_Madgraph->SetMarkerStyle(kFullSquare);


  // MCNLO cosmetics
  //----------------------------------------------------------------------------
  if (drawMcfm)
    {
      xsValue_MCnlo->SetFillColor  (   kAzure-9);
      xsValue_MCnlo->SetFillStyle  (       1001);
      xsValue_MCnlo->SetLineColor  (     kAzure);
      xsValue_MCnlo->SetLineWidth  (          1);
      xsValue_MCnlo->SetMarkerColor(     kAzure);
      xsValue_MCnlo->SetMarkerSize (     _msize);
      xsValue_MCnlo->SetMarkerStyle(kOpenCircle);
    }


  // Set the canvas and pads
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("wwxs", "wwxs");

  TPad* pad1;
  TPad* pad2;
  TPad* pad3;

  if (drawMcfm)
    {
      pad1 = new TPad("pad1", "pad1", 0, 0.49, 1, 1.000);
      pad2 = new TPad("pad2", "pad2", 0, 0.33, 1, 0.492);
      pad3 = new TPad("pad3", "pad3", 0, 0.00, 1, 0.332);
  
      pad1->SetTopMargin(0.09);
      pad2->SetTopMargin(0);
      pad3->SetTopMargin(0);

      pad1->SetBottomMargin(0);
      pad2->SetBottomMargin(0);
      pad3->SetBottomMargin(0.45);

      pad1->SetLeftMargin(0.16);
      pad2->SetLeftMargin(0.16);
      pad3->SetLeftMargin(0.16);
      
      pad1->SetRightMargin(0.06);
      pad2->SetRightMargin(0.06);
      pad3->SetRightMargin(0.06);
    }
  else
    {
      pad1 = new TPad("pad1", "pad1", 0, 0.33, 1, 1.000);
      pad2 = new TPad("pad2", "pad2", 0, 0.00, 1, 0.332);
  
      pad1->SetTopMargin(0.09);
      pad2->SetTopMargin(0);

      pad1->SetBottomMargin(0);
      pad2->SetBottomMargin(0.45);

      pad1->SetLeftMargin(0.16);
      pad2->SetLeftMargin(0.16);

      pad1->SetRightMargin(0.06);
      pad2->SetRightMargin(0.06);
    }


  // Draw pad1
  //----------------------------------------------------------------------------
  pad1->Draw();
  pad1->cd();
  pad1->SetLogy();

  AxisFonts(xsValue->GetXaxis(), xtitle);
  AxisFonts(xsValue->GetYaxis(), ytitle);
  
  xsValue->SetTitle("");

  xsValue->Draw("pe");

  xsValue_Madgraph->Draw("p,same");

  if (drawMcfm) xsValue_MCnlo->Draw("p,same");

  xsValue->Draw("pe,same");

  if (var.EqualTo("Zpt"))          xsValue->SetMinimum(1.1e-4);
  if (var.EqualTo("LeadingJetPt")) xsValue->SetMinimum(5e-4);
  if (var.EqualTo("Njets"))        xsValue->SetMinimum(2e-2);

  DrawLatex(_cmsTextFont,   0.173, 0.935, 0.065, 11, "CMS");
  DrawLatex(_lumiTextFont,  0.940, 0.935, 0.050, 31, "19.6 fb^{-1} (8 TeV)");


  // Legend
  //----------------------------------------------------------------------------
  if (drawMcfm)
    {
      DrawLegend(0.72, 0.80, xsValue,          " Data",     "lp");
      DrawLegend(0.72, 0.73, xsValue_Madgraph, " MadGraph", "flp");
      DrawLegend(0.72, 0.66, xsValue_MCnlo,    " MCFM",     "flp");
    }
  else
    {
      DrawLegend(0.68, 0.81, xsValue,          " Data",     "lp");
      DrawLegend(0.68, 0.74, xsValue_Madgraph, " MadGraph", "flp");
    }


  // Prepare the ratios
  //----------------------------------------------------------------------------
  TH1F* ratio_mad  = (TH1F*)xsValue_Madgraph->Clone("ratio_mad");
  TH1F* hratio_mad = (TH1F*)xsValue_Madgraph->Clone("hratio_mad");

  TH1F* ratio_mcnlo;
  TH1F* hratio_mcnlo;

  if (drawMcfm)
    {
      ratio_mcnlo  = (TH1F*)xsValue_MCnlo->Clone("ratio_mcnlo");
      hratio_mcnlo = (TH1F*)xsValue_MCnlo->Clone("hratio_mcnlo");
    }


  // Set the bin content
  //----------------------------------------------------------------------------
  for (UInt_t ibin=1; ibin<=ratio_mad->GetNbinsX(); ibin++) {
   
    Double_t madValue   = xsValue_Madgraph->GetBinContent(ibin);
    Double_t dataValue  = xsValue->GetBinContent(ibin);
    Double_t dataError  = xsValue->GetBinError(ibin);
   
    Double_t ratioValue_mad = (madValue > 0) ? madValue / dataValue : 0.0;
    Double_t ratioError_mad = madValue / pow(dataValue,2)*dataError;
   
    ratio_mad->SetBinContent(ibin, ratioValue_mad);
    ratio_mad->SetBinError  (ibin, 1e-9);

    hratio_mad->SetBinContent(ibin, ratioValue_mad);
    hratio_mad->SetBinError  (ibin, ratioError_mad);
   

    if (drawMcfm)
      {
	Double_t mcnloValue       = xsValue_MCnlo->GetBinContent(ibin);
	Double_t ratioValue_mcnlo = (mcnloValue > 0) ? mcnloValue / dataValue : 0.0;
	Double_t ratioError_mcnlo = mcnloValue / pow(dataValue,2)*dataError;

	ratio_mcnlo->SetBinContent(ibin, ratioValue_mcnlo);
	ratio_mcnlo->SetBinError  (ibin, 1e-9);

	hratio_mcnlo->SetBinContent(ibin, ratioValue_mcnlo);
	hratio_mcnlo->SetBinError  (ibin, ratioError_mcnlo);
      }
  }


  // Draw pad2
  //----------------------------------------------------------------------------
  AxisFontsRatio(ratio_mad->GetYaxis(), "y", "#frac{Theory}{Data}");
  AxisFontsRatio(ratio_mad->GetXaxis(), "x", xtitle);

  canvas->cd();
  pad2->Draw();
  pad2->cd();

  ratio_mad->SetTitle("");
  
  ratio_mad ->Draw("ep");
  hratio_mad->Draw("e2,same");
  ratio_mad ->Draw("ep,same");

  pad2->GetFrame()->DrawClone();
  pad2->RedrawAxis();

  TLine* line2 = new TLine(ratio_mad->GetXaxis()->GetXmin(), 1.0, ratio_mad->GetXaxis()->GetXmax(), 1.0);
  line2->SetLineStyle(3);
  line2->Draw("same");

  ratio_mad->GetYaxis()->SetRangeUser(0.01, 2.3);  

  if (drawMcfm) DrawLatex(43, 0.20, 0.785, 15.0, 11, "MadGraph+Pythia normalized to #sigma_{NLO}");
  else          DrawLatex(43, 0.19, 0.845, 15.0, 11, "MadGraph+Pythia normalized to #sigma_{NLO}");


  // Draw pad3
  //----------------------------------------------------------------------------
  if (drawMcfm)
    {
      AxisFontsRatio(ratio_mcnlo->GetYaxis(), "y", "#frac{Theory}{Data}");
      AxisFontsRatio(ratio_mcnlo->GetXaxis(), "x", xtitle);

      canvas->cd();
      pad3->Draw();
      pad3->cd();
  
      ratio_mcnlo->SetTitle("");

      ratio_mcnlo ->Draw("ep");
      hratio_mcnlo->Draw("e2,same");
      ratio_mcnlo ->Draw("ep,same");

      TLine* line3 = new TLine(ratio_mcnlo->GetXaxis()->GetXmin(), 1.0, ratio_mcnlo->GetXaxis()->GetXmax(), 1.0);
      line3->SetLineStyle(3);
      line3->Draw("same");

      ratio_mcnlo->GetYaxis()->SetRangeUser(0.1, 2.3);  

      pad3->Modified();

      DrawLatex(43, 0.2, 0.885, 15.0, 11, "MCFM");
    }


  // Save
  //----------------------------------------------------------------------------
  pad1->cd(); pad1->GetFrame()->DrawClone(); pad1->RedrawAxis();

  if (drawMcfm)
    {
      pad3->cd();
      pad3->GetFrame()->DrawClone();
      pad3->RedrawAxis();
    }

  canvas->cd();
  canvas->Update();

  canvas->SaveAs("pdf/unfolded_" + var + ".pdf");
  canvas->SaveAs("png/unfolded_" + var + ".png");
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis*  axis,
	       TString title)
{
  axis->SetLabelFont  (   43);
  axis->SetLabelOffset(0.005);
  axis->SetLabelSize  (   22);
  axis->SetNdivisions (  110);
  axis->SetTitle      (title);
  axis->SetTitleFont  (   43);
  axis->SetTitleOffset(  2.1);
  axis->SetTitleSize  (   18);
}


//------------------------------------------------------------------------------
// AxisFontsRatio
//------------------------------------------------------------------------------
void AxisFontsRatio(TAxis*  axis,
		    TString which,
		    TString title)
{
  if (which.Contains("x"))
    {
      axis->SetLabelFont  (   43);
      axis->SetLabelOffset(0.022);
      axis->SetLabelSize  (   22);
      axis->SetNdivisions (  510);
      axis->SetTitle      (title);
      axis->SetTitleFont  (   43);
      axis->SetTitleOffset(  5.0);
      axis->SetTitleSize  (   18);
    }
  else
    {
      axis->CenterTitle   ( true);
      axis->SetLabelFont  (   43);
      axis->SetLabelOffset( 0.02);
      axis->SetLabelSize  (   15);
      axis->SetNdivisions (  505);
      axis->SetTitle      (title);
      axis->SetTitleFont  (   43);
      axis->SetTitleOffset(  2.3);
      axis->SetTitleSize  (   16);
    }
}


//------------------------------------------------------------------------------
// DrawLatex
//------------------------------------------------------------------------------
void DrawLatex(Font_t      tfont,
	       Double_t    x,
	       Double_t    y,
	       Double_t    tsize,
	       Short_t     align,
	       const char* text,
	       Bool_t      setndc = true)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
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
