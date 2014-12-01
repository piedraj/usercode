//------------------------------------------------------------------------------
//
// drawScaleFactors
//
//------------------------------------------------------------------------------
#include "DrawFunctions.h"


const Float_t _bigLabelSize = 0.04;


// Member functions
//------------------------------------------------------------------------------
void DrawIt             (TString filename,
			 TString hname,
			 TString cname,
			 TString title);

void AxisFonts          (TAxis*  axis,
			 TString title);

void TH2FAxisFonts      (TH2F*   h,
			 TString coordinate,
			 TString title);

void MakeOutputDirectory(TString format);


void drawScaleFactors()
{
  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  DrawIt("MuSF_2012",  "h2inverted", "MuonSF", "muon SF (8 TeV)");
  DrawIt("EleSF_2012", "h2inverted", "ElecSF", "electron SF (8 TeV)");

  DrawIt("MuPR_Moriond13_2012",  "h2inverted", "MuonPR", "muon PR (8 TeV)");
  DrawIt("ElePR_Moriond13_2012", "h2inverted", "ElecPR", "electron PR (8 TeV)");

  DrawIt("MuFR_Moriond13_jet10_EWKcorr",  "FR_pT_eta_EWKcorr", "MuonFR_Jet10", "muon FR jet10 (8 TeV)");
  DrawIt("MuFR_Moriond13_jet20_EWKcorr",  "FR_pT_eta_EWKcorr", "MuonFR_Jet20", "muon FR jet20 (8 TeV)");
  DrawIt("MuFR_Moriond13_jet30_EWKcorr",  "FR_pT_eta_EWKcorr", "MuonFR_Jet30", "muon FR jet30 (8 TeV)");
  
  DrawIt("EleFR_Moriond13_jet15_EWKcorr", "fakeElH2", "ElecFR_Jet15", "electron FR jet15 (8 TeV)");
  DrawIt("EleFR_Moriond13_jet35_EWKcorr", "fakeElH2", "ElecFR_Jet35", "electron FR jet35 (8 TeV)");
  DrawIt("EleFR_Moriond13_jet50_EWKcorr", "fakeElH2", "ElecFR_Jet50", "electron FR jet50 (8 TeV)");
  
  DrawIt("triggerEfficiencies", "DoubleElLead",  "DoubleElLead",  "leading electron trigger efficiency (8 TeV)");
  DrawIt("triggerEfficiencies", "DoubleMuLead",  "DoubleMuLead",  "leading muon trigger efficiency (8 TeV)");
  DrawIt("triggerEfficiencies", "DoubleElTrail", "DoubleElTrail", "trailing electron trigger efficiency (8 TeV)");
  DrawIt("triggerEfficiencies", "DoubleMuTrail", "DoubleMuTrail", "trailing muon trigger efficiency (8 TeV)");
}


//------------------------------------------------------------------------------
// DrawIt
//------------------------------------------------------------------------------
void DrawIt(TString filename,
	    TString hname,
	    TString cname,
	    TString title)
{
  TFile* inputfile = TFile::Open("../AuxiliaryFilesWZXS8TeV/" + filename + ".root");

  TH2F* h = (TH2F*)inputfile->Get(hname)->Clone(cname);

  h->SetDirectory(0);

  inputfile->Close();

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

  DrawTLatex(42, 0.940, 0.976, _bigLabelSize, 33, title);

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
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetTitleOffset(  1.5);
  axis->SetLabelSize  (_bigLabelSize);
  axis->SetTitleSize  (_bigLabelSize);

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
// MakeOutputDirectory
//------------------------------------------------------------------------------
void MakeOutputDirectory(TString format)
{
  gSystem->mkdir(Form("%s/scale_factors", format.Data()), kTRUE);

  gSystem->Exec(Form("cp index.php %s/.", format.Data()));

  gSystem->Exec(Form("cp index.php %s/scale_factors/.", format.Data()));
}
