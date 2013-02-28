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

  gSystem->Exec("cp index.php png/.");

  gSystem->Exec("cp index.php png/ScaleFactors/.");

  gStyle->SetOptStat(0);

  gStyle->SetPalette(1,0);


  // SF, FR and PR histograms
  //----------------------------------------------------------------------------
  MuonSF = LoadHistogram("MuSF_2012",  "muonDATAMCratio_All_selec",      "MuonSF");
  ElecSF = LoadHistogram("EleSF_2012", "electronsDATAMCratio_All_selec", "ElecSF");

  MuonPR = LoadHistogram("prompt_rateMuons",     "effDATA_prompt_rate", "MuonPR");
  ElecPR = LoadHistogram("prompt_rateElectrons", "effDATA_All_selec",   "ElecPR");

  MuonFR_lo = LoadHistogram("MuFR_2012_jet30",  "h_Muon_signal_pT_eta", "MuonFR_lo");
  ElecFR_lo = LoadHistogram("EleFR_2012_jet35", "fakeElH2",             "ElecFR_lo");

  MuonFR_hi = LoadHistogram("MuFR_2012_jet50",  "h_Muon_signal_pT_eta", "MuonFR_hi");
  ElecFR_hi = LoadHistogram("EleFR_2012_jet50", "fakeElH2",             "ElecFR_hi");

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
// LoadHistogram
//------------------------------------------------------------------------------
TH2F* LoadHistogram(TString filename,
		    TString hname,
		    TString cname)
{
  TFile* inputfile = TFile::Open("/nfs/fanae/user/piedra/work/PAF/LeptonScaleFactors/" + filename + ".root");

  //  TFile* inputfile = TFile::Open("/gpfs/csic_users/piedra/work/PAF/LeptonScaleFactors/" + filename + ".root");

  TH2F* hist = (TH2F*)inputfile->Get(hname)->Clone(cname);

  hist->SetDirectory(0);

  inputfile->Close();

  return hist;
}
