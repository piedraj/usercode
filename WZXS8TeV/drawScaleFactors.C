//------------------------------------------------------------------------------
//
// drawScaleFactors
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TColor.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "TH2.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPaletteAxis.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"


void DrawIt        (TH2F*       h,
		    TString     title);

void AxisFonts     (TAxis*      axis,
		    TString     title);

void TH2FAxisFonts (TH2F*       h,
		    TString     coordinate,
		    TString     title);

void DrawTLatex    (Double_t    x,
		    Double_t    y,
		    Double_t    tsize,
		    Short_t     align,
		    const char* text);

void setupGreyScale();

void setupColors   ();

TH2F* LoadHistogram(TString     filename,
		    TString     hname,
		    TString     cname);


TH2F* MuonSF;
TH2F* ElecSF;
TH2F* MuonPR;
TH2F* ElecPR;
TH2F* MuonFR_lo;
TH2F* ElecFR_lo;
TH2F* MuonFR_hi;
TH2F* ElecFR_hi;


void drawScaleFactors()
{
  gInterpreter->ExecuteMacro("./HiggsPaperStyle.C");

  gSystem->mkdir("png/ScaleFactors", kTRUE);

  gStyle->SetOptStat(0);

  gStyle->SetPalette(1,0);

  //  setupGreyScale();

  //  setupColors();


  // SF, FR and PR histograms
  //----------------------------------------------------------------------------
  TString path1 = "/nfs/fanae/user/piedra/work/WZ/AnalysisVHCode/WManager/data/";
  TString path2 = "/nfs/fanae/user/piedra/work/WZ/work/WManager/PR/";

  MuonSF = LoadHistogram(path1 + "MuSF_2012",  "muonDATAMCratio_All_selec",      "MuonSF");
  ElecSF = LoadHistogram(path1 + "EleSF_2012", "electronsDATAMCratio_All_selec", "ElecSF");

  MuonPR = LoadHistogram(path2 + "prompt_rateMuons",     "effDATA_prompt_rate", "MuonPR");
  ElecPR = LoadHistogram(path2 + "prompt_rateElectrons", "effDATA_All_selec",   "ElecPR");

  MuonFR_lo = LoadHistogram(path1 + "MuFR_2012_jet30",  "h_Muon_signal_pT_eta", "MuonFR_lo");
  ElecFR_lo = LoadHistogram(path1 + "EleFR_2012_jet35", "fakeElH2",             "ElecFR_lo");

  MuonFR_hi = LoadHistogram(path1 + "MuFR_2012_jet50",  "h_Muon_signal_pT_eta", "MuonFR_hi");
  ElecFR_hi = LoadHistogram(path1 + "EleFR_2012_jet50", "fakeElH2",             "ElecFR_hi");

  DrawIt(MuonSF,    "2012 muon SF");
  DrawIt(ElecSF,    "2012 electron SF");
  DrawIt(MuonPR,    "2012 muon PR");
  DrawIt(ElecPR,    "2012 electron PR");
  DrawIt(MuonFR_lo, "2012 muon FR jet30");
  DrawIt(ElecFR_lo, "2012 electron FR jet35");
  DrawIt(MuonFR_hi, "2012 muon FR jet50");
  DrawIt(ElecFR_hi, "2012 electron FR jet50");
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TH2F* h, TString title)
{
  TString name = h->GetName();

  TCanvas* canvas = new TCanvas(name, name, 600, 600);

  if (name.Contains("PR")) canvas->SetLogy();
  if (name.Contains("SF")) canvas->SetLogx();

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TString xtitle = (name.Contains("PR")) ? "#eta" : "p_{T} [GeV]";
  TString ytitle = (name.Contains("PR")) ? "p_{T} [GeV]" : "#eta";

  TH2FAxisFonts(h, "x", xtitle);
  TH2FAxisFonts(h, "y", ytitle);

  h->Draw("colz");

  h->SetTitle("");

  DrawTLatex(0.940, 0.976, 0.05, 33, title);
  
  Double_t hmin = h->GetMinimum();
  Double_t hmax = h->GetMaximum();

  for (Int_t i=1; i<=h->GetNbinsX(); i++) {
    for (Int_t j=1; j<=h->GetNbinsY(); j++) {

      Double_t value = h->GetBinContent(i,j);

      Double_t ypos = h->GetYaxis()->GetBinCenter(j);
      Double_t xpos = h->GetXaxis()->GetBinCenter(i);
      
      if (gPad->GetLogx()) xpos = h->GetXaxis()->GetBinCenterLog(i);

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

  canvas->SaveAs("png/ScaleFactors/" + name + ".png");
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis*  axis,
	       TString title)
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


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Double_t    x,
                Double_t    y,
                Double_t    tsize,
                Short_t     align,
                const char* text)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC();
  tl->SetTextAlign(align);
  tl->SetTextFont (   42);
  tl->SetTextSize (tsize);

  tl->Draw("same");
}


//------------------------------------------------------------------------------
// setupGreyScale
//------------------------------------------------------------------------------
void setupGreyScale() 
{
  const Int_t Number  =  10;
  const Int_t NColors = 100;

  Double_t Red  [Number];
  Double_t Green[Number];
  Double_t Blue [Number];
  Double_t Stops[Number];
  
  Double_t dcol = 1. / Number;
  Double_t grey = 1.;

  for (Int_t j=0; j<Number; j++) {

    Stops[j] = Double_t(j) / Double_t(Number - 1);

    Red  [j] = grey;
    Blue [j] = grey;
    Green[j] = grey;

    grey = grey - dcol;
  }

  TColor::CreateGradientColorTable(Number, Stops, Red, Green, Blue, NColors);

  gStyle->SetNumberContours(NColors);
}


//------------------------------------------------------------------------------
// setupColors
//------------------------------------------------------------------------------
void setupColors()
{
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  
  Double_t stops[NRGBs] = {0.00, 0.0625, 0.25, 0.5625, 1.00};
  Double_t red  [NRGBs] = {0.00, 0.00,   0.87, 1.00,   0.51};
  Double_t green[NRGBs] = {0.00, 0.81,   1.00, 0.20,   0.00};
  Double_t blue [NRGBs] = {0.51, 1.00,   0.12, 0.00,   0.00};
  
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}


//------------------------------------------------------------------------------
// LoadHistogram
//------------------------------------------------------------------------------
TH2F* LoadHistogram(TString filename,
		    TString hname,
		    TString cname)
{
  printf("%s.root\n", filename.Data());

  TFile* inputfile = TFile::Open(filename + ".root");

  TH2F* hist = (TH2F*)inputfile->Get(hname)->Clone(cname);

  hist->SetDirectory(0);

  inputfile->Close();

  return hist;
}
