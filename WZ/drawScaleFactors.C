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


// Member functions
//------------------------------------------------------------------------------
void    DrawIt             (TH2F*       h,
			    TString     title);

void    AxisFonts          (TAxis*      axis,
			    TString     title);

void    TH2FAxisFonts      (TH2F*       h,
			    TString     coordinate,
			    TString     title);

void    DrawTLatex         (Double_t    x,
			    Double_t    y,
			    Double_t    tsize,
			    Short_t     align,
			    const char* text);

TH2F*   LoadHistogram      (TString     filename,
			    TString     hname,
			    TString     cname);

TString GuessLocalBasePath ();

void    MakeOutputDirectory(TString     format);


// Data members
//------------------------------------------------------------------------------
TString localpath;

TH2F*   MuonSF;
TH2F*   ElecSF;
TH2F*   MuonPR;
TH2F*   ElecPR;
TH2F*   MuonFR;
TH2F*   ElecFR;
TH2F*   DoubleElLead;
TH2F*   DoubleMuLead;
TH2F*   DoubleElTrail;
TH2F*   DoubleMuTrail;


void drawScaleFactors()
{
  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  localpath = GuessLocalBasePath();

  gInterpreter->ExecuteMacro("./HiggsPaperStyle.C");

  gStyle->SetOptStat(0);

  gStyle->SetPalette(1,0);


  // SF, FR and PR histograms
  //----------------------------------------------------------------------------
  MuonSF = LoadHistogram("MuSF_2012",            "h2inverted", "MuonSF");
  ElecSF = LoadHistogram("EleSF_2012",           "h2inverted", "ElecSF");
  MuonPR = LoadHistogram("MuPR_Moriond13_2012",  "h2inverted", "MuonPR");
  ElecPR = LoadHistogram("ElePR_Moriond13_2012", "h2inverted", "ElecPR");

  MuonFR = LoadHistogram("MuFR_Moriond13_jet15_EWKcorr",  "FR_pT_eta_EWKcorr", "MuonFR_Jet15");
  ElecFR = LoadHistogram("EleFR_Moriond13_jet35_EWKcorr", "fakeElH2",          "ElecFR_Jet35");

  DoubleElLead  = LoadHistogram("triggerEfficiencies", "DoubleElLead",  "DoubleElLead");
  DoubleMuLead  = LoadHistogram("triggerEfficiencies", "DoubleMuLead",  "DoubleMuLead");
  DoubleElTrail = LoadHistogram("triggerEfficiencies", "DoubleElTrail", "DoubleElTrail");
  DoubleMuTrail = LoadHistogram("triggerEfficiencies", "DoubleMuTrail", "DoubleMuTrail");

  DrawIt(MuonSF,        "2012 muon SF");
  DrawIt(ElecSF,        "2012 electron SF");
  DrawIt(MuonPR,        "2012 muon PR");
  DrawIt(ElecPR,        "2012 electron PR");
  DrawIt(MuonFR,        "2012 muon FR jet15");
  DrawIt(ElecFR,        "2012 electron FR jet35");
  DrawIt(DoubleElLead,  "2012 leading electron trigger efficiency");
  DrawIt(DoubleMuLead,  "2012 leading muon trigger efficiency");
  DrawIt(DoubleElTrail, "2012 trailing electron trigger efficiency");
  DrawIt(DoubleMuTrail, "2012 trailing muon trigger efficiency");
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TH2F* h, TString title)
{
  TString name = h->GetName();

  TCanvas* canvas = new TCanvas(name, name, 600, 600);

  if (name.Contains("PR")) canvas->SetLogx();
  if (name.Contains("SF")) canvas->SetLogx();

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TH2FAxisFonts(h, "x", "p_{T} [GeV]");
  TH2FAxisFonts(h, "y", "#eta");

  h->Draw("colz");

  h->SetTitle("");

  DrawTLatex(0.940, 0.976, 0.05, 33, title);

  if (!title.Contains("trigger")) {

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

  canvas->SaveAs("pdf/scale_factors/" + name + ".pdf");
  canvas->SaveAs("png/scale_factors/" + name + ".png");
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
  TFile* inputfile = TFile::Open(localpath + "/piedra/work/WZLatino/AuxiliaryFilesWZXS8TeV/" + filename + ".root");

  TH2F* hist = (TH2F*)inputfile->Get(hname)->Clone(cname);

  hist->SetDirectory(0);

  inputfile->Close();

  return hist;
}


//------------------------------------------------------------------------------
// GuessLocalBasePath
//------------------------------------------------------------------------------
TString GuessLocalBasePath()
{
  TString host = gSystem->HostName();

  if (host.Contains("uniovi.es"))
    {
      return TString("/nfs/fanae/user");
    }
  else if (host.Contains("ifca.es"))
    {
      return TString("/gpfs/csic_users");
    }
  else
    {
      printf(" ERROR: Could not guess base path from host name %s.", host.Data());

      return TString("");
    }
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
