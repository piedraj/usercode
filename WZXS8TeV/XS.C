#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include <fstream>
#include <iostream>
#include <vector>


//------------------------------------------------------------------------------
//
//  MCFM 6.6
//
//------------------------------------------------------------------------------
//
//  xs = 13.24 +  7.68 = 20.92 pb                 // Scale x1 ( 85.79 GeV) with 81 < mZ < 101 GeV
//  xs = 12.72 +  7.37 = 20.09 pb (-0.8 = -4.0%)  // Scale x2 (171.58 GeV) with 81 < mZ < 101 GeV
//  xs = 13.94 +  8.09 = 22.03 pb (+1.1 = +5.3%)  // Scale /2 ( 42.90 GeV) with 81 < mZ < 101 GeV
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
//  MCFM 6.3
//
//------------------------------------------------------------------------------
//
// scale systematic
//
//  xs = 13.89 + 8.06 = 21.95 pb                 // Scale x1 ( 85.79 GeV) with 71 < mZ < 111 GeV
//  xs = 13.33 + 7.72 = 21.05 pb (-0.9 = -4.1%)  // Scale x2 (171.58 GeV) with 71 < mZ < 111 GeV
//  xs = 14.63 + 8.49.= 23.12 pb (+1.2 = +5.3%)  // Scale /2 ( 42.90 GeV) with 71 < mZ < 111 GeV
//
//
// pdf systematic
// 
//  xs = 13.89 + 8.06 = 21.95 pb        // 'mstw8nlo' [pdlabel]
//  xs = 13.68 + 7.61 = 21.29 pb (-3%)  // 'cteq66m'  [pdlabel]
//
//
// test different mZ windows
//
//  xs = 20.67 + 12.42 = 33.09 pb  // Scale x1 (-1d0) with 12 < mZ       GeV
//  xs = 13.89 +  8.06 = 21.95 pb  // Scale x1 (-1d0) with 71 < mZ < 111 GeV
//  xs = 13.67 +  7.93 = 21.60 pb  // Scale x1 (-1d0) with 76 < mZ < 106 GeV
//  xs = 13.28 +  7.69 = 20.97 pb  // Scale x1 (-1d0) with 81 < mZ < 101 GeV
//
//------------------------------------------------------------------------------


// Input parameters for the WZ cross section
//------------------------------------------------------------------------------
const Double_t xsWplusZ  = 13.24;  // pb (MCFM with 81 < mZ < 101 GeV)
const Double_t xsWminusZ =  7.68;  // pb (MCFM with 81 < mZ < 101 GeV)

const Double_t W2e         = 0.1075;
const Double_t W2m         = 0.1057;
const Double_t W2tau       = 0.1125;
const Double_t Z2ll        = 0.033658;
const Double_t WZ23lnu     = 3 * Z2ll * (W2e + W2m + W2tau);
const Double_t ngenWZ      = 2017979;
//const Double_t ngenWZphase = 1449067;  // (71 < mZ < 111 GeV)
const Double_t ngenWZphase = 1390244;  // (81 < mZ < 101 GeV)


// Data members
//------------------------------------------------------------------------------
const UInt_t nChannel = 4;

enum {
  MMM,
  EEE,
  MME,
  EEM,
  LLL
};

TString sChannel[nChannel+1] = {
  "MMM",
  "EEE",
  "MME",
  "EEM",
  "LLL"
};

TString lChannel[nChannel+1] = {
  "#mu#mu#mu",
  "eee",
  "#mu#mue",
  "ee#mu",
  "inclusive"
};

TString pdfChannel[nChannel+1] = {
  "$\\mu\\mu\\mu$",
  "eee",
  "$\\mu\\mu$e",
  "ee$\\mu$",
  "inclusive"
};


const UInt_t nCut = 6;

enum {
  Exactly3SoftLeptons,
  Exactly3Leptons,
  HasZ20,
  HasZ10,
  MET30,
  ClosureTest
};

TString sCut[nCut];


const UInt_t nProcess = 9;

enum {
  Data,
  Fakes,
  WZ,
  ZZ,
  ZG,
  Top,
  ZJets,
  VVV,
  WV
};

Color_t cProcess[nProcess];
TFile*  input   [nProcess];
TString sProcess[nProcess];


// Systematics
//------------------------------------------------------------------------------
const UInt_t nSystematic = 8;

enum {
  fakesSyst,
  qcdSyst,
  pdfSyst,
  metSyst,
  triggerSyst,
  muonSyst,
  electronSyst,
  pileupSyst
};

TString sSystematic[nSystematic] = {
  "fakes",
  "qcd",
  "pdf",
  "met",
  "trigger",
  "muon",
  "electron",
  "pileup"
};

Double_t totalSyst[nChannel][nProcess];

Double_t systematicError[nChannel][nProcess][nSystematic];


enum {linY, logY};

enum {MCmode, PPFmode, PPPmode};


// Settings
//------------------------------------------------------------------------------
Double_t        _luminosity;
Double_t        _luminosityUncertainty;
Double_t        _yoffset;
Int_t           _verbosity;
TString         _localpath;
TString         _datapath;
TString         _output;
UInt_t          _cut;
UInt_t          _mode;

vector<UInt_t>  vprocess;


