#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH1.h"
#include "THStack.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include <vector>


// Input parameters for the WZ cross section
//------------------------------------------------------------------------------
const Double_t xsWplusZ_nlo  = 14.48;  // pb (arXiv:1105.0020v1)
const Double_t xsWminusZ_nlo =  8.40;  // pb (arXiv:1105.0020v1)
const Double_t xsWplusZ      = 13.89;  // pb (MCFM with 71 < mZ < 111 GeV)
const Double_t xsWminusZ     =  8.06;  // pb (MCFM with 71 < mZ < 111 GeV)

const Double_t W2e         = 0.1075;
const Double_t W2m         = 0.1057;
const Double_t W2tau       = 0.1125;
const Double_t Z2ll        = 0.033658;
const Double_t WZ23lnu     = 3 * Z2ll * (W2e + W2m + W2tau);
const Double_t ngenWZ      = 2017979;
const Double_t ngenWZphase = 1449067;  // (71 < mZ < 111 GeV)


// Data members
//------------------------------------------------------------------------------
const UInt_t nChannels  =  4;
const UInt_t nCuts      =  7;
const UInt_t nProcesses = 13;

enum {MMM, EEE, MME, EEM};

enum {
  AllEvents,
  HLT,
  Has2IsoGoodLeptons,
  Exactly3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET
};

enum {
  Data,
  ZJets_Madgraph,
  TW,
  TbarW,
  TTbar_Madgraph,
  TTbar_Powheg_2L,
  WJets_Madgraph,
  ZZ,
  WW,
  WgammaToLNuG,
  ZgammaToLLG,
  WZTo3LNu
};


// Settings
//------------------------------------------------------------------------------
TString        sChannel [nChannels];
TString        sCut     [nCuts];
TFile*         input    [nProcesses];
TString        process  [nProcesses];
Color_t        color    [nProcesses];
Double_t       systError[nProcesses];

Bool_t         _setLogy;
Double_t       _luminosity;
Double_t       _yoffset;
Int_t          _verbosity;
TString        _directory;
TString        _format;

vector<UInt_t> vprocess;


// Member functions
//------------------------------------------------------------------------------
void     SetParameters            (UInt_t      channel);

void     MeasureTheCrossSection   (UInt_t      channel);

void     DrawHistogram            (TString     hname,
				   TString     xtitle,
				   Int_t       ngroup       = -1,
				   Int_t       precision    = 0,
				   TString     units        = "NULL",
				   Double_t    xmin         = -999,
				   Double_t    xmax         =  999,
				   Bool_t      moveOverflow = false);

Float_t  GetMaximumIncludingErrors(TH1F*       h,
				   Double_t    xmin = -999,
				   Double_t    xmax = -999);


void     MoveOverflowBins         (TH1*        h,
				   Double_t    xmin,
				   Double_t    xmax);

void     ZeroOutOfRangeBins       (TH1*        h,
				   Double_t    xmin,
				   Double_t    xmax);

void     SetAxis                  (TH1*        hist,
				   TString     xtitle,
				   TString     ytitle,
				   Float_t     size,
				   Float_t     offset);

void     DrawTLatex               (Double_t    x,
				   Double_t    y,
				   Double_t    tsize,
				   Short_t     align,
				   const char* text);

