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
const UInt_t nProcesses = 39;

enum {MMM, EEE, MME, EEM};

enum {
  AllEvents,
  HLT,
  Has2IsoGoodLeptons,
  AtLeast3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET
};

enum {
  Data,
  DataPPF,
  DataPPP,
  DYJets_Madgraph,
  ZJets_Madgraph,
  ZbbToLL,
  WbbToLNu,
  WJets_Madgraph,
  WGstarToElNuMad,
  WGstarToMuNuMad,
  WGstarToTauNuMad,
  WgammaToLNuG,
  ZgammaToLLG,
  TTbar_Madgraph,
  TW,
  TbarW,
  WW,
  WZTo3LNu,
  WZTo2L2QMad,
  ZZTo2L2QMad,
  ZZ,  // Not used
  ggZZ2L2L,
  ggZZ4L,
  ZZ2Mu2Tau,
  ZZ4E,
  ZZ2E2Tau,
  ZZ4Mu,
  ZZ2E2Mu,
  ZZ4Tau,
  HZZ4L,
  WWGJets,
  WZZJets,
  ZZZJets,
  WWZJets,
  WWWJets,
  TTWJets,
  TTZJets,
  TTWWJets,
  TTGJets
};


enum {linY = 0, logY};

enum {MCmode, PPFmode, PPPmode};


// Settings
//------------------------------------------------------------------------------
TString         sChannel [nChannels];
TString         sCut     [nCuts];
TFile*          input    [nProcesses];
TString         process  [nProcesses];
Color_t         color    [nProcesses];
Double_t        systError[nProcesses];

Bool_t         _closure_test;
Bool_t         _normalizeMC2Data;
Double_t       _luminosity;
Double_t       _yoffset;
Int_t          _verbosity;
TString        _directory;
TString        _format;
TString        _output;
UInt_t         _cut;
UInt_t         _mode;

vector<UInt_t>  vprocess;


// Member functions
//------------------------------------------------------------------------------
void     SetParameters            (UInt_t      cut,
				   UInt_t      mode,
				   UInt_t      closure_test);

Int_t    ReadInputFiles           (UInt_t      channel);

void     MeasureTheCrossSection   (UInt_t      channel);

void     DrawHistogram            (TString     hname,
				   TString     xtitle,
				   Int_t       ngroup       = -1,
				   Int_t       precision    =  0,
				   TString     units        = "NULL",
				   Bool_t      setLogy      = false,
				   Double_t    xmin         = -999,
				   Double_t    xmax         = -999,
				   Double_t    ymin         = -999,
				   Double_t    ymax         = -999,
				   Bool_t      moveOverflow = true);

Double_t GetMaximumIncludingErrors(TH1*        h,
				   Double_t    xmin = -999,
				   Double_t    xmax = -999);


void     MoveOverflowBins         (TH1*        h,
				   Double_t    xmin = -999,
				   Double_t    xmax = -999);

void     ZeroOutOfRangeBins       (TH1*        h,
				   Double_t    xmin = -999,
				   Double_t    xmax = -999);

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

Double_t Yield                    (TH1*        h);

