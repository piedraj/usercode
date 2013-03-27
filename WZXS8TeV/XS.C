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
//  MCFM 6.3
//
//------------------------------------------------------------------------------
//
// scale systematic
//
//  xs = 13.89 + 8.06 = 21.95 pb          // Scale x1 ( 85.79 GeV) with 71 < mZ < 111 GeV
//  xs = 13.33 + 7.72 = 21.05 pb (-4.1%)  // Scale x2 (171.58 GeV) with 71 < mZ < 111 GeV
//  xs = 14.63 + 8.49.= 23.12 pb (+5.3%)  // Scale /2 ( 42.90 GeV) with 71 < mZ < 111 GeV
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
const Double_t xsWplusZ  = 13.89;  // pb (MCFM with 71 < mZ < 111 GeV)
const Double_t xsWminusZ =  8.06;  // pb (MCFM with 71 < mZ < 111 GeV)

const Double_t W2e         = 0.1075;
const Double_t W2m         = 0.1057;
const Double_t W2tau       = 0.1125;
const Double_t Z2ll        = 0.033658;
const Double_t WZ23lnu     = 3 * Z2ll * (W2e + W2m + W2tau);
const Double_t ngenWZ      = 2017979;
const Double_t ngenWZphase = 1449067;  // (71 < mZ < 111 GeV)


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


const UInt_t nCut = 6;

enum {
  Exactly3Leptons,
  HasZCandidate,
  HasWCandidate,
  MET30,
  MET40,
  MET40AntiBtag
};

TString sCut[nCut] = {
  "Exactly3Leptons",
  "HasZCandidate",
  "HasWCandidate",
  "MET30",
  "MET40",
  "MET40AntiBtag"
};


const UInt_t nProcess = 38;