// Save the cross section
//------------------------------------------------------------------------------
Double_t        wzEffValue [nChannel+1];
Double_t        wzEffError [nChannel+1];
Double_t        xsValue    [nChannel+1];
Double_t        xsErrorLumi[nChannel+1];
Double_t        xsErrorStat[nChannel+1];
Double_t        xsErrorEff [nChannel+1];
Double_t        xsErrorBkgs[nChannel+1];
Double_t        xsErrorSyst[nChannel+1];


// Member functions
//------------------------------------------------------------------------------
void     SetParameters            (UInt_t        cut,
				   UInt_t        mode);

Int_t    ReadInputFiles           ();

void     MeasureTheCrossSection   (UInt_t        channel,
				   UInt_t        cut);

void     PrintYields              (UInt_t        channel);

void     PrintSystematics         (UInt_t        cut);

void     DrawHistogram            (TString       hname,
				   UInt_t        channel,
				   UInt_t        cut,
				   TString       xtitle,
				   Int_t         ngroup       = -1,
				   Int_t         precision    =  0,
				   TString       units        = "NULL",
				   Bool_t        setLogy      = false,
				   Double_t      xmin         = -999,
				   Double_t      xmax         = -999,
				   Double_t      ymin         = -999,
				   Double_t      ymax         = -999,
				   Bool_t        moveOverflow = true);

Double_t GetMaximumIncludingErrors(TH1*          h,
				   Double_t      xmin = -999,
				   Double_t      xmax = -999);

void     MoveOverflowBins         (TH1*          h,
				   Double_t      xmin = -999,
				   Double_t      xmax = -999);

void     SetAxis                  (TH1*          hist,
				   TString       xtitle,
				   TString       ytitle,
				   Float_t       size,
				   Float_t       offset);

void     DrawTLatex               (Double_t      x,
				   Double_t      y,
				   Double_t      tsize,
				   Short_t       align,
				   const char*   text,
				   Bool_t        setndc = true);

TLegend* DrawLegend               (Float_t       x1,
				   Float_t       y1,
				   TH1*          hist,
				   TString       label,
				   TString       option,
				   Float_t       tsize   = 0.03,
				   Float_t       xoffset = 0.20,
				   Float_t       yoffset = _yoffset);

Double_t Yield                    (TH1*          h);

TString  GuessLocalBasePath       ();

void     MakeOutputDirectory      (TString       format);

void     Inclusive                ();

void     RelativeSystematics      (UInt_t        channel,
				   UInt_t        cut);

void     DrawCrossSections        (UInt_t        cut);

void     PrintCrossSections       (UInt_t        cut);

Double_t RelativeDifference       (Double_t      x0,
				   Double_t      x1);


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(UInt_t cut  = MET30,
	UInt_t mode = PPFmode)
{
  gROOT->SetBatch();

  SetParameters(cut, mode);

  if (ReadInputFiles() < 0) return;

  for (UInt_t channel=0; channel<nChannel; channel++) {

    RelativeSystematics(channel, cut);

    MeasureTheCrossSection(channel, cut);

    PrintYields(channel);

    DrawHistogram("hSumCharges", channel, cut, "q_{1} + q_{2} + q_{3}");

    DrawHistogram("hMET",          channel, cut, "E_{T}^{miss}",                           5, 0, "GeV",  linY);
    DrawHistogram("hInvMass2Lep",  channel, cut, "m_{#font[12]{ll}}",                     -1, 0, "GeV",  linY, 76, 106);
    DrawHistogram("hInvMass3Lep",  channel, cut, "m_{#font[12]{3l}}",                      5, 0, "GeV",  linY, 60, 350);
    DrawHistogram("hPtLepton1",    channel, cut, "p_{T}^{first lepton}",                   5, 0, "GeV",  linY);
    DrawHistogram("hPtLepton2",    channel, cut, "p_{T}^{second lepton}",                  5, 0, "GeV",  linY);
    DrawHistogram("hPtLepton3",    channel, cut, "p_{T}^{third lepton}",                   5, 0, "GeV",  linY);
    DrawHistogram("hPtLeadingJet", channel, cut, "p_{T}^{leading jet}",                    5, 0, "GeV",  linY);
    DrawHistogram("hDPhiZLeptons", channel, cut, "#Delta#phi_{#font[12]{ll}}",            10, 1, "^{o}", linY);
    DrawHistogram("hPtZLepton1",   channel, cut, "p_{T}^{Z leading lepton}",               5, 0, "GeV",  linY);
    DrawHistogram("hPtZLepton2",   channel, cut, "p_{T}^{Z trailing lepton}",              5, 0, "GeV",  linY);
    DrawHistogram("hPtWLepton",    channel, cut, "p_{T}^{W lepton}",                       5, 0, "GeV",  linY);
    DrawHistogram("hPtZ",          channel, cut, "p_{T}^{Z}",                             10, 0, "GeV",  linY);
    DrawHistogram("hDRWZLepton1",  channel, cut, "#DeltaR(W lepton, Z leading lepton)",    5, 1, "NULL", linY);
    DrawHistogram("hDRWZLepton2",  channel, cut, "#DeltaR(W lepton, Z trailing lepton)",   5, 1, "NULL", linY);
    DrawHistogram("hMtW",          channel, cut, "m_{T}^{W}",                              5, 0, "GeV",  linY);
    DrawHistogram("hNJet30",       channel, cut, "number of jets (p_{T}^{jet} > 30 GeV)", -1, 0, "NULL", linY, 0, 4);
  }

  Inclusive();
  
  PrintSystematics(cut);

  DrawCrossSections(cut);

  PrintCrossSections(cut);
}