TLegend* DrawLegend               (Float_t     x1,
				   Float_t     y1,
				   TH1*        hist,
				   TString     label,
				   TString     option,
				   Float_t     tsize   = 0.03,
				   Float_t     xoffset = 0.200,
				   Float_t     yoffset = _yoffset);


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(UInt_t channel = EEE,
	UInt_t cut     = Exactly3Leptons,
	Bool_t batch   = false)
{
  if (channel >= nChannels) return; 

  gROOT->SetBatch(batch);

  SetParameters(channel);

  if (cut == MET) MeasureTheCrossSection(channel);

  //  DrawHistogram("hNPV_EEE_Has2IsoGoodLeptons", "number of primary vertices", -1, 0, "NULL", 0, 30, false);
  //  DrawHistogram("hNPV_MMM_Has2IsoGoodLeptons", "number of primary vertices", -1, 0, "NULL", 0, 30, false);

  TString suffix = "_" + sChannel[channel] + "_" + sCut[cut];

  DrawHistogram("hNPV" + suffix, "number of primary vertices", -1, 0, "NULL", 0, 30, false);
  DrawHistogram("hMET" + suffix, "E_{T}^{miss}",                5, 0, "GeV");

  if (cut < Exactly3Leptons) return;

  DrawHistogram("hSumCharges"   + suffix, "q_{1} + q_{2} + q_{3}");
  DrawHistogram("hPtLepton1"    + suffix, "p_{T}^{first lepton}",  5, 0, "GeV");
  DrawHistogram("hPtLepton2"    + suffix, "p_{T}^{second lepton}", 5, 0, "GeV");
  DrawHistogram("hPtLepton3"    + suffix, "p_{T}^{third lepton}",  5, 0, "GeV");
  DrawHistogram("hInvMass2Lep1" + suffix, "m_{#font[12]{ll}}",     2, 0, "GeV", 60, 120);
  DrawHistogram("hInvMass2Lep2" + suffix, "m_{#font[12]{ll}}^{second combination}",   2, 0, "GeV");
  DrawHistogram("hInvMass3Lep"  + suffix, "m_{#font[12]{3l}}",     5, 0, "GeV",  0, 600);

  if (cut < HasWCandidate) return;

  DrawHistogram("hPtZLepton1" + suffix, "p_{T}^{Z leading lepton}",  5, 0, "GeV");
  DrawHistogram("hPtZLepton2" + suffix, "p_{T}^{Z trailing lepton}", 5, 0, "GeV", 0, 100);
  DrawHistogram("hPtWLepton"  + suffix, "p_{T}^{W lepton}",          5, 0, "GeV");
  DrawHistogram("hInvMassZ"   + suffix, "m_{#font[12]{ll}}",         2, 0, "GeV");
}