enum {
  Data,
  DataPPF,
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
  WZTo2QLNuMad,
  ZZTo2L2QMad,
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

Color_t cProcess[nProcess];
TFile*  input   [nProcess];
TString sProcess[nProcess];


// Systematics
//------------------------------------------------------------------------------
const UInt_t nSystematic = 4;

enum {fakesSyst, scaleSyst, pdfSyst, metSyst};

TString sSystematic[nSystematic] = {"fakes", "scale", "pdf", "met"};

Double_t processSyst[nProcess];

Double_t systematicError[nProcess][nSystematic];


enum {linY, logY};

enum {MCmode, PPFmode, PPPmode};


// Settings
//------------------------------------------------------------------------------
Bool_t          _closure_test;
Double_t        _luminosity;
Double_t        _yoffset;
Int_t           _verbosity;
TString         _directory;
TString         _localpath;
TString         _datapath;
TString         _output;
UInt_t          _cut;
UInt_t          _mode;

vector<UInt_t>  vprocess;


// Save the cross section
//------------------------------------------------------------------------------
Double_t        xsValue    [nChannel+1];
Double_t        xsErrorStat[nChannel+1];
Double_t        xsErrorSyst[nChannel+1];
Double_t        xsErrorLumi[nChannel+1];


// Member functions
//------------------------------------------------------------------------------
void     SetParameters            (UInt_t        cut,
				   UInt_t        mode,
				   UInt_t        closure_test);

Int_t    ReadInputFiles           ();

void     MeasureTheCrossSection   (UInt_t        channel,
				   UInt_t        cut);

void     PrintLatexTable          (UInt_t        channel);

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

void     DrawCrossSections        (UInt_t        cut);

void     Inclusive                (UInt_t        cut);

void     RelativeSystematics      (UInt_t        cut);


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(UInt_t cut          = MET30,
	UInt_t mode         = PPFmode,
	UInt_t closure_test = 0)
{
  gROOT->SetBatch();

  SetParameters(cut, mode, closure_test);

  if (ReadInputFiles() < 0) return;

  RelativeSystematics(cut);

  for (UInt_t channel=0; channel<nChannel; channel++) {

    DrawHistogram("hSumCharges", channel, cut, "q_{1} + q_{2} + q_{3}");

    if (closure_test)
      {
	DrawHistogram("hInvMass2Lep",  channel, cut, "m_{#font[12]{ll}}",                    -1, 0, "GeV",  linY, 76, 106);
	DrawHistogram("hInvMass3Lep",  channel, cut, "m_{#font[12]{3l}}",                     2, 0, "GeV",  linY, 60, 200);
	DrawHistogram("hPtLeadingJet", channel, cut, "p_{T}^{leading jet}",                   2, 0, "GeV",  linY,  0, 125);
	DrawHistogram("hPtZLepton1",   channel, cut, "p_{T}^{Z leading lepton}",              2, 0, "GeV",  linY,  0, 125);
	DrawHistogram("hPtZLepton2",   channel, cut, "p_{T}^{Z trailing lepton}",             2, 0, "GeV",  linY,  0, 125);
	DrawHistogram("hMtW",          channel, cut, "m_{T}^{W}",                             2, 0, "GeV",  linY,  0,  50);
	DrawHistogram("hDPhiZLeptons", channel, cut, "#Delta#phi_{#font[12]{ll}}",           10, 1, "^{o}", linY);
	DrawHistogram("hDRWZLepton1",  channel, cut, "#DeltaR(W lepton, Z leading lepton)",   5, 1, "NULL", linY);
	DrawHistogram("hDRWZLepton2",  channel, cut, "#DeltaR(W lepton, Z trailing lepton)",  5, 1, "NULL", linY);
      }
    else
      {
	MeasureTheCrossSection(channel, cut);

	PrintLatexTable(channel);
	
	DrawHistogram("hMET",          channel, cut, "E_{T}^{miss}",          5, 0, "GeV", linY);
	DrawHistogram("hInvMass3Lep",  channel, cut, "m_{#font[12]{3l}}",     5, 0, "GeV", linY, 60, 350);
	DrawHistogram("hPtLepton1",    channel, cut, "p_{T}^{first lepton}",  5, 0, "GeV", linY);
	DrawHistogram("hPtLepton2",    channel, cut, "p_{T}^{second lepton}", 5, 0, "GeV", linY);
	DrawHistogram("hPtLepton3",    channel, cut, "p_{T}^{third lepton}",  5, 0, "GeV", linY);
	DrawHistogram("hPtLeadingJet", channel, cut, "p_{T}^{leading jet}",   5, 0, "GeV", linY);
	
	if (cut < HasZCandidate) continue;
	
	DrawHistogram("hInvMass2Lep",  channel, cut, "m_{#font[12]{ll}}",                    -1, 0, "GeV",  linY, 71, 111);
	DrawHistogram("hDPhiZLeptons", channel, cut, "#Delta#phi_{#font[12]{ll}}",           10, 1, "^{o}", linY);
	DrawHistogram("hPtZLepton1",   channel, cut, "p_{T}^{Z leading lepton}",              5, 0, "GeV",  linY);
	DrawHistogram("hPtZLepton2",   channel, cut, "p_{T}^{Z trailing lepton}",             5, 0, "GeV",  linY);
	DrawHistogram("hPtWLepton",    channel, cut, "p_{T}^{W lepton}",                      5, 0, "GeV",  linY);
	DrawHistogram("hDRWZLepton1",  channel, cut, "#DeltaR(W lepton, Z leading lepton)",   5, 1, "NULL", linY);
	DrawHistogram("hDRWZLepton2",  channel, cut, "#DeltaR(W lepton, Z trailing lepton)",  5, 1, "NULL", linY);
	DrawHistogram("hMtW",          channel, cut, "m_{T}^{W}",                             5, 0, "GeV",  linY);
      }
  }

  if (closure_test) return;

  Inclusive(cut);
  
  DrawCrossSections(cut);
}


//------------------------------------------------------------------------------
// MeasureTheCrossSection
//------------------------------------------------------------------------------
void MeasureTheCrossSection(UInt_t channel, UInt_t cut)
{
  if (cut < MET30) return;

  if (_closure_test) return;

  Double_t ndata   = 0;
  Double_t nsignal = 0;
  Double_t nbkg    = 0;
  Double_t ebkg    = 0;

  TString suffix = "_" + sChannel[channel] + "_" + sCut[cut] + "_LLL";

  Double_t nWZ = Yield((TH1D*)input[WZTo3LNu]->Get("hCounterEff" + suffix));

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    Double_t process_yield = Yield((TH1D*)input[j]->Get("hCounter" + suffix));

    if (j == Data)
      {
	ndata = process_yield;
      }
    else if (j == WZTo3LNu)
      {
	nsignal = process_yield;
      }
    else
      {
	nbkg += process_yield;

	Double_t ebkgStat = sqrt(process_yield);
	Double_t ebkgSyst = processSyst[j] * process_yield / 1e2;

	ebkg += (ebkgStat * ebkgStat);
	ebkg += (ebkgSyst * ebkgSyst);
      }
  }

