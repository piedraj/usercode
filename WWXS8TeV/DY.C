#include "TCanvas.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"


const UInt_t numberMetCuts = 5;

Float_t MetCuts[numberMetCuts] = {20, 25, 30, 45, 1000};
Float_t MetDraw[numberMetCuts] = {20, 25, 30, 45,   75};


TCanvas* canvas = NULL;


// Member functions
//------------------------------------------------------------------------------
Double_t errkFunction              (Double_t a,
				    Double_t b);

Double_t errkSF                    (Double_t a,
				    Double_t b);

Double_t errRFunction              (Double_t Nout,
				    Double_t Nin);

Double_t RDataFunction             (Double_t NoutSF,
				    Double_t NoutOF,
				    Double_t NinSF,
				    Double_t NinOF,
				    Double_t k);

Double_t errRDataFunction          (Double_t NoutSF,
				    Double_t NoutOF,
				    Double_t NinSF,
				    Double_t NinOF,
				    Double_t k,
				    Double_t errk);

Double_t errNinEstFunction         (Double_t NinDataSF,
				    Double_t NinDataOF,
				    Double_t k,
				    Double_t errk);

Double_t errNestFunction           (Double_t Nest,
				    Double_t R,
				    Double_t errR,
				    Double_t Ninest,
				    Double_t errNinest);

Double_t errNinEstNoDibosonFunction(Double_t NinDataSF,
				    Double_t k,
				    Double_t errk,
				    Double_t NinDataOF,
				    Double_t NinWZ,
				    Double_t NinZZ);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// DY