//------------------------------------------------------------------------------
// MeasureTheCrossSection
//------------------------------------------------------------------------------
void MeasureTheCrossSection(UInt_t channel, UInt_t cut)
{
  Double_t ndata   = 0;
  Double_t nsignal = 0;
  Double_t nbkg    = 0;
  Double_t ebkg    = 0;

  TString suffix = "_" + sChannel[channel] + "_" + sCut[cut] + "_LLL";

  Double_t nWZ = Yield((TH1D*)input[WZ]->Get("hCounterEff" + suffix));

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    Double_t process_yield = Yield((TH1D*)input[j]->Get("hCounter" + suffix));

    if (j == Data)
      {
	ndata = process_yield;
      }
    else if (j == WZ)
      {
	nsignal = process_yield;
      }
    else
      {
	nbkg += process_yield;

	Double_t ebkgStat = sqrt(process_yield);
	Double_t ebkgSyst = totalSyst[channel][j] * process_yield / 1e2;

	ebkg += (ebkgStat * ebkgStat);
	ebkg += (ebkgSyst * ebkgSyst);
      }
  }

  ebkg = sqrt(ebkg);


  // Estimate the cross section
  //----------------------------------------------------------------------------
  wzEffValue[channel] = nWZ / ngenWZphase;
  wzEffError[channel] = totalSyst[channel][WZ] * wzEffValue[channel] / 1e2;

  xsValue[channel] = (ndata - nbkg) / (_luminosity * wzEffValue[channel] * WZ23lnu);


  // Relative errors
  //----------------------------------------------------------------------------
  Double_t xsRelativeErrorLumi = _luminosityUncertainty;
  Double_t xsRelativeErrorStat = 1e2 * sqrt(ndata) / (ndata - nbkg);
  Double_t xsRelativeErrorEff  = totalSyst[channel][WZ];
  Double_t xsRelativeErrorBkgs = 1e2 * ebkg        / (ndata - nbkg);
  Double_t xsRelativeErrorSyst = 0;

  xsRelativeErrorSyst += (xsRelativeErrorBkgs * xsRelativeErrorBkgs);
  xsRelativeErrorSyst += (xsRelativeErrorEff * xsRelativeErrorEff);
  xsRelativeErrorSyst = sqrt(xsRelativeErrorSyst);


  // Absolute errors
  //----------------------------------------------------------------------------
  xsErrorLumi[channel] = xsRelativeErrorLumi * xsValue[channel] / 1e2;  // pb
  xsErrorStat[channel] = xsRelativeErrorStat * xsValue[channel] / 1e2;  // pb
  xsErrorEff [channel] = xsRelativeErrorEff  * xsValue[channel] / 1e2;  // pb
  xsErrorBkgs[channel] = xsRelativeErrorBkgs * xsValue[channel] / 1e2;  // pb
  xsErrorSyst[channel] = xsRelativeErrorSyst * xsValue[channel] / 1e2;  // pb


  // Print the results
  //----------------------------------------------------------------------------
  if (_verbosity > 0)
    {
      printf("\n");
      printf(" WZ efficiency = (%.3f +- %.3f)%s\n", 1e2 * wzEffValue[channel], 1e2 * wzEffError[channel], "%");
      printf("          nbkg = %5.1f +- %4.1f\n",   nbkg,       sqrt(nbkg));
      printf("         ndata = %5.1f +- %4.1f\n",   ndata,      sqrt(ndata));
      printf("  ndata - nbkg = %5.1f +- %4.1f\n",   ndata-nbkg, sqrt(ndata+nbkg));
      printf("       nsignal = %5.1f +- %4.1f\n",   nsignal,    sqrt(nsignal));
      printf("\n       xs(%s) = %.2f +- %.2f (stat) +- %.2f (syst) +- %.2f (lumi) pb\n",
	     sChannel   [channel].Data(),
	     xsValue    [channel],
	     xsErrorStat[channel],
	     xsErrorSyst[channel],
	     xsErrorLumi[channel]);
      printf("       xs(NLO) = %.2f pb\n", xsWplusZ + xsWminusZ);
      printf("\n");
    }
}