  ebkg = sqrt(ebkg);


  // Estimate the cross section
  //----------------------------------------------------------------------------
  Double_t efficiency = nWZ / ngenWZphase;

  xsValue[channel] = (ndata - nbkg) / (_luminosity * efficiency * WZ23lnu);


  // Relative errors
  //----------------------------------------------------------------------------
  Double_t xsRelativeErrorLumi       = 4.4;
  Double_t xsRelativeErrorStat       = 1e2 * sqrt(ndata) / (ndata - nbkg);
  Double_t xsRelativeErrorBackground = 1e2 * ebkg        / (ndata - nbkg);
  Double_t xsRelativeErrorEfficiency = processSyst[WZTo3LNu];
  Double_t xsRelativeErrorSyst       = 0;

  xsRelativeErrorSyst += (xsRelativeErrorBackground * xsRelativeErrorBackground);
  xsRelativeErrorSyst += (xsRelativeErrorEfficiency * xsRelativeErrorEfficiency);
  xsRelativeErrorSyst = sqrt(xsRelativeErrorSyst);


  // Absolute errors
  //----------------------------------------------------------------------------
  xsErrorStat[channel] = xsRelativeErrorStat * xsValue[channel] / 1e2;  // pb
  xsErrorSyst[channel] = xsRelativeErrorSyst * xsValue[channel] / 1e2;  // pb
  xsErrorLumi[channel] = xsRelativeErrorLumi * xsValue[channel] / 1e2;  // pb


