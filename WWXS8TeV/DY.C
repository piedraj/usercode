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


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Input histograms
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TH1F* hNinDYSF  [numberMetCuts];
  TH1F* hNinWZSF  [numberMetCuts];
  TH1F* hNinZZSF  [numberMetCuts];
  TH1F* hNinDataSF[numberMetCuts];
  TH1F* hNinDataOF[numberMetCuts];

  TH1F* hNoutDYSF  [numberMetCuts];
  TH1F* hNoutWZSF  [numberMetCuts];
  TH1F* hNoutZZSF  [numberMetCuts];
  TH1F* hNoutDataSF[numberMetCuts];
  TH1F* hNoutDataOF[numberMetCuts];

  for (UInt_t nC=0; nC<numberMetCuts; nC++) {
    hNinDYSF  [nC] = (TH1F*)inputDYSF  ->Get(Form("hNinZevents%.2f", MetCuts[nC]));
    hNinWZSF  [nC] = (TH1F*)inputWZSF  ->Get(Form("hNinZevents%.2f", MetCuts[nC]));
    hNinZZSF  [nC] = (TH1F*)inputZZSF  ->Get(Form("hNinZevents%.2f", MetCuts[nC]));
    hNinDataSF[nC] = (TH1F*)inputDataSF->Get(Form("hNinZevents%.2f", MetCuts[nC]));
    hNinDataOF[nC] = (TH1F*)inputDataOF->Get(Form("hNinZevents%.2f", MetCuts[nC]));

    hNoutDYSF  [nC] = (TH1F*)inputDYSF  ->Get(Form("hNoutZevents%.2f", MetCuts[nC]));     
    hNoutWZSF  [nC] = (TH1F*)inputWZSF  ->Get(Form("hNoutZevents%.2f", MetCuts[nC]));     
    hNoutZZSF  [nC] = (TH1F*)inputZZSF  ->Get(Form("hNoutZevents%.2f", MetCuts[nC]));     
    hNoutDataSF[nC] = (TH1F*)inputDataSF->Get(Form("hNoutZevents%.2f", MetCuts[nC]));
    hNoutDataOF[nC] = (TH1F*)inputDataOF->Get(Form("hNoutZevents%.2f", MetCuts[nC]));
  }


  // Histogram at analysis level
  //----------------------------------------------------------------------------
  TH1F* hExpectedDYSF = (TH1F*)inputDYSF->Get("hWTopTagging");


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // k estimation
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TFile* inputDYmumu = new TFile(Form("%s/%djet/MuMu/DY.root", directory.Data(), njet));
  TFile* inputDYee   = new TFile(Form("%s/%djet/EE/DY.root",   directory.Data(), njet));

  TH1F* hNinDYmumu = (TH1F*)inputDYmumu->Get("hNinLooseZevents20.00");
  TH1F* hNinDYee   = (TH1F*)inputDYee  ->Get("hNinLooseZevents20.00");

  Double_t NinDYmumu = hNinDYmumu->GetBinContent(2);
  Double_t NinDYee   = hNinDYee  ->GetBinContent(2);

  Double_t k    = 0.5 * (sqrt(NinDYmumu / NinDYee) + sqrt(NinDYee / NinDYmumu));
  Double_t errk = errkSF(NinDYmumu, NinDYee);

  if (channel == "MuMu") {
    k    = 0.5 * sqrt(NinDYmumu / NinDYee);
    errk = errkFunction(NinDYmumu, NinDYee);
  }
  else if (channel == "EE") {
    k    = 0.5 * sqrt(NinDYee / NinDYmumu);
    errk = errkFunction(NinDYee, NinDYmumu);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // Counters
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t NinDYSF  [numberMetCuts];
  Double_t NinWZSF  [numberMetCuts];
  Double_t NinZZSF  [numberMetCuts];
  Double_t NinDataSF[numberMetCuts];
  Double_t NinDataOF[numberMetCuts];

  Double_t NoutDYSF  [numberMetCuts];
  Double_t NoutWZSF  [numberMetCuts];  // Not used for now
  Double_t NoutZZSF  [numberMetCuts];  // Not used for now
  Double_t NoutDataSF[numberMetCuts];
  Double_t NoutDataOF[numberMetCuts];

  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {
    NinDYSF   [nC] = hNinDYSF   [nC]->GetBinContent(2);
    NinWZSF   [nC] = hNinWZSF   [nC]->GetBinContent(2);
    NinZZSF   [nC] = hNinZZSF   [nC]->GetBinContent(2);
    NinDataSF [nC] = hNinDataSF [nC]->GetBinContent(2);    
    NinDataOF [nC] = hNinDataOF [nC]->GetBinContent(2);

    NoutDYSF  [nC] = hNoutDYSF  [nC]->GetBinContent(2);
    NoutWZSF  [nC] = hNoutWZSF  [nC]->GetBinContent(2);
    NoutZZSF  [nC] = hNoutZZSF  [nC]->GetBinContent(2);
    NoutDataSF[nC] = hNoutDataSF[nC]->GetBinContent(2);    
    NoutDataOF[nC] = hNoutDataOF[nC]->GetBinContent(2);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  // R estimation
  //
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Double_t R       [numberMetCuts];
  Double_t RData   [numberMetCuts];
  Double_t errR    [numberMetCuts];
  Double_t errRData[numberMetCuts];


  // Loop over the met cuts
  //----------------------------------------------------------------------------
  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    R   [nC] = NoutDYSF[nC] / NinDYSF[nC]; 
    errR[nC] = errRFunction(NoutDYSF[nC], NinDYSF[nC]);

    RData   [nC] = RDataFunction   (NoutDataSF[nC], NoutDataOF[nC], NinDataSF[nC], NinDataOF[nC], k);
    errRData[nC] = errRDataFunction(NoutDataSF[nC], NoutDataOF[nC], NinDataSF[nC], NinDataOF[nC], k, errk);


    if (printLevel > 2) {
      printf("\n %.0f < mpmet < %.0f GeV\n", MetCuts[nC], MetCuts[nC+1]);
      printf(" -------------------------------------------------\n");
      printf("         N^{MC}_{out,SF}   = %6.1f\n", NoutDYSF[nC]);
      printf("         N^{MC}_{in, SF}   = %6.1f\n", NinDYSF[nC]);
      printf("         N^{data}_{out,SF} = %4.0f\n", NoutDataSF[nC]);
      printf("         N^{data}_{out,OF} = %4.0f\n", NoutDataOF[nC]);
      printf("         N^{data}_{in, SF} = %4.0f\n", NinDataSF[nC]);
      printf("         N^{data}_{in, OF} = %4.0f\n", NinDataOF[nC]);
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


  // Estimate Nout
  //----------------------------------------------------------------------------
  Double_t NinCountedSFWZ   = NinWZSF  [sysR];
  Double_t NinCountedSFZZ   = NinZZSF  [sysR];
  Double_t NinCountedSFData = NinDataSF[sysR];
  Double_t NinCountedOFData = NinDataOF[sysR];

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
    printf("         N^{WZ}_{in,SF}   = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NinCountedSFWZ, sqrt(NinCountedSFWZ), 0.1*NinCountedSFWZ);
    printf("         N^{ZZ}_{in,SF}   = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NinCountedSFZZ, sqrt(NinCountedSFZZ), 0.1*NinCountedSFZZ);
    printf("         N^{est}_{in, SF} = %7.2f +- %6.2f\n", NinEstSFFinal, errNinEstSFFinal);
    printf("         N^{est}_{out,SF} = %7.2f +- %6.2f (stat.) +- %6.2f (syst.)\n",
	   NestSFFinal, errNestSFFinal, RelDiffR*NestSFFinal);
    printf(" -------------------------------------------------\n");
    printf(" [no VZ] N^{est}_{out,SF} = %7.2f +- %6.2f (stat.) +- %6.2f (syst.) = %7.2f +- %6.2f (stat. + syst.)\n",
	   NestSFNoDibosonFinal, errNestSFNoDibosonFinal, RelDiffR*NestSFNoDibosonFinal,
	   NestSFNoDibosonFinal, totalError);
    printf("         N^{MC}_{out,SF}  = %7.2f +- %6.2f\n",
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


  // Check
  //----------------------------------------------------------------------------
  Double_t check = hExpectedDYSF->GetBinContent(2) - NoutDYSF[sysR];
  if (check != 0) printf(" WARNING: DY yields do not much by %f\n\n", check);


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