//------------------------------------------------------------------------------
// PrintYields
//------------------------------------------------------------------------------
void PrintYields(UInt_t channel)
{
  ofstream outputfile;

  TString suffix = (_mode == MCmode) ? "mc" : "ppf";

  outputfile.open(Form("tex/%s_%s.tex", sChannel[channel].Data(), suffix.Data()));

  Double_t nData [nCut];
  Double_t nFakes[nCut];
  Double_t nWZ   [nCut];
  Double_t nZZ   [nCut];
  Double_t nZG   [nCut];
  Double_t nTop  [nCut];
  Double_t nZJets[nCut];
  Double_t nVVV  [nCut];
  Double_t nWV   [nCut];
  Double_t nBkg  [nCut];
  
  for (UInt_t i=0; i<nCut; i++)
    {
      nData [i] = 0.;
      nFakes[i] = 0.;
      nWZ   [i] = 0.;
      nZZ   [i] = 0.;
      nZG   [i] = 0.;
      nTop  [i] = 0.;
      nZJets[i] = 0.;
      nVVV  [i] = 0.;
      nWV   [i] = 0.;
      nBkg  [i] = 0.;
  
      TH1D* hist[nProcess];

      for (UInt_t j=0; j<vprocess.size(); j++)
	{
	  UInt_t k = vprocess.at(j);

	  TString hname = "hCounter_" + sChannel[channel] + "_" + sCut[i] + "_LLL";

	  hist[k] = (TH1D*)input[k]->Get(hname);

	  hist[k]->SetName(hname + "_" + sProcess[k]);
	}

      nData[i] = Yield(hist[Data]);
      nWZ  [i] = Yield(hist[WZ]);
      nZZ  [i] = Yield(hist[ZZ]);
      nZG  [i] = Yield(hist[ZG]);
      nVVV [i] = Yield(hist[VVV]);
      nWV  [i] = Yield(hist[WV]);

      nBkg[i] = nZZ[i] + nZG[i] + nVVV[i] + nWV[i];

      if (_mode == MCmode)
	{
	  nTop  [i] = Yield(hist[Top]);
	  nZJets[i] = Yield(hist[ZJets]);

	  nBkg[i] += nTop[i] + nZJets[i];
	}
      else if (_mode == PPFmode)
	{
	  nFakes[i] = Yield(hist[Fakes]);

	  nBkg[i] += nFakes[i];
	}
    }

  
  // Print
  //----------------------------------------------------------------------------
  if (_mode == MCmode)  {outputfile << Form(" %-20s", "top");         for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nTop[i],   sqrt(nTop[i]));   outputfile << "\\\\\n";}
  if (_mode == MCmode)  {outputfile << Form(" %-20s", "Z+jets");      for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZJets[i], sqrt(nZJets[i])); outputfile << "\\\\\n";}
  if (_mode == PPFmode) {outputfile << Form(" %-20s", "data-driven"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nFakes[i], sqrt(nFakes[i])); outputfile << "\\\\\n";}

  outputfile << Form(" %-20s", "ZZ");              for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZZ[i],  sqrt(nZZ[i]));  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "Z $\\to 4\\ell$"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZG[i],  sqrt(nZG[i]));  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "WV");              for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nWV[i],  sqrt(nWV[i]));  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "VVV");             for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nVVV[i], sqrt(nVVV[i])); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "backgrounds");     for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nBkg[i], sqrt(nBkg[i])); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "WZ");              for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nWZ[i],  sqrt(nWZ[i]));  outputfile << "\\\\\n";

  outputfile << " \\hline\n";

  outputfile << Form(" %-20s", "WZ + backgrounds"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nBkg[i]+nWZ[i], sqrt(nBkg[i]+nWZ[i])); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "data");             for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nData[i],       sqrt(nData[i]));       outputfile << "\\\\\n";

  outputfile.close();
}