  // Print the results
  //----------------------------------------------------------------------------
  if (_verbosity > 0)
    {
      printf("\n");
      printf(" WZ efficiency = %.2f%s\n", 1e2 * efficiency, "%");
      printf("          nbkg = %5.1f +- %4.1f\n", nbkg,       sqrt(nbkg));
      printf("         ndata = %5.1f +- %4.1f\n", ndata,      sqrt(ndata));
      printf("  ndata - nbkg = %5.1f +- %4.1f\n", ndata-nbkg, sqrt(ndata+nbkg));
      printf("       nsignal = %5.1f +- %4.1f\n", nsignal,    sqrt(nsignal));
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
// PrintLatexTable
//------------------------------------------------------------------------------
void PrintLatexTable(UInt_t channel)
{
  gSystem->mkdir("tex", kTRUE);

  ofstream outputfile;

  TString suffix = (_mode == MCmode) ? "mc" : "ppf";

  outputfile.open(Form("tex/%s_%s.tex", sChannel[channel].Data(), suffix.Data()));

  Double_t nData [nCut];
  Double_t nWZ   [nCut];
  Double_t nZZ   [nCut];
  Double_t nWV   [nCut];
  Double_t nVVV  [nCut];
  Double_t nTop  [nCut];
  Double_t nZJets[nCut];
  Double_t nFakes[nCut];
  Double_t nBkg  [nCut];

  Double_t eData [nCut];
  Double_t eWZ   [nCut];
  Double_t eZZ   [nCut];
  Double_t eWV   [nCut];
  Double_t eVVV  [nCut];
  Double_t eTop  [nCut];
  Double_t eZJets[nCut];
  Double_t eFakes[nCut];
  Double_t eBkg  [nCut];

  for (UInt_t i=0; i<nCut; i++)
    {
      nData [i] = 0.;
      nWZ   [i] = 0.;
      nZZ   [i] = 0.;
      nWV   [i] = 0.;
      nVVV  [i] = 0.;
      nTop  [i] = 0.;
      nZJets[i] = 0.;
      nFakes[i] = 0.;
      nBkg  [i] = 0.;

      eData [i] = 0.;
      eWZ   [i] = 0.;
      eZZ   [i] = 0.;
      eWV   [i] = 0.;
      eVVV  [i] = 0.;
      eTop  [i] = 0.;
      eZJets[i] = 0.;
      eFakes[i] = 0.;
      eBkg  [i] = 0.;
  
      TH1D* hist[nProcess];

      for (UInt_t j=0; j<vprocess.size(); j++)
	{
	  UInt_t k = vprocess.at(j);

	  TString hname = "hCounter_" + sChannel[channel] + "_" + sCut[i] + "_LLL";

	  hist[k] = (TH1D*)input[k]->Get(hname);

	  hist[k]->SetName(hname + "_" + sProcess[k]);
	}

      nData[i] = Yield(hist[Data]);
      eData[i] = sqrt(nData[i]);

      nWZ[i] = Yield(hist[WZTo3LNu]);
      eWZ[i] = sqrt(nWZ[i]);

      nZZ[i] += Yield(hist[ZgammaToLLG]);
      nZZ[i] += Yield(hist[ZZTo2L2QMad]);
      nZZ[i] += Yield(hist[ggZZ2L2L]);
      nZZ[i] += Yield(hist[ggZZ4L]);
      nZZ[i] += Yield(hist[ZZ2Mu2Tau]);
      nZZ[i] += Yield(hist[ZZ4E]);
      nZZ[i] += Yield(hist[ZZ2E2Tau]);
      nZZ[i] += Yield(hist[ZZ4Mu]);
      nZZ[i] += Yield(hist[ZZ2E2Mu]);
      nZZ[i] += Yield(hist[ZZ4Tau]);
      nZZ[i] += Yield(hist[HZZ4L]);
      eZZ[i]  = sqrt(nZZ[i]);
    
      nWV[i] += Yield(hist[WW]);
      nWV[i] += Yield(hist[WZTo2L2QMad]);
      nWV[i] += Yield(hist[WZTo2QLNuMad]);
      nWV[i] += Yield(hist[WbbToLNu]);
      nWV[i] += Yield(hist[WJets_Madgraph]);
      nWV[i] += Yield(hist[WGstarToElNuMad]);
      nWV[i] += Yield(hist[WGstarToMuNuMad]);
      nWV[i] += Yield(hist[WGstarToTauNuMad]);
      nWV[i] += Yield(hist[WgammaToLNuG]);
      eWV[i]  = sqrt(nWV[i]);
    
      nVVV[i] += Yield(hist[WWGJets]);
      nVVV[i] += Yield(hist[WZZJets]);
      nVVV[i] += Yield(hist[ZZZJets]);
      nVVV[i] += Yield(hist[WWZJets]);
      nVVV[i] += Yield(hist[WWWJets]);
      nVVV[i] += Yield(hist[TTWJets]);
      nVVV[i] += Yield(hist[TTZJets]);
      nVVV[i] += Yield(hist[TTWWJets]);
      nVVV[i] += Yield(hist[TTGJets]);
      eVVV[i]  = sqrt(nVVV[i]);

      if (_mode == MCmode)
	{
	  nTop[i] += Yield(hist[TTbar_Madgraph]);
	  nTop[i] += Yield(hist[TW]);
	  nTop[i] += Yield(hist[TbarW]);
	  eTop[i]  = sqrt(nTop[i]);

	  nZJets[i] += Yield(hist[DYJets_Madgraph]);
	  nZJets[i] += Yield(hist[ZJets_Madgraph]);
	  nZJets[i] += Yield(hist[ZbbToLL]);
	  eZJets[i]  = sqrt(nZJets[i]);

	  nBkg[i] = nZZ[i] + nWV[i] + nVVV[i] + nTop[i] + nZJets[i];
	  eBkg[i] = sqrt(nBkg[i]);
	}
      else if (_mode == PPFmode)
	{
	  nFakes[i] = Yield(hist[DataPPF]);
	  eFakes[i] = sqrt(nFakes[i]);

	  nBkg[i] = nZZ[i] + nWV[i] + nVVV[i] + nFakes[i];
	  eBkg[i] = sqrt(nBkg[i]);
	}
    }

  
  // Print
  //----------------------------------------------------------------------------
  if (_mode == MCmode)  {outputfile << Form(" %-20s", "top");         for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nTop[i],   eTop[i]);   outputfile << "\\\\\n";}
  if (_mode == MCmode)  {outputfile << Form(" %-20s", "Z+jets");      for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZJets[i], eZJets[i]); outputfile << "\\\\\n";}
  if (_mode == PPFmode) {outputfile << Form(" %-20s", "data-driven"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nFakes[i], eFakes[i]); outputfile << "\\\\\n";}