//
// channel = SF, MuMu, EE
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void DY(Double_t &yield,
	Double_t &statError,
	Double_t &systError,
	Double_t &scaleFactor,
	Int_t     njet,
	TString   channel,
	TString   directory,
	Bool_t    useDataDriven,
	Int_t     printLevel,
	Bool_t    drawR = false)
{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Input files
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TString path = Form("%s/%djet/%s/", directory.Data(), njet, channel.Data());

  TFile* inputDYSF   = new TFile(path + "DY.root");
  TFile* inputZZSF   = new TFile(path + "ZZ.root");
  TFile* inputWZSF   = new TFile(path + "WZ.root");
  TFile* inputDataSF = new TFile(path + "DataRun2012_Total.root");

  TFile* inputDataOF = new TFile(Form("%s/%djet/OF/DataRun2012_Total.root", directory.Data(), njet));
  TFile* inputZmumu  = new TFile(Form("%s/%djet/MuMu/DY.root",              directory.Data(), njet));
  TFile* inputZee    = new TFile(Form("%s/%djet/EE/DY.root",                directory.Data(), njet));


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Input histograms
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TH1F* hNinZeventsZSF    [numberMetCuts];
  TH1F* hNinZeventsDataSF [numberMetCuts];
  TH1F* hNinZeventsDataOF [numberMetCuts];
  TH1F* hNoutZeventsZSF   [numberMetCuts];
  TH1F* hNoutZeventsDataSF[numberMetCuts];
  TH1F* hNoutZeventsDataOF[numberMetCuts];

  for (UInt_t nC=0; nC<numberMetCuts; nC++) {
    hNinZeventsZSF    [nC] = (TH1F*)inputDYSF  ->Get(Form("hNinZevents%.1f",  MetCuts[nC]));
    hNinZeventsDataSF [nC] = (TH1F*)inputDataSF->Get(Form("hNinZevents%.1f",  MetCuts[nC]));
    hNinZeventsDataOF [nC] = (TH1F*)inputDataOF->Get(Form("hNinZevents%.1f",  MetCuts[nC]));
    hNoutZeventsZSF   [nC] = (TH1F*)inputDYSF  ->Get(Form("hNoutZevents%.1f", MetCuts[nC]));     
    hNoutZeventsDataSF[nC] = (TH1F*)inputDataSF->Get(Form("hNoutZevents%.1f", MetCuts[nC]));
    hNoutZeventsDataOF[nC] = (TH1F*)inputDataOF->Get(Form("hNoutZevents%.1f", MetCuts[nC]));
  }


  // Histograms at (pfmet > 20 && mpmet > 20)
  //----------------------------------------------------------------------------
  TH1F* hNinZeventsZmumu = (TH1F*)inputZmumu->Get("hNinLooseZevents20.0");
  TH1F* hNinZeventsZee   = (TH1F*)inputZee  ->Get("hNinLooseZevents20.0");


  // Histograms at mpmet > 45 for 0/1-jet or pfmet > 45 for 2-jet
  //----------------------------------------------------------------------------
  TH1F* hNinWZSF = (TH1F*)inputWZSF->Get("hNinZevents45.0");
  TH1F* hNinZZSF = (TH1F*)inputZZSF->Get("hNinZevents45.0");


  // Histogram at analysis level
  //----------------------------------------------------------------------------
  TH1F* hExpectedDYSF = (TH1F*) inputDYSF->Get("hWTopTagging");


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Counters
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t NinSF     [numberMetCuts];
  Double_t NinSFData [numberMetCuts];
  Double_t NinOFData [numberMetCuts];
  Double_t NoutSF    [numberMetCuts];
  Double_t NoutSFData[numberMetCuts];
  Double_t NoutOFData[numberMetCuts];

  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {
    NinSF     [nC] = hNinZeventsZSF    [nC]->GetBinContent(2);
    NinSFData [nC] = hNinZeventsDataSF [nC]->GetBinContent(2);    
    NinOFData [nC] = hNinZeventsDataOF [nC]->GetBinContent(2);
    NoutSF    [nC] = hNoutZeventsZSF   [nC]->GetBinContent(2);
    NoutSFData[nC] = hNoutZeventsDataSF[nC]->GetBinContent(2);    
    NoutOFData[nC] = hNoutZeventsDataOF[nC]->GetBinContent(2);
  }

  Double_t NinMuMu = hNinZeventsZmumu->GetBinContent(2);
  Double_t NinEE   = hNinZeventsZee  ->GetBinContent(2);

  Double_t NinCountedSFWZ   = hNinWZSF->GetBinContent(2);
  Double_t NinCountedSFZZ   = hNinZZSF->GetBinContent(2);
  Double_t NinCountedSFData = NinSFData[numberMetCuts-2];
  Double_t NinCountedOFData = NinOFData[numberMetCuts-2];


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // k and R estimation
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t k    = 0.5 * (sqrt(NinMuMu / NinEE) + sqrt(NinEE / NinMuMu));
  Double_t errk = errkSF(NinMuMu, NinEE);

  if (channel == "MuMu") {
    k    = 0.5 * sqrt(NinMuMu / NinEE);
    errk = errkFunction(NinMuMu, NinEE);
  }
  else if (channel == "EE") {
    k    = 0.5 * sqrt(NinEE / NinMuMu);
    errk = errkFunction(NinEE, NinMuMu);
  }


  // R estimation
  //----------------------------------------------------------------------------
  Double_t R       [numberMetCuts];
  Double_t RData   [numberMetCuts];
  Double_t errR    [numberMetCuts];
  Double_t errRData[numberMetCuts];


  // Loop over the met cuts
  //----------------------------------------------------------------------------
  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    R   [nC] = NoutSF[nC] / NinSF[nC]; 
    errR[nC] = errRFunction(NoutSF[nC], NinSF[nC]);

    RData   [nC] = RDataFunction   (NoutSFData[nC], NoutOFData[nC], NinSFData[nC], NinOFData[nC], k);
    errRData[nC] = errRDataFunction(NoutSFData[nC], NoutOFData[nC], NinSFData[nC], NinOFData[nC], k, errk);


    if (printLevel > 2) {
      printf("\n %.0f < mpmet < %.0f GeV\n", MetCuts[nC], MetCuts[nC+1]);
      printf(" -------------------------------------------------\n");
      printf("         N^{MC}_{out,SF}   = %6.1f\n", NoutSF[nC]);
      printf("         N^{MC}_{in, SF}   = %6.1f\n", NinSF[nC]);
      printf("         N^{data}_{out,SF} = %4.0f\n", NoutSFData[nC]);
      printf("         N^{data}_{out,OF} = %4.0f\n", NoutOFData[nC]);
      printf("         N^{data}_{in, SF} = %4.0f\n", NinSFData[nC]);
      printf("         N^{data}_{in, OF} = %4.0f\n", NinOFData[nC]);
      printf("         k                 = % 5.3f +- %5.3f\n", k,         errk);
      printf("         R^{MC}            = % 5.3f +- %5.3f\n", R[nC],     errR[nC]);
      printf("         R^{data}          = % 5.3f +- %5.3f\n", RData[nC], errRData[nC]);
    }
  }


  // Estimate the R systematic as the difference between R[2] and R[3]
  //----------------------------------------------------------------------------
  Int_t iMaxR = 0; 
  Int_t iMinR = 0; 

  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    if (R[nC] > 0 && R[nC] > R[iMaxR]) iMaxR = nC;
    if (R[nC] > 0 && R[nC] < R[iMinR]) iMinR = nC;
  }

  Int_t theR = 2;
  Int_t sysR = 3;

  Double_t RelDiffR = (R[theR] > 0) ? fabs(R[theR] - R[sysR]) / R[theR] : -999;


  if (printLevel > 0) {
    printf("\n [%s] R systematic uncertainty\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf("         min R              = %5.3f\n",     R[iMinR]);
    printf("         max R              = %5.3f\n",     R[iMaxR]);
    printf("         R[%d]               = %5.3f\n",    theR, R[theR]);
    printf("         R[%d]               = %5.3f\n",    sysR, R[sysR]);
    printf("         |R[%d]-R[%d]| / R[%d] = %.1f%s\n", theR, sysR, theR, 1e2*RelDiffR, "%");
    printf("\n");
  }


  // Remove WZ and ZZ dibosons
  //----------------------------------------------------------------------------
  Double_t NinEstSFFinal    = NinCountedSFData - k*NinCountedOFData;
  Double_t errNinEstSFFinal = errNinEstFunction(NinCountedSFData, NinCountedOFData, k, errk);

  Double_t NestSFFinal    = R[theR] * NinEstSFFinal;
  Double_t errNestSFFinal = errNestFunction(NestSFFinal, R[theR], errR[theR], NinEstSFFinal, errNinEstSFFinal);

  Double_t NinEstSFNoDibosonFinal    = NinEstSFFinal - NinCountedSFWZ - NinCountedSFZZ;
  Double_t errNinEstSFNoDibosonFinal = errNinEstNoDibosonFunction(NinCountedSFData, k, errk, NinCountedOFData, NinCountedSFWZ, NinCountedSFZZ);

  Double_t NestSFNoDibosonFinal    = R[theR] * NinEstSFNoDibosonFinal;
  Double_t errNestSFNoDibosonFinal = errNestFunction(NestSFNoDibosonFinal, R[theR], errR[theR], NinEstSFNoDibosonFinal, errNinEstSFNoDibosonFinal);
  Double_t totalError              = sqrt(errNestSFNoDibosonFinal*errNestSFNoDibosonFinal + (RelDiffR*NestSFNoDibosonFinal)*(RelDiffR*NestSFNoDibosonFinal));


  Double_t SFsf = NestSFNoDibosonFinal / hExpectedDYSF->GetBinContent(2);


  if (printLevel > 1) {
    printf("\n Analysis results\n");
    printf(" -------------------------------------------------\n");
    printf("         N^{data}_{in,SF} = %4.0f\n", NinCountedSFData);
    printf("         N^{data}_{in,OF} = %4.0f\n", NinCountedOFData);
    printf("         k                = %5.3f +- %5.3f\n", k, errk);
    printf("         R[%d]             = %5.3f +- %5.3f\n", theR, R[theR], errR[theR]);
    printf("         N^{WZ}_{in,SF}   = %6.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NinCountedSFWZ, sqrt(NinCountedSFWZ), 0.1*NinCountedSFWZ);
    printf("         N^{ZZ}_{in,SF}   = %6.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NinCountedSFZZ, sqrt(NinCountedSFZZ), 0.1*NinCountedSFZZ);
    printf("         N^{est}_{in, SF} = %6.2f +- %6.2f\n", NinEstSFFinal, errNinEstSFFinal);
    printf("         N^{est}_{out,SF} = %6.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NestSFFinal, errNestSFFinal, RelDiffR*NestSFFinal);
    printf(" -------------------------------------------------\n");
    printf(" [no VZ] N^{est}_{out,SF} = %6.2f +- %6.2f (stat.) +- %6.2f (syst.) = %6.2f +- %6.2f (stat. + syst.)\n",
	   NestSFNoDibosonFinal, errNestSFNoDibosonFinal, RelDiffR*NestSFNoDibosonFinal,
	   NestSFNoDibosonFinal, totalError);
    printf("         N^{MC}_{out,SF}  = %6.2f +- %6.2f\n",
	   hExpectedDYSF->GetBinContent(2), hExpectedDYSF->GetBinError(2));
    printf("     *** scale factor     = %.3f\n\n", SFsf);
  }


  // Save the result
  //----------------------------------------------------------------------------
  yield       = (useDataDriven) ? NestSFNoDibosonFinal : hExpectedDYSF->GetBinContent(2);
  statError   = errNestSFNoDibosonFinal;
  systError   = RelDiffR*NestSFNoDibosonFinal;
  scaleFactor = yield / hExpectedDYSF->GetBinContent(2);


  // For the note
  //----------------------------------------------------------------------------
  if (printLevel > 0) {
    printf("\n [%s] DY values for the note\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf(" final state   &             R_{MC}  &  N^{control,data}  &     N_{DY}^{data}  &       N_{DY}^{MC}  &  data/MC\n");
    printf(" same flavour  &  %5.3f $\\pm$ %5.3f  &              %4.0f  &  %5.1f $\\pm$ %4.1f  &  %5.1f $\\pm$ %4.1f  &     %4.1f\n\n",
	 R[theR],
	   errR[theR],
	   NinCountedSFData,
	   yield,
	   statError,
	   hExpectedDYSF->GetBinContent(2),
	   hExpectedDYSF->GetBinError(2),
	   scaleFactor);
    printf("\n [%s] DY relative systematic uncertainties\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf(" DY normalisation = %.0f (stat.) $\\bigoplus$ %.0f (syst.)\n\n",
	   1e2*statError/yield, 1e2*systError/yield);
  }


  // Draw histograms
  //----------------------------------------------------------------------------
  if (drawR) {

    Double_t absoluteMin = 999;

    TGraphErrors* gR     = new TGraphErrors(numberMetCuts-1);
    TGraphErrors* gRdata = new TGraphErrors(numberMetCuts-1);

    for (UInt_t i=0; i<numberMetCuts-1; i++) {

      gR->SetPoint     (i, 0.5 * (MetDraw[i+1] + MetDraw[i]),    R[i]);
      gR->SetPointError(i, 0.5 * (MetDraw[i+1] - MetDraw[i]), errR[i]);

      gRdata->SetPoint     (i, 0.5 * (MetDraw[i+1] + MetDraw[i]),    RData[i]);
      gRdata->SetPointError(i, 0.5 * (MetDraw[i+1] - MetDraw[i]), errRData[i]);

      if (absoluteMin > (R[i]     - errR[i]))     absoluteMin = R[i]     - errR[i];
      if (absoluteMin > (RData[i] - errRData[i])) absoluteMin = RData[i] - errRData[i];
    }
    
    if (absoluteMin > 0) absoluteMin = 0;


    // Cosmetics
    //--------------------------------------------------------------------------
    gR->SetMarkerSize (0.9);
    gR->SetMarkerStyle(kFullCircle);

    gRdata->SetLineColor  (kRed+1);
    gRdata->SetMarkerColor(kRed+1);
    gRdata->SetMarkerSize (0.9);
    gRdata->SetMarkerStyle(kFullCircle);


    // Draw
    //--------------------------------------------------------------------------
    canvas = new TCanvas();

    TMultiGraph *mgR = new TMultiGraph();
    mgR->Add(gRdata);
    mgR->Add(gR);

    mgR->Draw("ap");

    mgR->GetYaxis()->SetTitle("R^{out/in}");
    mgR->GetXaxis()->SetTitle("mpmet (GeV)");

    mgR->SetMinimum(absoluteMin - 0.1);
    mgR->SetMaximum(1.0);


    // Legend
    //--------------------------------------------------------------------------
    TLegend* lmgR = new TLegend(0.72, 0.68, 0.92, 0.88);

    lmgR->AddEntry(gR,    " DY MC", "lp");
    lmgR->AddEntry(gRdata," data",  "lp");

    lmgR->SetFillColor(0);
    lmgR->SetTextAlign(12);
    lmgR->SetTextFont (42);
    lmgR->SetTextSize (0.04);

    if      (channel == "SF")   lmgR->SetHeader("ee + #mu#mu");
    else if (channel == "EE")   lmgR->SetHeader("ee");
    else if (channel == "MuMu") lmgR->SetHeader("#mu#mu");

    lmgR->Draw("same");


    // Line at zero
    //--------------------------------------------------------------------------
    TLine* zeroLine = new TLine(canvas->GetUxmin(), 0.0, canvas->GetUxmax(), 0.0);
    zeroLine->SetLineStyle(3);
    zeroLine->SetLineWidth(2);
    zeroLine->Draw("same");
    mgR->Draw("p,same");
  

    // Save
    //--------------------------------------------------------------------------
    canvas->SaveAs("R_" + channel + ".png");
  }
}


//------------------------------------------------------------------------------
// errkFunction
//------------------------------------------------------------------------------
Double_t errkFunction(Double_t a, Double_t b)
{
  Double_t errk = sqrt((1 + a/b) / b) / 4.0;

  return errk;
}


//------------------------------------------------------------------------------
// errkSF
//------------------------------------------------------------------------------
Double_t errkSF(Double_t a, Double_t b)
{
  Double_t errk = sqrt(b/(a*a) + a/(b*b) - 1/b - 1/a) / 4.0;

  return errk;
}


//------------------------------------------------------------------------------
// errRFunction
//------------------------------------------------------------------------------
Double_t errRFunction(Double_t Nout, Double_t Nin)
{
  Double_t R = Nout / Nin;

  Double_t errR = R * sqrt(1.0/Nout + 1.0/Nin);

  return errR;
}


//------------------------------------------------------------------------------
// RDataFunction
//------------------------------------------------------------------------------
Double_t RDataFunction(Double_t NoutSF,
		       Double_t NoutOF,
		       Double_t NinSF,
		       Double_t NinOF,
		       Double_t k)
{
  Double_t R = (NoutSF - k*NoutOF) / (NinSF - k*NinOF);

  return R;
}


//------------------------------------------------------------------------------
// errRDataFunction
//------------------------------------------------------------------------------
Double_t errRDataFunction(Double_t NoutSF,
			  Double_t NoutOF,
			  Double_t NinSF,
			  Double_t NinOF,
			  Double_t k,
			  Double_t errk)
{
  Double_t num = NoutSF - k*NoutOF;
  Double_t den = NinSF  - k*NinOF;
  
  Double_t Term1 = sqrt(NoutSF + sqrt((errk*NoutOF)*(errk*NoutOF) + k*k*NoutOF));
  Double_t Term2 = sqrt(NinSF  + sqrt((errk*NinOF) *(errk*NinOF)  + k*k*NinOF));

  Term1 = Term1 / den;
  Term2 = num * Term2 / (den*den);

  Double_t errRData = sqrt(Term1*Term1 + Term2*Term2);

  return errRData;
}


//------------------------------------------------------------------------------
// errNinEstFunction
//------------------------------------------------------------------------------
Double_t errNinEstFunction(Double_t NinDataSF,
			   Double_t NinDataOF,
			   Double_t k,
			   Double_t errk)
{
  Double_t errNinEst = NinDataSF;

  errNinEst += sqrt(k*k*NinDataOF + (NinDataOF*errk)*(NinDataOF*errk));

  errNinEst = sqrt(errNinEst);

  return errNinEst;
}


//------------------------------------------------------------------------------
// errNestFunction
//------------------------------------------------------------------------------
Double_t errNestFunction(Double_t Nest,
			 Double_t R,
			 Double_t errR,
			 Double_t Ninest,
			 Double_t errNinest)
{
  Double_t errNest = (errR*errR)/(R*R) + (errNinest*errNinest)/(Ninest*Ninest);

  errNest *= (Nest*Nest);

  errNest = sqrt(errNest);

  return errNest;
}


//------------------------------------------------------------------------------
// errNinEstNoDibosonFunction
//------------------------------------------------------------------------------
Double_t errNinEstNoDibosonFunction(Double_t NinDataSF,
				    Double_t k,
				    Double_t errk,
				    Double_t NinDataOF,
				    Double_t NinWZ,
				    Double_t NinZZ)
{
  Double_t errNWZ = sqrt(NinWZ + (0.1*NinWZ)*(0.1*NinWZ));
  Double_t errNZZ = sqrt(NinZZ + (0.1*NinZZ)*(0.1*NinZZ));

  Double_t errNinEst = NinDataSF + errNWZ*errNWZ + errNZZ*errNZZ;

  errNinEst += sqrt(k*k*NinDataOF + (NinDataOF*errk)*(NinDataOF*errk));

  errNinEst = sqrt(errNinEst);

  return errNinEst;
}
