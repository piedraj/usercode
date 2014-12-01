//------------------------------------------------------------------------------
//
// drawElectronScaleSystematic
//
//------------------------------------------------------------------------------
#include "DrawFunctions.h"


const Float_t _bigLabelSize = 0.04;


// Member functions
//------------------------------------------------------------------------------
void    AxisFonts          (TAxis*        axis,
			    TString       title);

void    MakeOutputDirectory(TString       format);

void    SetGraph           (TGraphErrors* g,
			    Color_t       color,
			    Style_t       style);


void drawElectronScaleSystematic()
{
  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  TFile* file = TFile::Open("../AuxiliaryFilesWZXS8TeV/gScaleSyst-8TeV.root");

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

  yaxis->CenterTitle();

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
  DrawTLatex(61, 0.190, 0.94, 0.055, 11, "CMS");
  DrawTLatex(52, 0.315, 0.94, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.940, 0.94, 0.040, 31, "19.6 fb^{-1} (8 TeV)");


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
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetLabelSize  (_bigLabelSize);
  axis->SetTitleSize  (_bigLabelSize);

  (title.Contains("[GeV]")) ? axis->SetTitleOffset(1.45) : axis->SetTitleOffset(1.8);

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