//------------------------------------------------------------------------------
// MeasureTheCrossSection
//------------------------------------------------------------------------------
void MeasureTheCrossSection(UInt_t channel)
{
  Double_t ndata       = 0;
  Double_t nsignal     = 0;
  Double_t nbackground = 0;
  Double_t nTTbar      = 0;

  Double_t nWZ = ((TH1F*)input[WZTo3LNu]->Get("hCounterEff_" + sChannel[channel] + "_MET"))->Integral();

  TString hname = "hCounter_" + sChannel[channel] + "_MET";

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TH1F* dummy = (TH1F*)input[j]->Get(hname);

    if (j == Data) {
      ndata = dummy->Integral();
    }
    else if (j == WZTo3LNu) {
      nsignal = dummy->Integral();
    }
    else {
      nbackground += dummy->Integral();
      if (j == TTbar_Madgraph || j == TTbar_Powheg_2L)
	nTTbar = dummy->Integral();
    }
  }


  // Estimate the cross section
  //----------------------------------------------------------------------------
  Double_t efficiency = nWZ / ngenWZphase;

  Double_t xs = (ndata - nbackground) / (_luminosity * efficiency * WZ23lnu);


  // Relative errors
  //----------------------------------------------------------------------------
  Double_t xsRelativeErrorBackground = 0.0;
  Double_t xsRelativeErrorEfficiency = 0.0;

  Double_t xsRelativeErrorSyst = 0;
  xsRelativeErrorSyst += (xsRelativeErrorBackground * xsRelativeErrorBackground);
  xsRelativeErrorSyst += (xsRelativeErrorEfficiency * xsRelativeErrorEfficiency);
  xsRelativeErrorSyst = sqrt(xsRelativeErrorSyst);

  Double_t xsRelativeErrorStat = 1e2 * sqrt(ndata) / (ndata - nbackground);
  Double_t xsRelativeErrorLumi = 4.4;


  // Absolute errors
  //----------------------------------------------------------------------------
  Double_t xsErrorSyst = xs * xsRelativeErrorSyst / 1e2;  // pb
  Double_t xsErrorStat = xs * xsRelativeErrorStat / 1e2;  // pb
  Double_t xsErrorLumi = xs * xsRelativeErrorLumi / 1e2;  // pb


  // Print the results
  //----------------------------------------------------------------------------
  if (_verbosity > 0) {
    printf("\n");
    printf("                          ---------- %s ---------- ", sChannel[channel].Data());
  }

  if (_verbosity > 2) {
    printf("\n");
    printf("                    BR(W  -> e   nu) = %5.2f %s\n", 1e2 * W2e,     "%");
    printf("                    BR(W  -> mu  nu) = %5.2f %s\n", 1e2 * W2m,     "%");
    printf("                    BR(W  -> tau nu) = %5.2f %s\n", 1e2 * W2tau,   "%");
    printf("                    BR(Z  -> ll)     = %7.4f %s\n", 1e2 * Z2ll,    "%");
    printf("                    BR(WZ -> 3l nu)  = %5.2f %s\n", 1e2 * WZ23lnu, "%");
    printf("                          luminosity = %.1f pb\n", _luminosity);
    printf("                        generated WZ = %.0f\n", ngenWZ);
    printf(" generated WZ with 71 < mZ < 111 GeV = %.0f (%.0f %s)\n", ngenWZphase, 1e2 * ngenWZphase / ngenWZ, "%");
  }

  if (_verbosity > 1) {
    printf("\n");
    printf("               number of selected WZ = %.0f\n", nWZ);
    printf("                       WZ efficiency = %.2f %s\n", 1e2 * efficiency, "%");
    printf("                         nbackground = %5.1f +- %4.1f\n", nbackground,       sqrt(nbackground));
    printf("                               ndata = %5.1f +- %4.1f\n", ndata,             sqrt(ndata));
    printf("                 ndata - nbackground = %5.1f +- %4.1f\n", ndata-nbackground, sqrt(ndata+nbackground));
    printf("                             nsignal = %5.1f +- %4.1f\n", nsignal,           sqrt(nsignal));
    printf("                              nTTbar = %5.1f +- %4.1f\n", nTTbar,            sqrt(nTTbar));
  }

  if (_verbosity > 0) {
    printf("\n");
    printf("                         measured xs = %.2f +- %.2f (stat) +- %.2f (syst) +- %.2f (lumi) pb\n", xs, xsErrorStat, xsErrorSyst, xsErrorLumi);
    printf("                              NLO xs = %.2f pb (MCFM with 71 < mZ < 111 GeV)\n", xsWplusZ + xsWminusZ);
    printf("\n");
  }
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   TString  xtitle,
		   Int_t    ngroup,
		   Int_t    precision,
		   TString  units,
		   Double_t xmin,
		   Double_t xmax,
		   Bool_t   moveOverflow)
{
  TCanvas* canvas = new TCanvas(hname, hname, 550, 900);

  TPad* pad1 = new TPad("pad1", "pad1", 0, 0.44, 1, 1.00);
  TPad* pad2 = new TPad("pad1", "pad1", 0, 0.22, 1, 0.44);
  TPad* pad3 = new TPad("pad1", "pad1", 0, 0.00, 1, 0.22);

  pad1->SetTopMargin   (0.08);
  pad1->SetBottomMargin(0.02);
  pad1->Draw();

  pad2->SetTopMargin   (0.08);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();

  pad3->SetTopMargin   (0.03);
  pad3->SetBottomMargin(0.35);
  pad3->Draw();


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();
  
  pad1->SetLogy(_setLogy);

  Double_t mcIntegral = 0;

  THStack* hstack = new THStack(hname, hname);

  TH1F* hist[nProcesses];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1F*)input[j]->Get(hname);
    hist[j]->SetName(hname + process[j]);

    if (ngroup > 0) hist[j]->Rebin(ngroup);

    if (moveOverflow) MoveOverflowBins  (hist[j], xmin, xmax);
    else              ZeroOutOfRangeBins(hist[j], xmin, xmax);
    
    if (j == Data) {
      hist[j]->SetMarkerStyle(kFullCircle);
      hist[j]->SetTitle("");
    }
    else {

      mcIntegral += hist[j]->Integral();

      hist[j]->SetFillColor(color[j]);
      hist[j]->SetFillStyle(1001);
      hist[j]->SetLineColor(color[j]);

      hstack->Add(hist[j]);
    }
  }


  // Normalize MC to data
  //----------------------------------------------------------------------------
  //  if (hname.Contains("hNPV") && !hname.Contains("MET")) {
  //  
  //    for (UInt_t i=0; i<vprocess.size(); i++) {
  //
  //      UInt_t j = vprocess.at(i);
  //      
  //      if (j != Data && mcIntegral > 0)
  //	hist[j]->Scale(hist[Data]->Integral() / mcIntegral);
  //    }
  //  }


  // All MC
  //----------------------------------------------------------------------------
  TH1F* allmc = (TH1F*)hist[Data]->Clone("allmc");

  allmc->SetFillColor  (kGray+2);
  allmc->SetFillStyle  (   3345);
  allmc->SetLineColor  (kGray+2);
  allmc->SetMarkerColor(kGray+2);
  allmc->SetMarkerSize (      0);

  for (Int_t ibin=1; ibin<=allmc->GetNbinsX(); ibin++) {

    Double_t binValue = 0;
    Double_t binError = 0;

    for (UInt_t i=0; i<vprocess.size(); i++) {

      UInt_t j = vprocess.at(i);

      if (j == Data) continue;

      Double_t binContent = hist[j]->GetBinContent(ibin);
      
      binValue += binContent;
      binError += (hist[j]->GetBinError(ibin) * hist[j]->GetBinError(ibin));
      binError += (systError[j]*binContent * systError[j]*binContent);
    }
    
    binError = sqrt(binError);

    allmc->SetBinContent(ibin, binValue);
    allmc->SetBinError  (ibin, binError);
  }


  // Axis labels
  //----------------------------------------------------------------------------
  TAxis* xaxis = hist[Data]->GetXaxis();
  TAxis* yaxis = hist[Data]->GetYaxis();

  TString ytitle = Form("entries / %s.%df", "%", precision);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(Form(ytitle.Data(), hist[Data]->GetBinWidth(0)));
  yaxis->SetTitleOffset(1.6);

  if (!units.Contains("NULL")) {
    
    xaxis->SetTitle(Form("%s [%s]", xaxis->GetTitle(), units.Data()));
    yaxis->SetTitle(Form("%s %s",   yaxis->GetTitle(), units.Data()));
  }


  // Draw
  //----------------------------------------------------------------------------
  xaxis->SetRangeUser(xmin, xmax);

  hist[Data]->Draw("ep");
  hstack    ->Draw("hist,same");
  allmc     ->Draw("e2,same");
  hist[Data]->Draw("ep,same");


  // Adjust scale
  //----------------------------------------------------------------------------
  Float_t theMax   = GetMaximumIncludingErrors(hist[Data], xmin, xmax);
  Float_t theMaxMC = GetMaximumIncludingErrors(allmc,      xmin, xmax);

  if (theMaxMC > theMax) theMax = theMaxMC;

  if (pad1->GetLogy()) {

    theMax = TMath::Power(10, TMath::Log10(theMax) + 4);

    hist[Data]->SetMinimum(0.01);
  }
  else theMax *= 1.4;

  hist[Data]->SetMaximum(theMax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.72;
  Double_t y0     = 0.834;
  Double_t delta  = _yoffset + 0.001;
  Double_t ndelta = 0;
  
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[Data],           Form(" data (%.0f)",   hist[Data]          ->Integral()), "lp"); ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, allmc,                Form(" MC (%.0f)",     allmc               ->Integral()), "f");  ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[WZTo3LNu],       Form(" WZ (%.0f)",     hist[WZTo3LNu]      ->Integral()), "f");  ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[ZJets_Madgraph], Form(" Z+jets (%.0f)", hist[ZJets_Madgraph]->Integral()), "f");  ndelta += delta;

  ndelta = 0;

  DrawLegend(x0 - 0.23, y0 - ndelta, hist[ZgammaToLLG],    Form(" Z#gamma (%.0f)",  hist[ZgammaToLLG]   ->Integral()), "f"); ndelta += delta;
  DrawLegend(x0 - 0.23, y0 - ndelta, hist[ZZ],             Form(" ZZ (%.0f)",       hist[ZZ]            ->Integral()), "f"); ndelta += delta;
  DrawLegend(x0 - 0.23, y0 - ndelta, hist[TTbar_Madgraph], Form(" t#bar{t} (%.0f)", hist[TTbar_Madgraph]->Integral()), "f"); ndelta += delta;
  DrawLegend(x0 - 0.23, y0 - ndelta, hist[TW],             Form(" tW (%.0f)",       hist[TW]            ->Integral()), "f"); ndelta += delta;

  ndelta = 0;

  DrawLegend(x0, y0 - ndelta, hist[WW],             Form(" WW (%.0f)",      hist[WW]            ->Integral()), "f"); ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[WgammaToLNuG],   Form(" W#gamma (%.0f)", hist[WgammaToLNuG]  ->Integral()), "f"); ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[WJets_Madgraph], Form(" W+jets (%.0f)",  hist[WJets_Madgraph]->Integral()), "f"); ndelta += delta;


  // CMS titles
  //----------------------------------------------------------------------------
  TString leftTitle = "CMS preliminary";

  if (hname.Contains("MMM")) leftTitle = "#mu#mu#mu";
  if (hname.Contains("EEE")) leftTitle = "eee";
  if (hname.Contains("MME")) leftTitle = "#mu#mue";
  if (hname.Contains("EEM")) leftTitle = "ee#mu";

  if (hname.Contains("NPV") && hname.Contains("NoCuts")) leftTitle = "";

  DrawTLatex(0.185, 0.975, 0.05, 13, leftTitle.Data());
  DrawTLatex(0.940, 0.983, 0.05, 33, Form("#sqrt{s} = 8 TeV, L = %.1f fb^{-1}", _luminosity/1e3));


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  pad2->cd();
    
  TH1F* ratio       = (TH1F*)hist[Data]->Clone("ratio");
  TH1F* uncertainty = (TH1F*)allmc->Clone("uncertainty");

  for (Int_t ibin=1; ibin<=ratio->GetNbinsX(); ibin++) {

    Double_t mcValue = allmc->GetBinContent(ibin);
    Double_t mcError = allmc->GetBinError  (ibin);
    
    Double_t dtValue = ratio->GetBinContent(ibin);
    Double_t dtError = ratio->GetBinError  (ibin);

    Double_t ratioValue       = (mcValue > 0) ? dtValue/mcValue : 0.0;
    Double_t ratioError       = (mcValue > 0) ? dtError/mcValue : 0.0;
    Double_t uncertaintyError = (mcValue > 0) ? mcError/mcValue : 0.0;

    ratio->SetBinContent(ibin, ratioValue);
    ratio->SetBinError  (ibin, ratioError);

    uncertainty->SetBinContent(ibin, 1.0);
    uncertainty->SetBinError  (ibin, uncertaintyError);
  }

  TAxis* uaxis = (TAxis*)uncertainty->GetXaxis();
    
  uaxis->SetRangeUser(xmin, xmax);
     
  uncertainty->Draw("e2");
  ratio->Draw("ep,same");

  uncertainty->GetYaxis()->SetRangeUser(0, 2.5);


  //----------------------------------------------------------------------------
  // pad3
  //----------------------------------------------------------------------------
  pad3->cd();

  TH1F* hdeviations = new TH1F("hdeviations", "", 25, 0, 2.5);

  hdeviations->SetName("hdeviations_" + hname);

  for (Int_t i=1; i<=ratio->GetNbinsX(); i++)
    if (ratio->GetBinContent(i) > 0) hdeviations->Fill(ratio->GetBinContent(i));

  hdeviations->Draw("hist");

  hdeviations->SetFillColor(kGray+2);
  hdeviations->SetFillStyle(3354);
  hdeviations->SetLineColor(0);
  hdeviations->SetLineWidth(0);


  // Save
  //----------------------------------------------------------------------------
  pad2->cd(); SetAxis(uncertainty, hist[Data]->GetXaxis()->GetTitle(), "data / prediction", 0.10, 0.8);
  pad1->cd(); SetAxis(hist[Data], "", hist[Data]->GetYaxis()->GetTitle(),                   0.05, 1.6);
  pad3->cd(); SetAxis(hdeviations, "data / prediction", "entries / bin",                    0.10, 0.8);

  canvas->cd();

  TString suffixLogy = (_setLogy) ? "_log" : "";

  canvas->SaveAs(Form("%s/%s/%s%s.%s",
		      _format.Data(),
		      _directory.Data(),
		      hname.Data(),
		      suffixLogy.Data(),
		      _format.Data()));
}