//------------------------------------------------------------------------------
// PrintSystematics
//------------------------------------------------------------------------------
void PrintSystematics(UInt_t cut)
{
  ofstream outputfile;

  outputfile.open(Form("tex/systematics_%s.tex", sCut[cut].Data()));

  outputfile << Form(" %-30s", "QCD scale");                     for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][qcdSyst]);      outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "PDFs");                          for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][pdfSyst]);      outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "\\MET");                         for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][metSyst]);      outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "lepton and trigger efficiency"); for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][triggerSyst]);  outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "muon momentum scale");           for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][muonSyst]);     outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "electron energy scale");         for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][electronSyst]); outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "pile-up");                       for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", systematicError[i][WZ][pileupSyst]);   outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "WZ efficiency");                 for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", 1e2 * xsErrorEff [i] / xsValue[i]);    outputfile << "\\\\\n"; outputfile << " \\hline\n";
  outputfile << Form(" %-30s", "backgrounds");                   for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", 1e2 * xsErrorBkgs[i] / xsValue[i]);    outputfile << "\\\\\n"; outputfile << " \\hline\n";
  outputfile << Form(" %-30s", "statistical");                   for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", 1e2 * xsErrorStat[i] / xsValue[i]);    outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "systematic");                    for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", 1e2 * xsErrorSyst[i] / xsValue[i]);    outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "luminosity");                    for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", 1e2 * xsErrorLumi[i] / xsValue[i]);    outputfile << "\\\\\n"; outputfile << " \\hline\n";

  outputfile.close();
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   UInt_t   channel,
		   UInt_t   cut,
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
  hname += "_" + sChannel[channel] + "_" + sCut[cut];

  TCanvas* canvas = new TCanvas(hname, hname, 550, 720);

  TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  TPad* pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);

  pad1->SetTopMargin   (0.08);
  pad1->SetBottomMargin(0.02);
  pad1->Draw();

  pad2->SetTopMargin   (0.08);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();
  
  pad1->SetLogy(setLogy);

  THStack* hstack = new THStack(hname, hname);

  TH1D* hist[nProcess];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1D*)input[j]->Get(hname);

    hist[j]->SetName(hname + "_" + sProcess[j]);

    if (moveOverflow) MoveOverflowBins(hist[j], xmin, xmax);
    
    if (ngroup > 0) hist[j]->Rebin(ngroup);

    if (j == Data)
      {
	hist[j]->SetLineColor  (kBlack);
	hist[j]->SetMarkerColor(kBlack);
	hist[j]->SetMarkerStyle(kFullCircle);
	hist[j]->SetTitle("");
      }
    else
      {
	hist[j]->SetFillColor(cProcess[j]);
	hist[j]->SetFillStyle(1001);
	hist[j]->SetLineColor(cProcess[j]);

	hstack->Add(hist[j]);
      }
  }


  // All MC
  //----------------------------------------------------------------------------
  TH1D* allmc = (TH1D*)hist[Data]->Clone("allmc");

  allmc->SetFillColor  (kGray+1);
  allmc->SetFillStyle  (   3345);
  allmc->SetLineColor  (kGray+1);
  allmc->SetMarkerColor(kGray+1);
  allmc->SetMarkerSize (      0);

  for (Int_t ibin=1; ibin<=allmc->GetNbinsX(); ibin++) {

    Double_t binValue = 0;
    Double_t binError = 0;

    for (UInt_t i=0; i<vprocess.size(); i++) {

      UInt_t j = vprocess.at(i);

      if (j == Data) continue;

      Double_t binContent   = hist[j]->GetBinContent(ibin);
      Double_t binStatError = hist[j]->GetBinError(ibin);
      Double_t binSystError = totalSyst[channel][j] * binContent / 1e2;

      binValue += binContent;
      binError += (binStatError * binStatError);
      binError += (binSystError * binSystError);
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
    theMax *= 1.55;

  hist[Data]->SetMaximum(theMax);

  if (ymin != -999) hist[Data]->SetMinimum(ymin);
  if (ymax != -999) hist[Data]->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.720;
  Double_t y0     = 0.834;
  Double_t delta  = _yoffset + 0.001;
  Double_t ndelta = 0;
  
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[Data], Form(" data (%.0f)", Yield(hist[Data])), "lp"); ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, allmc,      Form(" all (%.0f)",  Yield(allmc)),      "f");  ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[WZ],   Form(" WZ (%.0f)",   Yield(hist[WZ])),   "f");  ndelta += delta;

  if (_mode == MCmode)
    {
      DrawLegend(x0 - 0.49, y0 - ndelta, hist[Top],   Form(" top (%.0f)",    Yield(hist[Top])),   "f"); ndelta += delta;
      DrawLegend(x0 - 0.49, y0 - ndelta, hist[ZJets], Form(" Z+jets (%.0f)", Yield(hist[ZJets])), "f"); ndelta += delta;
    }
  else if (_mode == PPFmode)
    {
      DrawLegend(x0 - 0.49, y0 - ndelta, hist[Fakes], Form(" data-driven (%.0f)", Yield(hist[Fakes])), "f"); ndelta += delta;
    }

  ndelta = 0;
  
  DrawLegend(x0 - 0.22, y0 - ndelta, hist[ZZ],  Form(" ZZ (%.0f)",                          Yield(hist[ZZ])),  "f"); ndelta += delta;
  DrawLegend(x0 - 0.22, y0 - ndelta, hist[ZG],  Form(" Z #rightarrow 4#font[12]{l} (%.0f)", Yield(hist[ZG])),  "f"); ndelta += delta;
  DrawLegend(x0 - 0.22, y0 - ndelta, hist[VVV], Form(" VVV (%.0f)",                         Yield(hist[VVV])), "f"); ndelta += delta;
  DrawLegend(x0 - 0.22, y0 - ndelta, hist[WV],  Form(" WV (%.0f)",                          Yield(hist[WV])),  "f"); ndelta += delta;


  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(0.185, 0.975, 0.05, 13, lChannel[channel]);
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


  // Save
  //----------------------------------------------------------------------------
  pad2->cd(); SetAxis(uncertainty, hist[Data]->GetXaxis()->GetTitle(), "data / prediction", 0.10, 0.8);
  pad1->cd(); SetAxis(hist[Data], "", hist[Data]->GetYaxis()->GetTitle(),                   0.05, 1.6);

  canvas->cd();

  TString cname = _output + "/" + hname;

  if (setLogy) cname += "_log";

  canvas->SaveAs(Form("pdf/%s.pdf", cname.Data()));
  canvas->SaveAs(Form("png/%s.png", cname.Data()));
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
		   UInt_t mode)
{
  sCut[Exactly3SoftLeptons] = "Exactly3SoftLeptons";
  sCut[Exactly3Leptons]     = "Exactly3Leptons";
  sCut[HasZ20]              = "HasZ20";
  sCut[HasZ10]              = "HasZ10";
  sCut[MET30]               = "MET30";
  sCut[ClosureTest]         = "ClosureTest";

  sProcess[Data]  = "Data";
  sProcess[Fakes] = "Data_PPF";
  sProcess[WZ]    = "WZTo3LNu";
  sProcess[ZZ]    = "ZZ";
  sProcess[ZG]    = "ZgammaToLLG";
  sProcess[Top]   = "Top";
  sProcess[ZJets] = "ZJets";
  sProcess[VVV]   = "VVV";
  sProcess[WV]    = "WV";

  cProcess[Data]  = kBlack;
  cProcess[Fakes] = kGray+1;
  cProcess[WZ]    = kOrange-2;
  cProcess[ZZ]    = kRed+1;
  cProcess[ZG]    = kRed+2;
  cProcess[Top]   = kAzure-9;
  cProcess[ZJets] = kGreen+2;
  cProcess[VVV]   = kBlack;
  cProcess[WV]    = kAzure;

  _luminosity            = 19602.0;  // pb
  _luminosityUncertainty =     4.4;  // %

  _yoffset   = 0.048;
  _verbosity = 2;
  _cut       = cut;
  _mode      = mode;
  _localpath = GuessLocalBasePath();
  
  _datapath = Form("%s/piedra/work/WZXS8TeV/results/Summer12_53X/WH/CalibratedE",
		   _localpath.Data());

  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  gSystem->mkdir("tex", kTRUE);

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetEndErrorSize    (  5);
  gStyle->SetHatchesLineWidth(  1);
  gStyle->SetHatchesSpacing  (0.7);
  
  TH1::SetDefaultSumw2();


  vprocess.clear();

  vprocess.push_back(Data);
  vprocess.push_back(WV);
  vprocess.push_back(VVV);

  if (mode == MCmode)
    {
      vprocess.push_back(ZJets);
      vprocess.push_back(Top);
    }
  else if (mode == PPFmode)
    {
      vprocess.push_back(Fakes);
    }
      
  vprocess.push_back(ZZ);
  vprocess.push_back(ZG);
  vprocess.push_back(WZ);
}


