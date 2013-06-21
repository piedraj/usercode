#include "DrawFunctions.h"

#include <fstream>
#include <iostream>
#include <vector>


#include "TMatrixD.h"


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
const Double_t xsWPlusZ  = 13.89;  // pb (MCFM with 71 < mZ < 111 GeV)
const Double_t xsWMinusZ =  8.06;  // pb (MCFM with 71 < mZ < 111 GeV)

const Double_t ngenWPlusZ  = 906262;  // (71 < mZ < 111 GeV)
const Double_t ngenWMinusZ = 542805;  // (71 < mZ < 111 GeV)

const Double_t W2e     = 0.1075;
const Double_t W2m     = 0.1057;
const Double_t W2tau   = 0.1125;
const Double_t Z2ll    = 0.033658;
const Double_t WZ23lnu = 3 * Z2ll * (W2e + W2m + W2tau);


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


const UInt_t nCut = 4;

enum {
  Exactly3LeptonsM3L,
  HasZ,
  //  PreMET30,
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


const UInt_t nCharge = 3;

enum {
  WInclusive,
  WPlus,
  WMinus
};

const TString sCharge[nCharge] = {
  "WInclusive",
  "WPlus",
  "WMinus"
};

const Double_t ngenWZ[nCharge] = {
  ngenWPlusZ+ngenWMinusZ,
  ngenWPlusZ,
  ngenWMinusZ
};  

const Double_t xs_nlo[nCharge] = {
  xsWPlusZ+xsWMinusZ,
  xsWPlusZ,
  xsWMinusZ
};


// Systematics
//------------------------------------------------------------------------------
const UInt_t nSystematic = 10;

enum {
  fakesSyst,
  qcdSyst,
  pdfSyst,
  metSyst,
  triggerSyst,
  muonSyst,
  electronSyst,
  pileupSyst,
  zzSyst,
  zgSyst
};

TString sSystematic[nSystematic] = {
  "fakes",
  "qcd",
  "pdf",
  "met",
  "trigger",
  "muon",
  "electron",
  "pileup",
  "zz",
  "zg"
};

Double_t totalSyst[nChannel][nProcess];

Double_t systematicError[nChannel][nProcess][nSystematic];


enum {linY, logY};

enum {MCmode, PPFmode, PPPmode};


// Settings
//------------------------------------------------------------------------------
Double_t        _luminosity;
Double_t        _luminosityUncertainty;
Int_t           _verbosity;
TString         _localpath;
TString         _datapath;
TString         _output;
UInt_t          _cut;
UInt_t          _mode;
UInt_t          _wcharge;

vector<UInt_t>   vprocess;


// Save the cross section
//------------------------------------------------------------------------------
Double_t         wzEffValue  [nChannel+1];
Double_t         wzEffError  [nChannel+1];
Double_t         xsValue     [nChannel+1];
Double_t         xsErrorLumi [nChannel+1];
Double_t         xsErrorStat [nChannel+1];
Double_t         xsErrorSyst [nChannel+1];
Double_t         xsSystematic[nChannel][nSystematic+1][nCut];


// Save the ratio
//------------------------------------------------------------------------------
Double_t         ratioValue     [nChannel+1];
Double_t         ratioErrorStat [nChannel+1];
Double_t         ratioErrorSyst [nChannel+1];
Double_t         ratioSystematic[nChannel][nSystematic+1][nCut];


// Member functions
//------------------------------------------------------------------------------
void     SetParameters            (UInt_t        cut,
				   UInt_t        mode,
				   UInt_t        wcharge);

Int_t    ReadInputFiles           ();

void     CrossSection             (Double_t&     xsVal,
				   Double_t&     xsErr,
				   Double_t&     wzEff,
				   UInt_t        wcharge,
				   UInt_t        channel,
				   UInt_t        cut,
				   Int_t         syst = -1);

void     CrossSectionRatio        (UInt_t        wchargeNum,
				   UInt_t        wchargeDen,
				   UInt_t        channel,
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

Double_t Yield                    (TH1*          h);

TString  GuessLocalBasePath       ();

void     MakeOutputDirectory      (TString       format);

void     BlueMethod               (UInt_t        cut,
				   Bool_t        xsratio = false);

void     RelativeSystematics      (UInt_t        channel,
				   UInt_t        cut);

void     DrawCrossSections        (UInt_t        cut);

void     PrintCrossSections       (UInt_t        cut);

void     PrintRatios              (UInt_t        cut,
				   UInt_t        wchargeNum,
				   UInt_t        wchargeDen);

Double_t RelativeDifference       (Double_t      x0,
				   Double_t      x1);

Double_t Ratio                    (Double_t      a,
				   Double_t      b);

Double_t RatioError               (Double_t      a,
				   Double_t      b,
				   Double_t      aErr,
				   Double_t      bErr);


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(UInt_t cut     = MET30,
	UInt_t mode    = PPFmode,
	UInt_t wcharge = WInclusive,
	Bool_t draw    = false)
{
  SetParameters(cut, mode, wcharge);

  if (ReadInputFiles() < 0) return;

  wzEffValue[nChannel] = 0.0;
  wzEffError[nChannel] = 0.0;

  for (UInt_t channel=0; channel<nChannel; channel++) {

    RelativeSystematics(channel, cut);


    // Measure the cross section
    //--------------------------------------------------------------------------
    CrossSection(xsValue[channel],
		 xsErrorStat[channel],
		 wzEffValue[channel],
		 _wcharge,
		 channel,
		 cut);

    for (UInt_t syst=0; syst<nSystematic; syst++)
      {
	Double_t xsVal, xsErr, wzEff;

	CrossSection(xsVal,
		     xsErr,
		     wzEff,
		     _wcharge,
		     channel,
		     cut,
		     syst); 

	xsSystematic[channel][syst][cut] = fabs(xsVal - xsValue[channel]);
      }

    xsErrorSyst[channel] = 0;

    for (UInt_t k=0; k<=nSystematic; k++)
      xsErrorSyst[channel] += (xsSystematic[channel][k][cut] * xsSystematic[channel][k][cut]);

    xsErrorSyst[channel] = sqrt(xsErrorSyst[channel]);

    xsErrorLumi[channel] = _luminosityUncertainty * xsValue[channel] / 1e2;

    wzEffError[channel] = totalSyst[channel][WZ] * wzEffValue[channel] / 1e2;

    wzEffValue[nChannel] += wzEffValue[channel];

    wzEffError[nChannel] += (wzEffError[channel] * wzEffError[channel]);

    
    // Print table with yields and draw histograms
    //--------------------------------------------------------------------------
    PrintYields(channel);

    if (draw)
      {
	DrawHistogram("hSumCharges",   channel, cut, "q_{1} + q_{2} + q_{3}");
	DrawHistogram("hMET",          channel, cut, "E_{T}^{miss}",                           5, 0, "GeV",  linY);
	DrawHistogram("hInvMass2Lep",  channel, cut, "m_{#font[12]{ll}}",                     -1, 0, "GeV",  linY, 70, 112);
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
  }

  wzEffError[nChannel] = sqrt(wzEffError[nChannel]);


  // Final numbers
  //----------------------------------------------------------------------------
  BlueMethod(cut);
  
  PrintSystematics(cut);

  if (draw) DrawCrossSections(cut);

  PrintCrossSections(cut);


  // Cross section ratio
  //----------------------------------------------------------------------------
  if (wcharge == WInclusive) {
    

    // W- / W+
    //--------------------------------------------------------------------------
    for (UInt_t channel=0; channel<nChannel; channel++)
      CrossSectionRatio(WMinus, WPlus, channel, cut);

    BlueMethod(cut, true);

    PrintRatios(cut, WMinus, WPlus);


    // W+ / W-
    //--------------------------------------------------------------------------
    for (UInt_t channel=0; channel<nChannel; channel++)
      CrossSectionRatio(WPlus, WMinus, channel, cut);

    BlueMethod(cut, true);

    PrintRatios(cut, WPlus, WMinus);
  }
}


//------------------------------------------------------------------------------
// CrossSection
//------------------------------------------------------------------------------
void CrossSection(Double_t& xsVal,
		  Double_t& xsErr,
		  Double_t& wzEff,
		  UInt_t    wcharge,
		  UInt_t    channel,
		  UInt_t    cut,
		  Int_t     syst)
{
  Double_t ndata = 0;
  Double_t nbkg  = 0;
  Double_t ebkg  = 0;
  Double_t nWZ   = 0;

  TString suffix = "_" + sChannel[channel] + "_" + sCut[cut] + "_" + sCharge[wcharge] + "_LLL";

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TString prefix = (j == WZ) ? "hCounterEff" : "hCounter";

    Double_t process_yield = Yield((TH1D*)input[j]->Get(prefix + suffix));

    Double_t eStat = sqrt(process_yield);

    if (j != Data && j != WZ) ebkg += (eStat * eStat);

    if (j == Data)
      {
	ndata = process_yield;
      }
    else if (j == Fakes)
      {
	if (syst == fakesSyst) process_yield *= (1. + 0.36);

	nbkg += process_yield;
      }
    else
      {
	if (syst == muonSyst || syst == electronSyst || syst == metSyst)
	  {
	    TFile* fUp   = new TFile(_datapath + "/systematics/" + sSystematic[syst] + "Up/"   + sProcess[j] + ".root");
	    TFile* fDown = new TFile(_datapath + "/systematics/" + sSystematic[syst] + "Down/" + sProcess[j] + ".root");
	    
	    Double_t yUp   = Yield((TH1D*)fUp  ->Get(prefix + suffix));
	    Double_t yDown = Yield((TH1D*)fDown->Get(prefix + suffix));
	    
	    Double_t systUp   = RelativeDifference(process_yield, yUp);
	    Double_t systDown = RelativeDifference(process_yield, yDown);
	    
	    process_yield = (systUp > systDown) ? yUp : yDown;
	    
	    fUp  ->Close();
	    fDown->Close();
	  }
	else if (syst == pileupSyst)
	  {
	    TFile* f1 = new TFile(_datapath + "/systematics/" + sSystematic[syst] + "/" + sProcess[j] + ".root");
	    
	    process_yield = Yield((TH1D*)f1->Get(prefix + suffix));
	    
	    f1->Close();
	  }
	else if (syst == triggerSyst)
	  {
	    process_yield *= (1. + sqrt(3.) / 1e2);
	  }

	if (j == WZ)
	  {
	    nWZ = process_yield;

	    if (syst == qcdSyst) nWZ *= (1. + 0.016);
	    if (syst == pdfSyst) nWZ *= (1. + 0.007);
	  }
	else
	  {
	    if (syst == zzSyst && j == ZZ) process_yield *= (1. + 0.15);
	    if (syst == zgSyst && j == ZG) process_yield *= (1. + 0.15);
	    
	    nbkg += process_yield;
	  }
      }
  }

  wzEff = nWZ / ngenWZ[wcharge];

  xsVal = (ndata - nbkg) / (_luminosity * wzEff * WZ23lnu);

  xsErr = sqrt(ndata) / (_luminosity * wzEff * WZ23lnu);


  if (wcharge == WInclusive)
    {
      ebkg = sqrt(ebkg);
      
      xsSystematic[channel][nSystematic][cut] = ebkg / (_luminosity * wzEff * WZ23lnu);  // DIRTY
    }
}


//------------------------------------------------------------------------------
// CrossSectionRatio
//------------------------------------------------------------------------------
void CrossSectionRatio(UInt_t wchargeNum,
		       UInt_t wchargeDen,
		       UInt_t channel,
		       UInt_t cut)
{
  Double_t xsValNum, xsErrNum, wzEffNum;
  Double_t xsValDen, xsErrDen, wzEffDen;

  CrossSection(xsValNum, xsErrNum, wzEffNum, wchargeNum, channel, cut);
  CrossSection(xsValDen, xsErrDen, wzEffDen, wchargeDen, channel, cut);

  ratioValue    [channel] = Ratio     (xsValNum, xsValDen);
  ratioErrorStat[channel] = RatioError(xsValNum, xsValDen, xsErrNum, xsErrDen);
  ratioErrorSyst[channel] = 0.0;

  for (UInt_t syst=0; syst<=nSystematic; syst++)
    {
      CrossSection(xsValNum, xsErrNum, wzEffNum, wchargeNum, channel, cut, syst);
      CrossSection(xsValDen, xsErrDen, wzEffDen, wchargeDen, channel, cut, syst);

      ratioSystematic[channel][syst][cut] = fabs(Ratio(xsValNum, xsValDen) - ratioValue[channel]);

      ratioErrorSyst[channel] += (ratioSystematic[channel][syst][cut] * ratioSystematic[channel][syst][cut]);
    }

  ratioErrorSyst[channel] = sqrt(ratioErrorSyst[channel]);

  if (_verbosity > 1)
    {
      printf(" [%s] xs(%s)/xs(%s) = %.4f +- %.4f (stat.) +- %.4f (syst.) \t 1/ratio = %.4f\n",
	     sChannel[channel].Data(),
	     sCharge[wchargeNum].Data(),
	     sCharge[wchargeDen].Data(),
	     ratioValue    [channel],
	     ratioErrorStat[channel],
	     ratioErrorSyst[channel],
	     1. / ratioValue[channel]);
    }
}


//------------------------------------------------------------------------------
// PrintYields
//------------------------------------------------------------------------------
void PrintYields(UInt_t channel)
{
  ofstream outputfile;

  TString suffix = (_mode == MCmode) ? "mc" : "ppf";

  outputfile.open(Form("tex/%s_%s_%s.tex",
		       sChannel[channel].Data(),
		       sCharge[_wcharge].Data(),
		       suffix.Data()));

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

	  TString hname = "hCounter_" + sChannel[channel] + "_" + sCut[i] + "_" + sCharge[_wcharge] + "_LLL";

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

  outputfile << Form(" %-20s", "ZZ");          for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZZ[i],  sqrt(nZZ[i]));  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "Z$\\gamma$");  for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nZG[i],  sqrt(nZG[i]));  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "WV");          for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nWV[i],  sqrt(nWV[i]));  outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "VVV");         for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nVVV[i], sqrt(nVVV[i])); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "backgrounds"); for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nBkg[i], sqrt(nBkg[i])); outputfile << "\\\\\n";
  outputfile << Form(" %-20s", "WZ");          for (UInt_t i=0; i<nCut; i++) outputfile << Form(" & %3.0f $\\pm$ %2.0f", nWZ[i],  sqrt(nWZ[i]));  outputfile << "\\\\\n";

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

  TString filename = Form("tex/systematics_%s_%s.tex", sCut[cut].Data(), sCharge[_wcharge].Data());

  outputfile.open(filename);

  outputfile << Form(" %-30s", "QCD scale");                     for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][qcdSyst][cut]);      outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "PDFs");                          for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][pdfSyst][cut]);      outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "lepton and trigger efficiency"); for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][triggerSyst][cut]);  outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "\\MET");                         for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][metSyst][cut]);      outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "muon momentum scale");           for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][muonSyst][cut]);     outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "electron energy scale");         for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][electronSyst][cut]); outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "pile-up");                       for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][pileupSyst][cut]);   outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "ZZ cross section");              for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][zzSyst][cut]);       outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "Z$\\gamma$ cross section");      for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][zgSyst][cut]);       outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "data-driven");                   for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][fakesSyst][cut]);    outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "backgrounds");                   for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsSystematic[i][nSystematic][cut]);  outputfile << "\\\\\n";

  outputfile << " \\hline\n";

  outputfile << Form(" %-30s", "statistical"); for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsErrorStat[i]); outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "systematic");  for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsErrorSyst[i]); outputfile << "\\\\\n";
  outputfile << Form(" %-30s", "luminosity");  for (UInt_t i=0; i<nChannel; i++) outputfile << Form(" & %.1f", (1e2 / xsValue[i]) * xsErrorLumi[i]); outputfile << "\\\\\n";

  outputfile << " \\hline\n";

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
  hname += "_" + sChannel[channel] + "_" + sCut[cut] + "_" + sCharge[_wcharge];

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
  Double_t theMax   = GetMaximumIncludingErrors(hist[Data], xmin, xmax);
  Double_t theMaxMC = GetMaximumIncludingErrors(allmc,      xmin, xmax);

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
  Double_t delta  = 0.048 + 0.001;
  Double_t ndelta = 0;
  
  DrawLegend(x0 - 0.49, y0 - ndelta, (TObject*)hist[Data], Form(" data (%.0f)", Yield(hist[Data])), "lp"); ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, (TObject*)allmc,      Form(" all (%.0f)",  Yield(allmc)),      "f");  ndelta += delta;
  DrawLegend(x0 - 0.49, y0 - ndelta, (TObject*)hist[WZ],   Form(" WZ (%.0f)",   Yield(hist[WZ])),   "f");  ndelta += delta;

  if (_mode == MCmode)
    {
      DrawLegend(x0 - 0.49, y0 - ndelta, (TObject*)hist[Top],   Form(" top (%.0f)",    Yield(hist[Top])),   "f"); ndelta += delta;
      DrawLegend(x0 - 0.49, y0 - ndelta, (TObject*)hist[ZJets], Form(" Z+jets (%.0f)", Yield(hist[ZJets])), "f"); ndelta += delta;
    }
  else if (_mode == PPFmode)
    {
      DrawLegend(x0 - 0.49, y0 - ndelta, (TObject*)hist[Fakes], Form(" data-driven (%.0f)", Yield(hist[Fakes])), "f"); ndelta += delta;
    }

  ndelta = 0;
  
  DrawLegend(x0 - 0.22, y0 - ndelta, (TObject*)hist[ZZ],  Form(" ZZ (%.0f)",      Yield(hist[ZZ])),  "f"); ndelta += delta;
  DrawLegend(x0 - 0.22, y0 - ndelta, (TObject*)hist[ZG],  Form(" Z#gamma (%.0f)", Yield(hist[ZG])),  "f"); ndelta += delta;
  DrawLegend(x0 - 0.22, y0 - ndelta, (TObject*)hist[VVV], Form(" VVV (%.0f)",     Yield(hist[VVV])), "f"); ndelta += delta;
  DrawLegend(x0 - 0.22, y0 - ndelta, (TObject*)hist[WV],  Form(" WV (%.0f)",      Yield(hist[WV])),  "f"); ndelta += delta;


  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(0.185, 0.975, 0.05, 13, lChannel[channel]);
  DrawTLatex(0.940, 0.983, 0.05, 33, Form("#sqrt{s} = 8 TeV, L = %.1f fb^{-1}", _luminosity/1e3));


  // Fakes checks / systematics
  //----------------------------------------------------------------------------
  if (hname.Contains("hSumCharges") && _verbosity > 2)
    {
      if (_mode == MCmode)
	{
	  printf(" [%s]   Z+jets and top MC: %5.1f\n",
		 sChannel[channel].Data(),
		 Yield(hist[Top]) + Yield(hist[ZJets]));
	}
      else if (_mode == PPFmode)
	{
	  Double_t dataMinusMc = Yield(hist[Data]);
	  
	  dataMinusMc -= Yield(hist[WZ]);
	  dataMinusMc -= Yield(hist[ZZ]);
	  dataMinusMc -= Yield(hist[ZG]);
	  dataMinusMc -= Yield(hist[VVV]);
	  dataMinusMc -= Yield(hist[WV]);

	  printf(" [%s]   data-MC: %5.1f   data PPF: %5.1f   delta: %5.1f\%s\n",
		 sChannel[channel].Data(),
		 dataMinusMc,
		 Yield(hist[Fakes]),
		 1e2 * fabs(dataMinusMc - Yield(hist[Fakes])) / dataMinusMc,
		 "%");
	}
    }


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

    Double_t ratioVal         = (mcValue > 0) ? dtValue/mcValue : 0.0;
    Double_t ratioErr         = (mcValue > 0) ? dtError/mcValue : 0.0;
    Double_t uncertaintyError = (mcValue > 0) ? mcError/mcValue : 0.0;

    ratio->SetBinContent(ibin, ratioVal);
    ratio->SetBinError  (ibin, ratioErr);

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
// SetParameters
//------------------------------------------------------------------------------
void SetParameters(UInt_t cut,
		   UInt_t mode,
		   UInt_t wcharge)
{
  gROOT->SetBatch();

  sCut[Exactly3LeptonsM3L] = "Exactly3LeptonsM3L";
  sCut[HasZ]               = "HasZ";
  //  sCut[PreMET30]           = "PreMET30";
  sCut[MET30]              = "MET30";
  sCut[ClosureTest]        = "ClosureTest";

  sProcess[Data]  = "Data";
  sProcess[Fakes] = "Data_PPF";
  sProcess[WZ]    = "074_WZJetsMad";
  sProcess[ZZ]    = "ZZ";
  sProcess[ZG]    = "086_ZgammaToLLuG";
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

  _verbosity = 0;
  _cut       = cut;
  _mode      = mode;
  _wcharge   = wcharge;
  _localpath = GuessLocalBasePath();

  _datapath = Form("%s/piedra/work/WZ/results",
		   _localpath.Data());

  MakeOutputDirectory("pdf");
  MakeOutputDirectory("png");

  gSystem->mkdir("tex", kTRUE);

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

    TH1D* dummy = (TH1D*)input[j]->Get("hCounter_MME_" + sCut[MET30] + "_" + sCharge[_wcharge] + "_TTT");

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

  Double_t xmin = xs_nlo[_wcharge] -  8.;
  Double_t xmax = xs_nlo[_wcharge] + 23.;
  Double_t ymin = 0.50;
  Double_t ymax = nChannel+1 + ymin;
  
  TH2F* dummy = new TH2F("dummy_xs", "",
			 100, xmin, xmax,
			 100, ymin, ymax);

  dummy->Draw();
  
  
  // NLO WZ cross-section
  //----------------------------------------------------------------------------
  TLine* line = new TLine(xs_nlo[_wcharge], ymin, xs_nlo[_wcharge], ymax);

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

  TString swz = "";

  if      (_wcharge == WPlus)  swz = "W^{+}Z";
  else if (_wcharge == WMinus) swz = "W^{-}Z";
  else                         swz = "W^{#pm}Z";
  
  dummy->GetXaxis()->CenterTitle();
  dummy->GetXaxis()->SetTitleOffset(1.4);
  dummy->GetXaxis()->SetTitle(Form("#sigma(pp #rightarrow %s) [pb]", swz.Data()));
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

  TString xs_name = "xs_" + sCut[cut] + "_" + sCharge[_wcharge];

  canvas->SaveAs(Form("pdf/%s/%s.pdf", _output.Data(), xs_name.Data()));
  canvas->SaveAs(Form("png/%s/%s.png", _output.Data(), xs_name.Data()));
}