TString  GuessLocalBasePath       ();


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(UInt_t cut          = AtLeast3Leptons,
	UInt_t mode         = PPFmode,
	UInt_t closure_test = 1,
	Bool_t batch        = false)
{
  if (cut < AtLeast3Leptons) return;

  gROOT->SetBatch(batch);

  SetParameters(cut, mode, closure_test);

  for (UInt_t channel=0; channel<nChannels; channel++) {

    if (channel != MMM) continue;

    if (ReadInputFiles(channel) < 0) break;
    
    if (cut == MET) MeasureTheCrossSection(channel);

    TString suffix = "_" + sChannel[channel] + "_" + sCut[cut];
    
    DrawHistogram("hNPV"          + suffix, "number of PV",               -1, 0, "NULL", linY, 0, 30);
    DrawHistogram("hSumCharges"   + suffix, "q_{1} + q_{2} + q_{3}",      -1, 0, "NULL", linY);
    DrawHistogram("hMET"          + suffix, "E_{T}^{miss}",                5, 0, "GeV",  linY);
    DrawHistogram("hInvMass2Lep"  + suffix, "m_{#font[12]{ll}}",           2, 0, "GeV",  linY, 60, 120);
    DrawHistogram("hInvMass3Lep"  + suffix, "m_{#font[12]{3l}}",          10, 0, "GeV",  linY);
    DrawHistogram("hPtLepton1"    + suffix, "p_{T}^{first lepton}",        5, 0, "GeV",  linY);
    DrawHistogram("hPtLepton2"    + suffix, "p_{T}^{second lepton}",       5, 0, "GeV",  linY);
    DrawHistogram("hPtLepton3"    + suffix, "p_{T}^{third lepton}",        5, 0, "GeV",  linY);
    DrawHistogram("hDPhiZLeptons" + suffix, "#Delta#phi_{#font[12]{ll}}",  5, 1, "^{o}", linY);
    DrawHistogram("hPtZLepton1"   + suffix, "p_{T}^{Z leading lepton}",    5, 0, "GeV",  linY);
    DrawHistogram("hPtZLepton2"   + suffix, "p_{T}^{Z trailing lepton}",   5, 0, "GeV",  linY);
    DrawHistogram("hPtWLepton"    + suffix, "p_{T}^{W lepton}",            5, 0, "GeV",  linY);
  }
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

  Double_t nWZ = ((TH1D*)input[WZTo3LNu]->Get("hCounterEff_" + sChannel[channel] + "_MET"))->Integral();

  TString hname = "hCounter_" + sChannel[channel] + "_MET";

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TH1D* dummy = (TH1D*)input[j]->Get(hname);

    if (j == Data) {
      ndata = dummy->Integral();
    }
    else if (j == WZTo3LNu) {
      nsignal = dummy->Integral();
    }
    else {
      nbackground += dummy->Integral();
      if (j == TTbar_Madgraph)
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
		   Bool_t   setLogy,
		   Double_t xmin,
		   Double_t xmax,
		   Double_t ymin,
		   Double_t ymax,
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
  
  pad1->SetLogy(setLogy);

  Double_t mcIntegral = 0;

  THStack* hstack = new THStack(hname, hname);

  TH1D* hist[nProcesses];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1D*)input[j]->Get(hname);
    hist[j]->SetName(hname + "_" + process[j]);

    if (moveOverflow) MoveOverflowBins  (hist[j], xmin, xmax);
    else              ZeroOutOfRangeBins(hist[j], xmin, xmax);
    
    if (ngroup > 0) hist[j]->Rebin(ngroup);

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
  if (_normalizeMC2Data)
    {
      for (UInt_t i=0; i<vprocess.size(); i++)
	{
	  UInt_t j = vprocess.at(i);
        
	  if (j != Data && mcIntegral > 0)
	    {
	      hist[j]->Scale(hist[Data]->Integral() / mcIntegral);
	    }
	}
    }


  // All MC
  //----------------------------------------------------------------------------
  TH1D* allmc = (TH1D*)hist[Data]->Clone("allmc");

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

  if (pad1->GetLogy())
    theMax = TMath::Power(10, TMath::Log10(theMax) + 2);
  else
    theMax *= 1.5;

  hist[Data]->SetMaximum(theMax);

  if (ymin != -999) hist[Data]->SetMinimum(ymin);
  if (ymax != -999) hist[Data]->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.72;
  Double_t y0     = 0.834;
  Double_t delta  = _yoffset + 0.001;
  Double_t ndelta = 0;
  
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[Data],     Form(" data (%.0f)", Yield(hist[Data])),     "lp"); ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, allmc,          Form(" all (%.0f)",  Yield(allmc)),          "f");  ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[WZTo3LNu], Form(" WZ (%.0f)",   Yield(hist[WZTo3LNu])), "f");  ndelta += delta;

  if (_mode == PPPmode)
    {
      DrawLegend(x0 - 0.49, y0 - ndelta, hist[DataPPP], Form(" PPP (%.0f)", Yield(hist[DataPPP])), "f"); ndelta += delta;
      DrawLegend(x0 - 0.49, y0 - ndelta, hist[DataPPF], Form(" PPF (%.0f)", Yield(hist[DataPPF])), "f"); ndelta += delta;
    }
  else
    {
      if (_mode == MCmode)
	{
	  Double_t TopYield   = 0.0;
	  Double_t ZJetsYield = 0.0;

	  TopYield += Yield(hist[TTbar_Madgraph]);
	  TopYield += Yield(hist[TW]);
	  TopYield += Yield(hist[TbarW]);

	  ZJetsYield += Yield(hist[DYJets_Madgraph]);
	  ZJetsYield += Yield(hist[ZJets_Madgraph]);
	  ZJetsYield += Yield(hist[ZbbToLL]);

	  DrawLegend(x0 - 0.49, y0 - ndelta, hist[TTbar_Madgraph], Form(" top (%.0f)",    TopYield),   "f"); ndelta += delta;
	  DrawLegend(x0 - 0.49, y0 - ndelta, hist[ZJets_Madgraph], Form(" Z+jets (%.0f)", ZJetsYield), "f"); ndelta += delta;
	}
      else if (_mode == PPFmode)
	{
	  DrawLegend(x0 - 0.49, y0 - ndelta, hist[DataPPF], Form(" data-driven (%.0f)", Yield(hist[DataPPF])), "f"); ndelta += delta;
	}

      Double_t ZZYield      = 0.0;
      Double_t WVYield      = 0.0;
      Double_t VVVJetsYield = 0.0;

      ZZYield += Yield(hist[ZgammaToLLG]);
      ZZYield += Yield(hist[ggZZ2L2L]);
      ZZYield += Yield(hist[ggZZ4L]);
      ZZYield += Yield(hist[ZZ2Mu2Tau]);
      ZZYield += Yield(hist[ZZ4E]);
      ZZYield += Yield(hist[ZZ2E2Tau]);
      ZZYield += Yield(hist[ZZ4Mu]);
      ZZYield += Yield(hist[ZZ2E2Mu]);
      ZZYield += Yield(hist[ZZ4Tau]);
      ZZYield += Yield(hist[HZZ4L]);

      WVYield += Yield(hist[WW]);
      WVYield += Yield(hist[WZTo2L2QMad]);
      WVYield += Yield(hist[WbbToLNu]);
      WVYield += Yield(hist[WJets_Madgraph]);
      WVYield += Yield(hist[WGstarToElNuMad]);
      WVYield += Yield(hist[WGstarToMuNuMad]);
      WVYield += Yield(hist[WGstarToTauNuMad]);
      WVYield += Yield(hist[WgammaToLNuG]);

      VVVJetsYield += Yield(hist[WWGJets]);
      VVVJetsYield += Yield(hist[WZZJets]);
      VVVJetsYield += Yield(hist[ZZZJets]);
      VVVJetsYield += Yield(hist[WWZJets]);
      VVVJetsYield += Yield(hist[WWWJets]);
      VVVJetsYield += Yield(hist[TTWJets]);
      VVVJetsYield += Yield(hist[TTZJets]);
      VVVJetsYield += Yield(hist[TTWWJets]);
      VVVJetsYield += Yield(hist[TTGJets]);
      
      ndelta = 0;

      DrawLegend(x0 - 0.23, y0 - ndelta, hist[ggZZ2L2L], Form(" ZZ (%.0f)",  ZZYield),      "f"); ndelta += delta;
      DrawLegend(x0 - 0.23, y0 - ndelta, hist[WW],       Form(" WV (%.0f)",  WVYield),      "f"); ndelta += delta;
      DrawLegend(x0 - 0.23, y0 - ndelta, hist[WWGJets],  Form(" VVV (%.0f)", VVVJetsYield), "f"); ndelta += delta;
    }


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
    
  TH1D* ratio       = (TH1D*)hist[Data]->Clone("ratio");
  TH1D* uncertainty = (TH1D*)allmc->Clone("uncertainty");

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

  TH1D* hdeviations = new TH1D("hdeviations", "", 25, 0, 2.5);

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

  TString suffixLogy = (setLogy) ? "_log" : "";

  canvas->SaveAs(Form("%s/%s%s.%s",
		      _output.Data(),
		      hname.Data(),
		      suffixLogy.Data(),
		      _format.Data()));
}