//------------------------------------------------------------------------------
// ReadInputFiles
//------------------------------------------------------------------------------
Int_t ReadInputFiles()
{
  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    input[j] = new TFile(_datapath + "/analysis/" + sProcess[j] + ".root");

    TH1D* dummy = (TH1D*)input[j]->Get("hCounter_MME_" + sCut[MET30] + "_TTT");

    if (!dummy)
      {
	printf(" [ReadInputFiles] The %s file is broken or it does not exist.\n",
	       sProcess[j].Data());
	
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
  if (h)
    {
      Int_t nbins = h->GetNbinsX();
      
      return h->Integral(0, nbins+1);
    }
  else
    {
      return 0.;
    }
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
  gSystem->mkdir(format, kTRUE);

  gSystem->Exec(Form("cp index.php %s/.", format.Data()));

  _output = "analysis";

  gSystem->mkdir(format + "/" + _output, kTRUE);

  gSystem->Exec(Form("cp index.php %s/%s/.", format.Data(), _output.Data()));

  if (_mode == MCmode)  _output += "/MC";
  if (_mode == PPFmode) _output += "/PPF";

  gSystem->mkdir(format + "/" + _output, kTRUE);

  gSystem->Exec(Form("cp index.php %s/%s/.", format.Data(), _output.Data()));

  _output += "/" + sCut[_cut];

  gSystem->mkdir(format + "/" + _output, kTRUE);

  gSystem->Exec(Form("cp index.php %s/%s/.", format.Data(), _output.Data()));
}


//------------------------------------------------------------------------------
// DrawCrossSections
//------------------------------------------------------------------------------
void DrawCrossSections(UInt_t cut)
{
  if (sCut[cut].Contains("ClosureTest")) return;

  TGraphErrors* gStat = new TGraphErrors(nChannel+1);
  TGraphErrors* gSyst = new TGraphErrors(nChannel+1);
  TGraphErrors* gLumi = new TGraphErrors(nChannel+1);

  for (UInt_t i=0; i<nChannel+1; i++)
    {
      Double_t errorSquared = (xsErrorStat[i] * xsErrorStat[i]);
      
      gStat->SetPointError(i, sqrt(errorSquared), 0.0);
      
      errorSquared += (xsErrorSyst[i] * xsErrorSyst[i]);

      gSyst->SetPointError(i, sqrt(errorSquared), 0.0);

      errorSquared += (xsErrorLumi[i] * xsErrorLumi[i]);

      gLumi->SetPointError(i, sqrt(errorSquared), 0.0);

      gStat->SetPoint(i, xsValue[i], i+1);
      gSyst->SetPoint(i, xsValue[i], i+1);
      gLumi->SetPoint(i, xsValue[i], i+1);
    }


  // Cosmetics
  //----------------------------------------------------------------------------
  gStat->SetLineWidth  (2);
  gStat->SetMarkerSize (1.3);
  gStat->SetMarkerStyle(kFullCircle);

  gSyst->SetLineColor  (kRed);
  gSyst->SetLineWidth  (2);
  gSyst->SetMarkerSize (1.3);
  gSyst->SetMarkerStyle(kFullCircle);

  gLumi->SetLineColor  (kBlue);
  gLumi->SetLineWidth  (2);
  gLumi->SetMarkerSize (1.3);
  gLumi->SetMarkerStyle(kFullCircle);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("xs_" + sCut[cut],
				"xs_" + sCut[cut]);

  canvas->SetLeftMargin(canvas->GetRightMargin());

  Double_t xmin = (xsWplusZ + xsWminusZ) -  8.;
  Double_t xmax = (xsWplusZ + xsWminusZ) + 23.;
  Double_t ymin = 0.50;
  Double_t ymax = nChannel+1 + ymin;
  
  TH2F* dummy = new TH2F("dummy_xs", "",
			 100, xmin, xmax,
			 100, ymin, ymax);

  dummy->Draw();
  
  
  // NLO WZ cross-section
  //----------------------------------------------------------------------------
  TLine* line = new TLine(xsWplusZ+xsWminusZ, ymin,
			  xsWplusZ+xsWminusZ, ymax);

  line->SetLineColor(kGray+2);
  line->SetLineStyle(3);
  line->SetLineWidth(3);

  line->Draw("same");


  // Cross sections
  //----------------------------------------------------------------------------
  gLumi->Draw("p,same");
  gSyst->Draw("p,same");
  gStat->Draw("p,same");


  // Labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel+1; i++) {

    Double_t x = gStat->GetX()[i];
    Double_t y = gStat->GetY()[i];

    DrawTLatex(xmin+1., y, 0.035, 12, Form("%s", lChannel[i].Data()), 0);

    Double_t gStatError  = gStat->GetErrorX(i);
    Double_t gSystError  = gSyst->GetErrorX(i);
    Double_t gLumiError  = gLumi->GetErrorX(i);

    gLumiError = sqrt(gLumiError*gLumiError - gSystError*gSystError);
    gSystError = sqrt(gSystError*gSystError - gStatError*gStatError);

    DrawTLatex(xmax-1., y, 0.035, 32, Form("%.1f #pm %.1f #pm %.1f #pm %.1f",
					    x, gStatError, gSystError, gLumiError), 0);
  }

  DrawTLatex(0.940, 0.983, 0.05, 33,
	     Form("#sqrt{s} = 8 TeV, L = %.1f fb^{-1}", _luminosity/1e3));

  dummy->GetXaxis()->CenterTitle();
  dummy->GetXaxis()->SetTitleOffset(1.4);
  dummy->GetXaxis()->SetTitle("#sigma(pp #rightarrow WZ) [pb]");
  dummy->GetYaxis()->SetTitle("");


  // Remove y-axis labels
  //----------------------------------------------------------------------------
  TAxis* yaxis = dummy->GetYaxis();
  
  for (Int_t j=1; j<yaxis->GetNbins(); j++) yaxis->SetBinLabel(j, "");


  // Save
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();

  canvas->SaveAs(Form("pdf/%s/xs_%s.pdf", _output.Data(), sCut[cut].Data()));
  canvas->SaveAs(Form("png/%s/xs_%s.png", _output.Data(), sCut[cut].Data()));
}