//------------------------------------------------------------------------------
// PrintCrossSections
//------------------------------------------------------------------------------
void PrintCrossSections(UInt_t cut)
{
  ofstream outputfile;

  TString suffix = (_mode == MCmode) ? "mc" : "ppf";

  outputfile.open(Form("tex/xs_%s_%s_%s.tex",
		       sCut[cut].Data(),
		       sCharge[_wcharge].Data(),
		       suffix.Data()));


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
// PrintRatios
//------------------------------------------------------------------------------
void PrintRatios(UInt_t cut,
		 UInt_t wchargeNum,
		 UInt_t wchargeDen)
{
  ofstream outputfile;

  TString suffix = (_mode == MCmode) ? "mc" : "ppf";

  outputfile.open(Form("tex/ratio%s%s_%s_%s.tex",
		       sCharge[wchargeNum].Data(),
		       sCharge[wchargeDen].Data(),
		       sCut[cut].Data(),
		       suffix.Data()));


  // Print
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel+1; i++)
    {
      if (i == nChannel) outputfile << "\\hline\n";

      outputfile << Form("%s & %.2f $\\pm$ %.2f (stat.) $\\pm$ %.2f (syst.)\\\\\n",
			 pdfChannel[i].Data(),
			 ratioValue[i],
			 ratioErrorStat[i],
			 ratioErrorSyst[i]);
    }

  outputfile.close();
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
      if (syst == pdfSyst)   {systematicError[channel][WZ]   [syst] =  0.7; continue;}
      if (syst == zzSyst)    {systematicError[channel][ZZ]   [syst] = 15.0; continue;}
      if (syst == zgSyst)    {systematicError[channel][ZG]   [syst] = 15.0; continue;}

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
	  
	  TString suffix = "/" + sProcess[process] + ".root";

	  TFile* f0 = new TFile(_datapath + "/analysis" + suffix);

	  TString hname = "hCounter_" + sChannel[channel] + "_" + sCut[cut] + "_" + sCharge[_wcharge] + "_TTT";
	
	  Double_t y0 = Yield((TH1D*)f0->Get(hname));

	  Double_t y1 = y0;
	  
	  if (syst == muonSyst || syst == electronSyst || syst == metSyst)
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


//------------------------------------------------------------------------------
// BlueMethod
//------------------------------------------------------------------------------
void BlueMethod(UInt_t cut,
		Bool_t xsratio)
{
  UInt_t nMatrixElements = nChannel * nChannel;

  Double_t commonSystMatrix[nChannel][nChannel];

  for (UInt_t i=0; i<nChannel; i++) {
    for (UInt_t j=0; j<nChannel; j++) {

      commonSystMatrix[i][j] = 0.0;

      for (UInt_t k=0; k<=nSystematic; k++) {

	if (k == fakesSyst) continue;
	
	if (xsratio)
	  {
	    commonSystMatrix[i][j] += (ratioSystematic[i][k][cut] * ratioSystematic[j][k][cut]);
	  }
	else
	  {
	    commonSystMatrix[i][j] += (xsSystematic[i][k][cut] * xsSystematic[j][k][cut]);
	  }
      }
    }
  }

  Double_t elm[nMatrixElements];

  for (UInt_t i=0; i<nChannel; i++) {
    for (UInt_t j=i; j<nChannel; j++) {

      if (j == i)
	{
	  if (xsratio)
	    {
	      elm[4*i+j] = pow(ratioErrorStat[i],2) + pow(ratioErrorSyst[i],2);
	    }
	  else
	    {
	      elm[4*i+j] = pow(xsErrorStat[i],2) + pow(xsErrorSyst[i],2);
	    }
	}
      else
	{
	  elm[4*i+j] = elm[4*j+i] = commonSystMatrix[i][j];
	}
    }
  }


  // Deal with the error matrix
  //----------------------------------------------------------------------------
  TMatrixD errorMatrix(nChannel, nChannel, elm);

  TMatrixD errorMatrixCopy(errorMatrix);

  errorMatrix.Invert();

  Double_t *mRef = errorMatrix.GetMatrixArray();

  Double_t norm = 0.;

  Double_t alpha[nChannel] = {0., 0., 0., 0.};

  for (UInt_t i=0; i<nMatrixElements; i++) norm += mRef[i];

  for (UInt_t i=0; i<nChannel;i++) {
    for (UInt_t j=0; j<nChannel;j++) {

      alpha[i] += mRef[4*i+j];
    }

    alpha[i] /= norm;
  }

  Double_t combined_error = 0.0;

  Double_t *copyRef = errorMatrixCopy.GetMatrixArray();

  for (UInt_t i=0;i<nChannel;i++) {
    for (UInt_t j=0;j<nChannel;j++) {

      combined_error += alpha[i] * alpha[j] * copyRef[4*i + j];
    }
  }


  // Get the inclusive values
  //----------------------------------------------------------------------------
  if (xsratio)
    {
      ratioValue    [nChannel] = 0.0;
      ratioErrorStat[nChannel] = 0.0;

      for (UInt_t i=0; i<nChannel; i++) {

	ratioValue[nChannel] += alpha[i] * ratioValue[i];

	ratioErrorStat[nChannel] += pow(alpha[i] * ratioErrorStat[i],2);
      }

      ratioErrorSyst[nChannel] = sqrt (combined_error - ratioErrorStat[nChannel]);
      ratioErrorStat[nChannel] = sqrt(ratioErrorStat[nChannel]);
    }
  else
    {
      xsValue    [nChannel] = 0.0;
      xsErrorStat[nChannel] = 0.0;

      for (UInt_t i=0; i<nChannel; i++) {

	xsValue[nChannel] += alpha[i] * xsValue[i];
	
	xsErrorStat[nChannel] += pow(alpha[i] * xsErrorStat[i],2);
      }
  
      xsErrorSyst[nChannel] = sqrt (combined_error - xsErrorStat[nChannel]);
      xsErrorStat[nChannel] = sqrt(xsErrorStat[nChannel]);
      xsErrorLumi[nChannel] = _luminosityUncertainty * xsValue[nChannel] / 1e2;
    }


  // Print
  //----------------------------------------------------------------------------
  if (_verbosity > 1 && cut == MET30 && _wcharge == WInclusive)
    {
      printf("\n");

      for (UInt_t i=0; i<nChannel; i++)
	printf("       alpha(%s) = %.3f\n", sChannel[i].Data(), alpha[i]);

      if (xsratio)
	{
	  printf("\n       ratio(BLUE) = %.4f +- %.4f (stat) +- %.4f (syst) \t 1/ratio = %.4f\n\n",
		 ratioValue[nChannel],
		 ratioErrorStat[nChannel],
		 ratioErrorSyst[nChannel],
		 1. / ratioValue[nChannel]);
	}
      else
	{
	  printf("\n       xs(BLUE) = %.4f +- %.4f (stat) +- %.4f (syst) +- %.4f (lumi) pb\n\n",
		 xsValue[nChannel],
		 xsErrorStat[nChannel],
		 xsErrorSyst[nChannel],
		 xsErrorLumi[nChannel]);
	}
    }
}


//------------------------------------------------------------------------------
// Ratio
//------------------------------------------------------------------------------
Double_t Ratio(Double_t a,
	       Double_t b)
{
  if (b == 0) return 0.0;

  return a / b;
}


//------------------------------------------------------------------------------
// RatioError
//------------------------------------------------------------------------------
Double_t RatioError(Double_t a,
		    Double_t b,
		    Double_t aErr,
		    Double_t bErr)
{
  if (b == 0) return 0.0;

  if (aErr < 0) aErr = sqrt(a);
  if (bErr < 0) bErr = sqrt(b);

  return Ratio(a,b) * sqrt((aErr/a)*(aErr/a) + (bErr/b)*(bErr/b));
}