//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Double_t GetMaximumIncludingErrors(TH1*     h,
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
void SetParameters(UInt_t cut,
		   UInt_t mode,
		   UInt_t closure_test)
{
  sChannel[MMM] = "MMM";
  sChannel[EEE] = "EEE";
  sChannel[MME] = "MME";
  sChannel[EEM] = "EEM";

  sCut[AllEvents]          = "AllEvents";
  sCut[HLT]                = "HLT";
  sCut[Has2IsoGoodLeptons] = "Has2IsoGoodLeptons";
  sCut[AtLeast3Leptons]    = "AtLeast3Leptons";
  sCut[HasZCandidate]      = "HasZCandidate";
  sCut[HasWCandidate]      = "HasWCandidate";
  sCut[MET]                = "MET";

  process[DYJets_Madgraph]  = "DYJets_Madgraph";
  process[ZJets_Madgraph]   = "ZJets_Madgraph";
  process[ZbbToLL]          = "ZbbToLL";
  process[WbbToLNu]         = "WbbToLNu";
  process[WJets_Madgraph]   = "WJets_Madgraph";
  process[WGstarToElNuMad]  = "WGstarToElNuMad";
  process[WGstarToMuNuMad]  = "WGstarToMuNuMad";
  process[WGstarToTauNuMad] = "WGstarToTauNuMad";
  process[WgammaToLNuG]     = "WgammaToLNuG";
  process[ZgammaToLLG]      = "ZgammaToLLG";
  process[TTbar_Madgraph]   = "TTbar_Madgraph";
  process[TW]               = "TW";
  process[TbarW]            = "TbarW";
  process[WW]               = "WW";
  process[WZTo3LNu]         = "WZTo3LNu";
  process[WZTo2L2QMad]      = "WZTo2L2QMad";
  process[ZZTo2L2QMad]      = "ZZTo2L2QMad";
  process[ZZ]               = "ZZ";
  process[ggZZ2L2L]         = "ggZZ2L2L";
  process[ggZZ4L]           = "ggZZ4L";
  process[ZZ2Mu2Tau]        = "ZZ2Mu2Tau";
  process[ZZ4E]             = "ZZ4E";
  process[ZZ2E2Tau]         = "ZZ2E2Tau";
  process[ZZ4Mu]            = "ZZ4Mu";
  process[ZZ2E2Mu]          = "ZZ2E2Mu";
  process[ZZ4Tau]           = "ZZ4Tau";
  process[HZZ4L]            = "HZZ4L";
  process[WWGJets]          = "WWGJets";
  process[WZZJets]          = "WZZJets";
  process[ZZZJets]          = "ZZZJets";
  process[WWZJets]          = "WWZJets";
  process[WWWJets]          = "WWWJets";
  process[TTWJets]          = "TTWJets";
  process[TTZJets]          = "TTZJets";
  process[TTWWJets]         = "TTWWJets";
  process[TTGJets]          = "TTGJets";

  color[Data]             = kBlack;
  color[DataPPF]          = kGray+1;
  color[DataPPP]          = kOrange-2;
  color[DYJets_Madgraph]  = kGreen+2;   // Z+jets
  color[ZJets_Madgraph]   = kGreen+2;   // Z+jets
  color[ZbbToLL]          = kGreen+2;   // Z+jets
  color[WbbToLNu]         = kAzure;     // WV
  color[WJets_Madgraph]   = kAzure;     // WV
  color[WGstarToElNuMad]  = kAzure;     // WV
  color[WGstarToMuNuMad]  = kAzure;     // WV
  color[WGstarToTauNuMad] = kAzure;     // WV
  color[WgammaToLNuG]     = kAzure;     // WV
  color[WW]               = kAzure;     // WV
  color[WZTo2L2QMad]      = kAzure;     // WV
  color[TTbar_Madgraph]   = kAzure-9;   // top
  color[TW]               = kAzure-9;   // top
  color[TbarW]            = kAzure-9;   // top
  color[WZTo3LNu]         = kOrange-2;  // WZ
  color[ZZTo2L2QMad]      = kRed+1;     // ZZ
  color[ZgammaToLLG]      = kRed+1;     // ZZ
  color[ZZ]               = kRed+1;     // ZZ
  color[ggZZ2L2L]         = kRed+1;     // ZZ
  color[ggZZ4L]           = kRed+1;     // ZZ
  color[ZZ2Mu2Tau]        = kRed+1;     // ZZ
  color[ZZ4E]             = kRed+1;     // ZZ
  color[ZZ2E2Tau]         = kRed+1;     // ZZ
  color[ZZ4Mu]            = kRed+1;     // ZZ
  color[ZZ2E2Mu]          = kRed+1;     // ZZ
  color[ZZ4Tau]           = kRed+1;     // ZZ
  color[HZZ4L]            = kRed+1;     // ZZ
  color[WWGJets]          = kBlack;     // VVV
  color[WZZJets]          = kBlack;     // VVV
  color[ZZZJets]          = kBlack;     // VVV
  color[WWZJets]          = kBlack;     // VVV
  color[WWWJets]          = kBlack;     // VVV
  color[TTWJets]          = kBlack;     // VVV
  color[TTZJets]          = kBlack;     // VVV
  color[TTWWJets]         = kBlack;     // VVV
  color[TTGJets]          = kBlack;     // VVV

  for (UInt_t i=0; i<nProcesses; i++) systError[i] = 0.0;

  _normalizeMC2Data = false;
  _luminosity       = 19602.0;  // 19468.3 for PU
  _yoffset          = 0.048;
  _verbosity        = 3;
  _directory        = "Summer12_53X/WH";
  _format           = "png";
  _cut              = cut;
  _mode             = mode;
  _closure_test     = closure_test;

  if (_normalizeMC2Data) printf("\n WARNING: normalizing MC to data\n\n");

  TString patch = (_normalizeMC2Data) ? "normalized/" : "";

  _output = _format + "/" + _directory + "/" + patch + sCut[_cut];

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetHatchesLineWidth(  1);
  gStyle->SetHatchesSpacing  (0.7);
  
  gSystem->mkdir(_output, kTRUE);

  TH1::SetDefaultSumw2();

  vprocess.clear();

  vprocess.push_back(Data);

  if (mode == PPPmode)
    {
      vprocess.push_back(DataPPF);
      vprocess.push_back(DataPPP);
    }
  else
    {
      vprocess.push_back(WbbToLNu);
      vprocess.push_back(WJets_Madgraph);
      vprocess.push_back(WGstarToElNuMad);
      vprocess.push_back(WGstarToMuNuMad);
      vprocess.push_back(WGstarToTauNuMad);
      vprocess.push_back(WgammaToLNuG);
      vprocess.push_back(WW);
      vprocess.push_back(WZTo2L2QMad);

      if (mode == MCmode)
	{
	  vprocess.push_back(TTbar_Madgraph);
	  vprocess.push_back(TW);
	  vprocess.push_back(TbarW);
	  vprocess.push_back(ZZTo2L2QMad);
	  vprocess.push_back(DYJets_Madgraph);
	  vprocess.push_back(ZJets_Madgraph);
	  vprocess.push_back(ZbbToLL);
	}
      else if (mode == PPFmode)
	{
	  vprocess.push_back(DataPPF);
	}
      
      vprocess.push_back(ZgammaToLLG);
      vprocess.push_back(ggZZ2L2L);
      vprocess.push_back(ggZZ4L);
      vprocess.push_back(ZZ2Mu2Tau);
      vprocess.push_back(ZZ4E);
      vprocess.push_back(ZZ2E2Tau);
      vprocess.push_back(ZZ4Mu);
      vprocess.push_back(ZZ2E2Mu);
      vprocess.push_back(ZZ4Tau);
      vprocess.push_back(HZZ4L);
      vprocess.push_back(WWGJets);
      vprocess.push_back(WZZJets);
      vprocess.push_back(ZZZJets);
      vprocess.push_back(WWZJets);
      vprocess.push_back(WWWJets);
      vprocess.push_back(TTWJets);
      vprocess.push_back(TTZJets);
      vprocess.push_back(TTWWJets);
      vprocess.push_back(TTGJets);
      vprocess.push_back(WZTo3LNu);
    }
}