//------------------------------------------------------------------------------
// PrintCrossSections
//------------------------------------------------------------------------------
void PrintCrossSections(UInt_t cut)
{
  if (sCut[cut].Contains("ClosureTest")) return;

  ofstream outputfile;

  TString suffix = (_mode == MCmode) ? "mc" : "ppf";

  outputfile.open(Form("tex/xs_%s_%s.tex", sCut[cut].Data(), suffix.Data()));


  // Print
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel+1; i++)
    {
      if (i == nChannel) outputfile << "\\hline\n";

      outputfile << Form("%s & %.2f $\\pm$ %.2f & %.1f $\\pm$ %.1f (stat.) $\\pm$ %.1f (syst.) $\\pm$ %.1f (lumi.)\\\\\n",
			 pdfChannel[i].Data(),
			 1e2 * wzEffValue[i],
			 1e2 * wzEffError[i],
			 xsValue[i],
			 xsErrorStat[i],
			 xsErrorSyst[i],
			 xsErrorLumi[i]);
    }

  outputfile.close();
}


//------------------------------------------------------------------------------
// Inclusive
//------------------------------------------------------------------------------
void Inclusive()
{
  Double_t x     = 0;
  Double_t lumi  = 0;
  Double_t stat  = 0;
  Double_t eff   = 0;
  Double_t bkgs  = 0;
  Double_t syst  = 0;
  Double_t total = 0;

  for (UInt_t i=0; i<nChannel; i++)
    {
      Double_t xsErrorTotal = 0;

      xsErrorTotal += (xsErrorStat[i] * xsErrorStat[i]);
      xsErrorTotal += (xsErrorSyst[i] * xsErrorSyst[i]);
      xsErrorTotal += (xsErrorLumi[i] * xsErrorLumi[i]);

      xsErrorTotal = sqrt(xsErrorTotal);

      x += (xsValue[i] / xsErrorTotal / xsErrorTotal);

      lumi  += (1. / xsErrorLumi[i]  / xsErrorLumi[i]);
      stat  += (1. / xsErrorStat[i]  / xsErrorStat[i]);
      eff   += (1. / xsErrorEff [i]  / xsErrorEff [i]);
      bkgs  += (1. / xsErrorBkgs[i]  / xsErrorBkgs[i]);
      syst  += (1. / xsErrorSyst[i]  / xsErrorSyst[i]);
      total += (1. / xsErrorTotal    / xsErrorTotal);
    }

  xsValue[nChannel] = x / total;

  xsErrorLumi[nChannel] = 1. / sqrt(lumi);
  xsErrorStat[nChannel] = 1. / sqrt(stat);
  xsErrorEff [nChannel] = 1. / sqrt(eff);
  xsErrorBkgs[nChannel] = 1. / sqrt(bkgs);
  xsErrorSyst[nChannel] = 1. / sqrt(syst);


  // Total efficiency
  //----------------------------------------------------------------------------
  wzEffValue[nChannel] = 0.0;
  wzEffError[nChannel] = 0.0;

  for (UInt_t i=0; i<nChannel; i++)
    {
      wzEffValue[nChannel] += wzEffValue[i];
      wzEffError[nChannel] += (wzEffError[i] * wzEffError[i]);
    }

  wzEffError[nChannel] = sqrt(wzEffError[nChannel]);
}