  outputfile << Form(" %-20s", "ZZ");          for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZZ[i],  eZZ[i]);  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "WV");          for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nWV[i],  eWV[i]);  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "VVV");         for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nVVV[i], eVVV[i]); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "backgrounds"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nBkg[i], eBkg[i]); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "WZ");          for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nWZ[i],  eWZ[i]);  outputfile << "\\\\\n";

  outputfile << " \\hline\n";

  outputfile << Form(" %-20s", "WZ + backgrounds"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nBkg[i]+nWZ[i], sqrt(nBkg[i]+nWZ[i])); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "data");             for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nData[i],       eData[i]);             outputfile << "\\\\\n";

  //  outputfile << Form(" %-20s", "S/$\\sqrt{{\\rm S} + {\\rm B}}$");

  //  for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %.1f", nWZ[i] / (sqrt(nWZ[i] + nBkg[i]))); outputfile << "\\\\\n";

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

      Double_t binContent   = hist[j]->GetBinContent(ibin);
      Double_t binStatError = hist[j]->GetBinError(ibin);
      Double_t binSystError = processSyst[j] * binContent / 1e2;

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
    theMax *= 1.5;

  hist[Data]->SetMaximum(theMax);

  if (ymin != -999) hist[Data]->SetMinimum(ymin);
  if (ymax != -999) hist[Data]->SetMaximum(ymax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.720;
  Double_t y0     = 0.834;
  Double_t delta  = _yoffset + 0.001;
  Double_t ndelta = 0;
  
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[Data],     Form(" data (%.0f)", Yield(hist[Data])),     "lp"); ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, allmc,          Form(" all (%.0f)",  Yield(allmc)),          "f");  ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, hist[WZTo3LNu], Form(" WZ (%.0f)",   Yield(hist[WZTo3LNu])), "f");  ndelta += delta;

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

  Double_t ZZYield  = 0.0;
  Double_t WVYield  = 0.0;
  Double_t VVVYield = 0.0;

  ZZYield += Yield(hist[ZgammaToLLG]);
  ZZYield += Yield(hist[ZZTo2L2QMad]);
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
  WVYield += Yield(hist[WZTo2QLNuMad]);
  WVYield += Yield(hist[WbbToLNu]);
  WVYield += Yield(hist[WJets_Madgraph]);
  WVYield += Yield(hist[WGstarToElNuMad]);
  WVYield += Yield(hist[WGstarToMuNuMad]);
  WVYield += Yield(hist[WGstarToTauNuMad]);
  WVYield += Yield(hist[WgammaToLNuG]);

  VVVYield += Yield(hist[WWGJets]);
  VVVYield += Yield(hist[WZZJets]);
  VVVYield += Yield(hist[ZZZJets]);
  VVVYield += Yield(hist[WWZJets]);
  VVVYield += Yield(hist[WWWJets]);
  VVVYield += Yield(hist[TTWJets]);
  VVVYield += Yield(hist[TTZJets]);
  VVVYield += Yield(hist[TTWWJets]);
  VVVYield += Yield(hist[TTGJets]);
      
  ndelta = 0;
  
  DrawLegend(x0 - 0.23, y0 - ndelta, hist[ggZZ2L2L], Form(" ZZ (%.0f)",  ZZYield),  "f"); ndelta += delta;
  DrawLegend(x0 - 0.23, y0 - ndelta, hist[WW],       Form(" WV (%.0f)",  WVYield),  "f"); ndelta += delta;
  DrawLegend(x0 - 0.23, y0 - ndelta, hist[WWGJets],  Form(" VVV (%.0f)", VVVYield), "f"); ndelta += delta;


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
		   UInt_t mode,
		   UInt_t closure_test)
{
  sProcess[Data]             = "Data";
  sProcess[DataPPF]          = "Data_PPF";
  sProcess[DYJets_Madgraph]  = "DYJets_Madgraph";
  sProcess[ZJets_Madgraph]   = "ZJets_Madgraph";
  sProcess[ZbbToLL]          = "ZbbToLL";
  sProcess[WbbToLNu]         = "WbbToLNu";
  sProcess[WJets_Madgraph]   = "WJets_Madgraph";
  sProcess[WGstarToElNuMad]  = "WGstarToElNuMad";
  sProcess[WGstarToMuNuMad]  = "WGstarToMuNuMad";
  sProcess[WGstarToTauNuMad] = "WGstarToTauNuMad";
  sProcess[WgammaToLNuG]     = "WgammaToLNuG";
  sProcess[ZgammaToLLG]      = "ZgammaToLLG";
  sProcess[TTbar_Madgraph]   = "TTbar_Madgraph";
  sProcess[TW]               = "TW";
  sProcess[TbarW]            = "TbarW";
  sProcess[WW]               = "WW";
  sProcess[WZTo3LNu]         = "WZTo3LNu";
  sProcess[WZTo2L2QMad]      = "WZTo2L2QMad";
  sProcess[WZTo2QLNuMad]     = "WZTo2QLNuMad";
  sProcess[ZZTo2L2QMad]      = "ZZTo2L2QMad";
  sProcess[ggZZ2L2L]         = "ggZZ2L2L";
  sProcess[ggZZ4L]           = "ggZZ4L";
  sProcess[ZZ2Mu2Tau]        = "ZZ2Mu2Tau";
  sProcess[ZZ4E]             = "ZZ4E";
  sProcess[ZZ2E2Tau]         = "ZZ2E2Tau";
  sProcess[ZZ4Mu]            = "ZZ4Mu";
  sProcess[ZZ2E2Mu]          = "ZZ2E2Mu";
  sProcess[ZZ4Tau]           = "ZZ4Tau";
  sProcess[HZZ4L]            = "HZZ4L";
  sProcess[WWGJets]          = "WWGJets";
  sProcess[WZZJets]          = "WZZJets";
  sProcess[ZZZJets]          = "ZZZJets";
  sProcess[WWZJets]          = "WWZJets";
  sProcess[WWWJets]          = "WWWJets";
  sProcess[TTWJets]          = "TTWJets";
  sProcess[TTZJets]          = "TTZJets";
  sProcess[TTWWJets]         = "TTWWJets";
  sProcess[TTGJets]          = "TTGJets";

  cProcess[Data]             = kBlack;
  cProcess[DataPPF]          = kGray+1;
  cProcess[DYJets_Madgraph]  = kGreen+2;   // Z+jets
  cProcess[ZJets_Madgraph]   = kGreen+2;   // Z+jets
  cProcess[ZbbToLL]          = kGreen+2;   // Z+jets
  cProcess[WbbToLNu]         = kAzure;     // WV
  cProcess[WJets_Madgraph]   = kAzure;     // WV
  cProcess[WGstarToElNuMad]  = kAzure;     // WV
  cProcess[WGstarToMuNuMad]  = kAzure;     // WV
  cProcess[WGstarToTauNuMad] = kAzure;     // WV
  cProcess[WgammaToLNuG]     = kAzure;     // WV
  cProcess[WW]               = kAzure;     // WV
  cProcess[WZTo2L2QMad]      = kAzure;     // WV
  cProcess[WZTo2QLNuMad]     = kAzure;     // WV
  cProcess[TTbar_Madgraph]   = kAzure-9;   // top
  cProcess[TW]               = kAzure-9;   // top
  cProcess[TbarW]            = kAzure-9;   // top
  cProcess[WZTo3LNu]         = kOrange-2;  // WZ
  cProcess[ZgammaToLLG]      = kRed+1;     // ZZ
  cProcess[ZZTo2L2QMad]      = kRed+1;     // ZZ
  cProcess[ggZZ2L2L]         = kRed+1;     // ZZ
  cProcess[ggZZ4L]           = kRed+1;     // ZZ
  cProcess[ZZ2Mu2Tau]        = kRed+1;     // ZZ
  cProcess[ZZ4E]             = kRed+1;     // ZZ
  cProcess[ZZ2E2Tau]         = kRed+1;     // ZZ
  cProcess[ZZ4Mu]            = kRed+1;     // ZZ
  cProcess[ZZ2E2Mu]          = kRed+1;     // ZZ
  cProcess[ZZ4Tau]           = kRed+1;     // ZZ
  cProcess[HZZ4L]            = kRed+1;     // ZZ
  cProcess[WWGJets]          = kBlack;     // VVV
  cProcess[WZZJets]          = kBlack;     // VVV
  cProcess[ZZZJets]          = kBlack;     // VVV
  cProcess[WWZJets]          = kBlack;     // VVV
  cProcess[WWWJets]          = kBlack;     // VVV
  cProcess[TTWJets]          = kBlack;     // VVV
  cProcess[TTZJets]          = kBlack;     // VVV
  cProcess[TTWWJets]         = kBlack;     // VVV
  cProcess[TTGJets]          = kBlack;     // VVV


  _luminosity   = 19602.0;
  _yoffset      = 0.048;
  _verbosity    = 2;
  _cut          = cut;
  _mode         = mode;
  _closure_test = closure_test;
  _localpath    = GuessLocalBasePath();

  _directory = (_closure_test) ? "closure_test" : "analysis";
  
  _datapath = Form("%s/piedra/work/WZXS8TeV/results/Summer12_53X/WH/",
		   _localpath.Data());

  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetEndErrorSize    (  5);
  gStyle->SetHatchesLineWidth(  1);
  gStyle->SetHatchesSpacing  (0.7);
  
  TH1::SetDefaultSumw2();


  vprocess.clear();

  vprocess.push_back(Data);
  vprocess.push_back(WbbToLNu);
  vprocess.push_back(WJets_Madgraph);
  vprocess.push_back(WGstarToElNuMad);
  vprocess.push_back(WGstarToMuNuMad);
  vprocess.push_back(WGstarToTauNuMad);
  vprocess.push_back(WgammaToLNuG);
  vprocess.push_back(WW);
  vprocess.push_back(WZTo2L2QMad);
  vprocess.push_back(WZTo2QLNuMad);

  if (mode == MCmode)
    {
      vprocess.push_back(TTbar_Madgraph);
      vprocess.push_back(TW);
      vprocess.push_back(TbarW);
      vprocess.push_back(DYJets_Madgraph);
      vprocess.push_back(ZJets_Madgraph);
      vprocess.push_back(ZbbToLL);
    }
  else if (mode == PPFmode)
    {
      vprocess.push_back(DataPPF);
    }
      
  vprocess.push_back(ZgammaToLLG);
  vprocess.push_back(ZZTo2L2QMad);
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


//------------------------------------------------------------------------------
// ReadInputFiles
//------------------------------------------------------------------------------
Int_t ReadInputFiles()
{
  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    input[j] = new TFile(_datapath + "/" + _directory + "/" + sProcess[j] + ".root");

    TH1D* dummy = (TH1D*)input[j]->Get("hCounter_MME_MET30_TTT");

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

  _output = _directory;

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
  if (cut < MET30) return;

  TGraphErrors* gStat = new TGraphErrors(nChannel+1);
  TGraphErrors* gSyst = new TGraphErrors(nChannel+1);
  TGraphErrors* gLumi = new TGraphErrors(nChannel+1);

  for (UInt_t i=0; i<nChannel+1; i++) {

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
  Double_t xmax = (xsWplusZ + xsWminusZ) + 25.;
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

    DrawTLatex(xmax-1., y, 0.035, 32, Form("%.2f #pm %.2f #pm %.2f #pm %.2f",
					    x, gStatError, gSystError, gLumiError), 0);
  }

  DrawTLatex(0.940, 0.983, 0.05, 33,
	     Form("#sqrt{s} = 8 TeV, L = %.1f fb^{-1}", _luminosity/1e3));

  dummy->GetXaxis()->CenterTitle();
  dummy->GetXaxis()->SetTitleOffset(1.4);
  dummy->GetXaxis()->SetTitle("#sigma_{WZ} [pb]");
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
// Inclusive
//------------------------------------------------------------------------------
void Inclusive(UInt_t cut)
{
  if (cut < MET30) return;

  if (_closure_test) return;

  Double_t x     = 0;
  Double_t stat  = 0;
  Double_t syst  = 0;
  Double_t lumi  = 0;
  Double_t total = 0;

  for (UInt_t i=0; i<nChannel; i++) {

    Double_t xsErrorTotal = 0;

    xsErrorTotal += (xsErrorStat[i] * xsErrorStat[i]);
    xsErrorTotal += (xsErrorSyst[i] * xsErrorSyst[i]);
    xsErrorTotal += (xsErrorLumi[i] * xsErrorLumi[i]);

    xsErrorTotal = sqrt(xsErrorTotal);

    x += (xsValue[i] / xsErrorTotal / xsErrorTotal);

    stat  += (1. / xsErrorStat[i]  / xsErrorStat[i]);
    syst  += (1. / xsErrorSyst[i]  / xsErrorSyst[i]);
    lumi  += (1. / xsErrorLumi[i]  / xsErrorLumi[i]);
    total += (1. / xsErrorTotal    / xsErrorTotal);
  }

  xsValue[nChannel] = x / total;

  xsErrorStat[nChannel] = 1. / sqrt(stat);
  xsErrorSyst[nChannel] = 1. / sqrt(syst);
  xsErrorLumi[nChannel] = 1. / sqrt(lumi);
}


//------------------------------------------------------------------------------
// RelativeSystematics
//------------------------------------------------------------------------------
void RelativeSystematics(UInt_t cut)
{
  if (_closure_test) return;

  for (UInt_t k=0; k<nProcess; k++) processSyst[k] = 0.0;

  for (UInt_t i=0; i<nSystematic; i++) {

    Double_t maxSyst = 0.0;

    for (UInt_t k=0; k<nProcess; k++) systematicError[k][i] = 0.0;

    if (i == fakesSyst) {systematicError[DataPPF] [i] = 36.0; continue;}
    if (i == scaleSyst) {systematicError[WZTo3LNu][i] =  5.3; continue;}
    if (i == pdfSyst)   {systematicError[WZTo3LNu][i] =  3.0; continue;}

    for (UInt_t j=0; j<nChannel; j++) {

      TString hname = "hCounter_" + sChannel[j] + "_" + sCut[cut] + "_TTT";

      for(UInt_t k=0; k<nProcess; k++) {
	
	if (k == Data)    continue;
	if (k == DataPPF) continue;
	
	TFile* f0 = new TFile(_datapath + "/" + _directory + "/" + sProcess[k] + ".root");
	TFile* f1 = new TFile(_datapath + "/systematics/" + sSystematic[i] + "/" + sProcess[k] + ".root");

	Double_t y0 = Yield((TH1D*)f0->Get(hname));
	Double_t y1 = Yield((TH1D*)f1->Get(hname));

	f0->Close();
	f1->Close();

	Double_t syst = (y0 > 1) ? 1e2 * fabs(y1 - y0) / y0 : 0.0;

	if (syst > systematicError[k][i]) systematicError[k][i] = syst;

	if (systematicError[k][i] > maxSyst) maxSyst = systematicError[k][i];
      }
    }

    for (UInt_t k=0; k<nProcess; k++) {

      if (k == Data || k == DataPPF) continue;
      
      systematicError[k][i] = maxSyst;
    }
  }


  processSyst[DataPPF] = systematicError[DataPPF][fakesSyst];
  

  for (UInt_t i=0; i<nProcess; i++) {

    if (i == Data || i == DataPPF) continue;

    for (UInt_t j=0; j<nSystematic; j++) {

      if (j == fakesSyst) continue;
      
      Double_t syst2 = systematicError[i][j] * systematicError[i][j];

      if (j == scaleSyst) {if (i == WZTo3LNu) processSyst[i] += syst2; continue;}
      if (j == pdfSyst)   {if (i == WZTo3LNu) processSyst[i] += syst2; continue;}

      processSyst[i] += syst2;
    }

    processSyst[i] = sqrt(processSyst[i]);
  }


  // Print
  //----------------------------------------------------------------------------
  if (_verbosity < 2) return;

  printf("\n%10s ", " ");

  for (UInt_t i=0; i<nProcess; i++)
    {
      if (i != DataPPF && i != WZTo3LNu && i != ZJets_Madgraph && i != TW) continue;
      
      (i == DataPPF) ? printf(" %15s ", "PPF") : printf(" %15s ", sProcess[i].Data());
    }
  
  printf("\n");
  
  for (UInt_t j=0; j<nSystematic; j++) {
    
    printf("%10s ", sSystematic[j].Data());
    
    for (UInt_t i=0; i<nProcess; i++)
      {
	if (i != DataPPF && i != WZTo3LNu && i != ZJets_Madgraph && i != TW) continue;
	
	printf(" %15.1f ", systematicError[i][j]);
      }
    
    printf("\n");
  }

  printf("%10s ", "total");
  
  for (UInt_t i=0; i<nProcess; i++)
    {
      if (i != DataPPF && i != WZTo3LNu && i != ZJets_Madgraph && i != TW) continue;
      
      printf(" %15.1f ", processSyst[i]);
    }
      
  printf("\n\n");
}