//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Float_t GetMaximumIncludingErrors(TH1F*    h,
				  Double_t xmin,
				  Double_t xmax)
{
  UInt_t nbins = h->GetNbinsX();

  TAxis* axis = (TAxis*)h->GetXaxis();
  
  Int_t firstBin = (xmin != -999) ? axis->FindBin(xmin) : 1;
  Int_t lastBin  = (xmax != -999) ? axis->FindBin(xmax) : nbins;

  Float_t maxWithErrors = 0;

  for (Int_t i=firstBin; i<=lastBin; i++) {

    Float_t binHeight = h->GetBinContent(i) + h->GetBinError(i);

    if (binHeight > maxWithErrors) maxWithErrors = binHeight;
  }

  return maxWithErrors;
}


//------------------------------------------------------------------------------
// MoveOverflowBins
//------------------------------------------------------------------------------
void MoveOverflowBins(TH1*     h,
		      Double_t xmin,
		      Double_t xmax)
{
  UInt_t nbins = h->GetNbinsX();

  TAxis* axis = (TAxis*)h->GetXaxis();
  
  UInt_t firstBin = (xmin != -999) ? axis->FindBin(xmin) : 1;
  UInt_t lastBin  = (xmax != -999) ? axis->FindBin(xmax) : nbins;

  Double_t firstVal = 0;
  Double_t firstErr = 0;

  Double_t lastVal = 0;
  Double_t lastErr = 0;

  for (UInt_t i=0; i<=nbins+1; i++) {

    if (i <= firstBin) {
      firstVal += h->GetBinContent(i);
      firstErr += (h->GetBinError(i)*h->GetBinError(i));
    }

    if (i >= lastBin) {
      lastVal += h->GetBinContent(i);
      lastErr += (h->GetBinError(i)*h->GetBinError(i));
    }

    if (i < firstBin || i > lastBin) {
      h->SetBinContent(i, 0);
      h->SetBinError  (i, 0);
    }
  }

  firstErr = sqrt(firstErr);
  lastErr  = sqrt(lastErr);

  h->SetBinContent(firstBin, firstVal);
  h->SetBinError  (firstBin, firstErr);

  h->SetBinContent(lastBin, lastVal);
  h->SetBinError  (lastBin, lastErr);
}