//------------------------------------------------------------------------------
// RelativeSystematics
//------------------------------------------------------------------------------
void RelativeSystematics(UInt_t channel, UInt_t cut)
{
  for (UInt_t i=0; i<vprocess.size(); i++)
    {
      UInt_t process = vprocess.at(i);

      totalSyst[channel][process] = 0.0;
    }

  for (UInt_t syst=0; syst<nSystematic; syst++)
    {
      for (UInt_t i=0; i<vprocess.size(); i++)
	{
	  UInt_t process = vprocess.at(i);
	  
	  systematicError[channel][process][syst] = 0.0;
	}

      if (syst == fakesSyst) {systematicError[channel][Fakes][syst] = 36.0; continue;}
      if (syst == qcdSyst)   {systematicError[channel][WZ]   [syst] =  1.6; continue;}
      if (syst == pdfSyst)   {systematicError[channel][WZ]   [syst] =  3.0; continue;}

      for (UInt_t i=0; i<vprocess.size(); i++)
	{
	  UInt_t process = vprocess.at(i);

	  if (process == Data)  continue;
	  if (process == Fakes) continue;
      
	  if (syst == triggerSyst)
	    {
	      Double_t leptonSyst = 1.;
	      
	      systematicError[channel][process][syst] = leptonSyst * sqrt(3.);
	      
	      continue;
	    }
	  
	  TString hname = "hCounter_" + sChannel[channel] + "_" + sCut[cut] + "_TTT";
	
	  TString suffix = "/" + sProcess[process] + ".root";

	  TFile* f0 = new TFile(_datapath + "/analysis" + suffix);

	  Double_t y0 = Yield((TH1D*)f0->Get(hname));

	  Double_t y1;
	  
	  if (syst == muonSyst || syst == electronSyst)
	    {
	      TFile* fUp   = new TFile(_datapath + "/systematics/" + sSystematic[syst] + "Up"   + suffix);
	      TFile* fDown = new TFile(_datapath + "/systematics/" + sSystematic[syst] + "Down" + suffix);
	      
	      Double_t yUp   = Yield((TH1D*)fUp  ->Get(hname));
	      Double_t yDown = Yield((TH1D*)fDown->Get(hname));

	      Double_t systUp   = RelativeDifference(y0, yUp);
	      Double_t systDown = RelativeDifference(y0, yDown);

	      y1 = (systUp > systDown) ? yUp : yDown;

	      fUp  ->Close();
	      fDown->Close();
	    }
	  else if (syst == pileupSyst)
	    {
	      TString hnamePU = "hCounterPU_" + sChannel[channel] + "_" + sCut[cut] + "_TTT";

	      y1 = Yield((TH1D*)f0->Get(hnamePU));
	    }
	  else
	    {
	      TFile* f1 = new TFile(_datapath + "/systematics/" + sSystematic[syst] + suffix);

	      y1 = Yield((TH1D*)f1->Get(hname));

	      f1->Close();
	    }

	  f0->Close();

	  systematicError[channel][process][syst] = RelativeDifference(y0, y1);
	}
    }


  totalSyst[channel][Fakes] = systematicError[channel][Fakes][fakesSyst];
  

  for (UInt_t process=0; process<nProcess; process++)
    {
      if (process == Data)  continue;
      if (process == Fakes) continue;

      for (UInt_t syst=0; syst<nSystematic; syst++)
	{
	  if (syst == fakesSyst) continue;
      
	  Double_t syst2 = systematicError[channel][process][syst] * systematicError[channel][process][syst];

	  if (syst == qcdSyst) {if (process == WZ) totalSyst[channel][process] += syst2; continue;}
	  if (syst == pdfSyst) {if (process == WZ) totalSyst[channel][process] += syst2; continue;}

	  totalSyst[channel][process] += syst2;
	}
      
      totalSyst[channel][process] = sqrt(totalSyst[channel][process]);
    }


  // Print
  //----------------------------------------------------------------------------
  if (_verbosity < 2) return;

  printf("\n%10s ", " ");

  for (UInt_t i=0; i<vprocess.size(); i++)
    {
      UInt_t process = vprocess.at(i);

      if (process == Data) continue;

      printf(" %15s ", sProcess[process].Data());
    }
  
  printf("\n");
  
  for (UInt_t syst=0; syst<nSystematic; syst++) {
    
    printf("%10s ", sSystematic[syst].Data());
    
    for (UInt_t i=0; i<vprocess.size(); i++)
      {
	UInt_t process = vprocess.at(i);

	if (process == Data) continue;

	printf(" %15.1f ", systematicError[channel][process][syst]);
      }
    
    printf("\n");
  }

  printf("%10s ", "total");
  
  for (UInt_t i=0; i<vprocess.size(); i++)
    {
      UInt_t process = vprocess.at(i);

      if (process == Data) continue;

      printf(" %15.1f ", totalSyst[channel][process]);
    }
      
  printf("\n\n");
}


//------------------------------------------------------------------------------
// RelativeDifference
//------------------------------------------------------------------------------
Double_t RelativeDifference(Double_t x0, Double_t x1)
{
  if (x0 < 1) return 0;
  if (x1 < 1) return 0;

  return 1e2 * fabs(x1 - x0) / x0;
}