//------------------------------------------------------------------------------
// ReadInputFiles
//------------------------------------------------------------------------------
Int_t ReadInputFiles(UInt_t channel)
{
  if (channel == EEE || channel == EEM)
    {
      process[Data]    = "DoubleElectron";
      process[DataPPF] = "DoubleElectron_PPF";
      process[DataPPP] = "DoubleElectron_PPP";
    }
  else if (channel == MMM || channel == MME)
    {
      process[Data]    = "DoubleMu";
      process[DataPPF] = "DoubleMu_PPF";
      process[DataPPP] = "DoubleMu_PPP";
    }

  TString path = Form("%s/piedra/work/WZXS8TeV/results/%s/",
		      GuessLocalBasePath().Data(),
		      _directory.Data());

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TString suffix = (_closure_test) ? "_closure_test" : "";

    input[j] = new TFile(path + process[j] + suffix + ".root");


    // Debug
    //--------------------------------------------------------------------------
    TH1D* dummy = (TH1D*)input[j]->Get("hCounter_MMM_AtLeast3Leptons");

    if (!dummy)
      {
	printf(" [ReadInputFiles] The %s file is broken or it does not exist.\n", process[j].Data());

	return -1;
      }
  }

  return 0;
}


//------------------------------------------------------------------------------
// Yield
//------------------------------------------------------------------------------
Double_t Yield(TH1* h)
{
  Int_t nbins = h->GetNbinsX();

  return h->Integral(0, nbins+1);
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
      return TString("/gpfs/csic_user");
    }
  else
    {
      printf(" ERROR: Could not guess base path from host name %s.", host.Data());

      return TString("");
    }
}