//------------------------------------------------------------------------------
// ZeroOutOfRangeBins
//------------------------------------------------------------------------------
void ZeroOutOfRangeBins(TH1*     h,
			Double_t xmin,
			Double_t xmax)
{
  UInt_t nbins = h->GetNbinsX();

  TAxis* axis = (TAxis*)h->GetXaxis();
  
  UInt_t firstBin = (xmin != -999) ? axis->FindBin(xmin) : 1;
  UInt_t lastBin  = (xmax != -999) ? axis->FindBin(xmax) : nbins;

  for (UInt_t i=0; i<=nbins+1; i++) {

    if (i < firstBin || i > lastBin) {
      h->SetBinContent(i, 0);
      h->SetBinError  (i, 0);
    }
  }
}


//------------------------------------------------------------------------------
// SetAxis
//------------------------------------------------------------------------------
void SetAxis(TH1*    hist,
	     TString xtitle,
	     TString ytitle,
	     Float_t size,
	     Float_t offset)
{
  TAxis* xaxis = (TAxis*)hist->GetXaxis();
  TAxis* yaxis = (TAxis*)hist->GetYaxis();

  xaxis->SetLabelFont(42);
  yaxis->SetLabelFont(42);
  xaxis->SetTitleFont(42);
  yaxis->SetTitleFont(42);

  xaxis->SetLabelOffset(0.025);
  yaxis->SetLabelOffset(0.025);
  xaxis->SetTitleOffset(1.4);
  yaxis->SetTitleOffset(offset);

  xaxis->SetLabelSize(size);
  yaxis->SetLabelSize(size);
  xaxis->SetTitleSize(size);
  yaxis->SetTitleSize(size);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(ytitle);

  xaxis->SetNdivisions(505);
  yaxis->SetNdivisions(505);

  yaxis->CenterTitle();

  gPad->GetFrame()->DrawClone();
  gPad->RedrawAxis();
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


//------------------------------------------------------------------------------
// SetParameters
//------------------------------------------------------------------------------
void SetParameters(UInt_t channel)
{
  sChannel[MMM] = "MMM";
  sChannel[EEE] = "EEE";
  sChannel[MME] = "MME";
  sChannel[EEM] = "EEM";

  sCut[AllEvents]          = "AllEvents";
  sCut[HLT]                = "HLT";
  sCut[Has2IsoGoodLeptons] = "Has2IsoGoodLeptons";
  sCut[Exactly3Leptons]    = "Exactly3Leptons";
  sCut[HasZCandidate]      = "HasZCandidate";
  sCut[HasWCandidate]      = "HasWCandidate";
  sCut[MET]                = "MET";

  process[Data]            = "Double";
  process[ZJets_Madgraph]  = "ZJets_Madgraph";
  process[TW]              = "TW";
  process[TbarW]           = "TbarW";
  process[TTbar_Madgraph]  = "TTbar_Madgraph";
  process[TTbar_Powheg_2L] = "TTbar_Powheg_2L";
  process[WJets_Madgraph]  = "WJets_Madgraph";
  process[ZZ]              = "ZZ";
  process[WW]              = "WW";
  process[WgammaToLNuG]    = "WgammaToLNuG";
  process[ZgammaToLLG]     = "ZgammaToLLG";
  process[WZTo3LNu]        = "WZTo3LNu";

  if (channel == EEE || channel == EEM) process[Data] += "Electron";
  if (channel == MMM || channel == MME) process[Data] += "Mu";

  color[Data]            = kBlack;
  color[WW]              = kAzure;    // kRed+3 (VV)
  color[ZZ]              = kRed+1;    // kRed+3 (VV)
  color[WgammaToLNuG]    = kTeal;     // kRed+3 (VV)
  color[ZgammaToLLG]     = kRed+3;    // kRed+3 (VV)
  color[TW]              = kAzure-3;  // kAzure-9 (top)
  color[TbarW]           = kAzure-3;  // kAzure-9 (top)
  color[TTbar_Madgraph]  = kAzure-9;  // kAzure-9 (top)
  color[TTbar_Powheg_2L] = kAzure-9;  // kAzure-9 (top)
  color[WJets_Madgraph]  = kGray+1;   // kGreen+2 (V + jets)
  color[ZJets_Madgraph]  = kGreen+2;  // kGreen+2 (V + jets)
  color[WZTo3LNu]        = kOrange-2;

  systError[Data]            = 0.0;
  systError[ZJets_Madgraph]  = 0.0 / 1e2;
  systError[TW]              = 0.0 / 1e2;
  systError[TbarW]           = 0.0 / 1e2;
  systError[TTbar_Madgraph]  = 0.0 / 1e2;
  systError[TTbar_Powheg_2L] = 0.0 / 1e2;
  systError[WJets_Madgraph]  = 0.0 / 1e2;
  systError[ZZ]              = 0.0 / 1e2;
  systError[WW]              = 0.0 / 1e2;
  systError[WgammaToLNuG]    = 0.0 / 1e2;
  systError[ZgammaToLLG]     = 0.0 / 1e2;
  systError[WZTo3LNu]        = 0.0 / 1e2;

  _setLogy    = false;
  //  _luminosity = 12103.3;  // 12103.3 for PU
  _luminosity = 19602.0;  // 19468.3 for PU
  _yoffset    = 0.048;
  _verbosity  = 3;
  _directory  = "Summer12_53X/WH";
  _format     = "png";

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetHatchesLineWidth(  1);
  gStyle->SetHatchesSpacing  (0.7);
  
  gSystem->mkdir(_format + "/" + _directory, kTRUE);

  TH1::SetDefaultSumw2();

  vprocess.clear();
  vprocess.push_back(Data);
  vprocess.push_back(WW);               // VV
  vprocess.push_back(ZZ);               // VV
  vprocess.push_back(WgammaToLNuG);     // VV
  vprocess.push_back(ZgammaToLLG);      // VV
  vprocess.push_back(TW);               // top
  vprocess.push_back(TbarW);            // top
  vprocess.push_back(TTbar_Madgraph);   // top
  vprocess.push_back(WJets_Madgraph);   // V + jets
  vprocess.push_back(ZJets_Madgraph);   // V + jets
  vprocess.push_back(WZTo3LNu);         // WZ


  // Read input files
  //----------------------------------------------------------------------------
  TString path = Form("/nfs/fanae/user/piedra/work/WZXS8TeV/results/%s/",
  		      _directory.Data());

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    input[j] = new TFile(path + process[j] + ".root");
  }
}
