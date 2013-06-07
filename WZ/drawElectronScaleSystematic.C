//------------------------------------------------------------------------------
//
// drawElectronScaleSystematic
//
//------------------------------------------------------------------------------
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TSystem.h"


// Member functions
//------------------------------------------------------------------------------
void     AxisFonts         (TAxis*        axis,
			    TString       title);

TLegend* DrawLegend        (Float_t       x1,
			    Float_t       y1,
			    TObject*      hist,
			    TString       label,
			    TString       option  = "p",
			    Float_t       tsize   = 0.04,
			    Float_t       xoffset = 0.20,
			    Float_t       yoffset = 0.07);

void    MakeOutputDirectory(TString       format);

void    SetGraph           (TGraphErrors* g,
			    Color_t       color,
			    Style_t       style);

void    DrawTLatex         (Double_t      x,
			    Double_t      y,
			    Double_t      tsize,
			    Short_t       align,
			    const char*   text,
			    Bool_t        setndc = true);


void drawElectronScaleSystematic()
{
  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  gInterpreter->ExecuteMacro("./HiggsPaperStyle.C");

  TFile* file = TFile::Open("../WZ/AuxiliaryFilesWZXS8TeV/gScaleSyst-8TeV.root");

  TGraphErrors* gScaleInEB  = (TGraphErrors*)file->Get("gScaleInEB");
  TGraphErrors* gScaleOutEB = (TGraphErrors*)file->Get("gScaleOutEB");
  TGraphErrors* gScaleEE    = (TGraphErrors*)file->Get("gScaleEE");

  SetGraph(gScaleInEB,  kRed+2,    kOpenCircle);
  SetGraph(gScaleOutEB, kOrange+7, kFullCircle);
  SetGraph(gScaleEE,    kBlue+2,   kOpenSquare);

  TCanvas* c1 = new TCanvas("c1", "c1");

  gScaleInEB ->Draw("apz");
  gScaleOutEB->Draw("pz,same");
  gScaleEE   ->Draw("pz,same");

  TAxis* xaxis = (TAxis*)gScaleInEB->GetXaxis();
  TAxis* yaxis = (TAxis*)gScaleInEB->GetYaxis();

  AxisFonts(xaxis, "electron p_{T} [GeV]");
  AxisFonts(yaxis, "#Deltam(data - simulation) / m");

  gScaleInEB->SetMinimum(-0.023);
  gScaleInEB->SetMaximum( 0.010);

  c1->Modified();
  c1->Update();

  TLine* line = new TLine(c1->GetUxmin(), 0.0, c1->GetUxmax(), 0.0);

  line->SetLineStyle(3);
  line->SetLineWidth(3);

  line->Draw("same");

  DrawLegend(0.56, 0.38, (TObject*)gScaleInEB,  " Z, |#eta| < 0.8");
  DrawLegend(0.56, 0.31, (TObject*)gScaleOutEB, " Z, 0.8 < |#eta| < 1.48");
  DrawLegend(0.56, 0.24, (TObject*)gScaleEE,    " Z, |#eta| > 1.48");


  // CMS titles
  //----------------------------------------------------------------------------
  //  DrawTLatex(0.185, 0.975, 0.05, 13, "CMS Preliminary");
  DrawTLatex(0.940, 0.983, 0.05, 33, "#sqrt{s} = 8 TeV, L = 19.6 fb^{-1}");


  c1->SaveAs("pdf/scale_factors/electronScaleUncertainties.pdf");
  c1->SaveAs("png/scale_factors/electronScaleUncertainties.png");
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
  axis->SetTitleSize  (0.050);

  (title.Contains("[GeV]")) ? axis->SetTitleOffset(1.45) : axis->SetTitleOffset(1.8);

  axis->SetTitle(title);
}


//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
TLegend* DrawLegend(Float_t  x1,
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


//------------------------------------------------------------------------------
// MakeOutputDirectory
//------------------------------------------------------------------------------
void MakeOutputDirectory(TString format)
{
  gSystem->mkdir(Form("%s/scale_factors", format.Data()), kTRUE);

  gSystem->Exec(Form("cp index.php %s/.", format.Data()));

  gSystem->Exec(Form("cp index.php %s/scale_factors/.", format.Data()));
}


//------------------------------------------------------------------------------
// SetGraph
//------------------------------------------------------------------------------
void SetGraph(TGraphErrors* g,
	      Color_t       color,
	      Style_t       style)
{
  g->SetLineColor  (color);
  g->SetLineWidth  (2);
  g->SetMarkerColor(color);
  g->SetMarkerSize (1.2);
  g->SetMarkerStyle(style);
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Double_t    x,
		Double_t    y,
		Double_t    tsize,
		Short_t     align,
		const char* text,
		Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont (    42);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}
