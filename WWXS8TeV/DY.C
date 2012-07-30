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

Double_t RDATAFunction             (Double_t NoutSF,
				    Double_t NoutOF,
				    Double_t NinSF,
				    Double_t NinOF,
				    Double_t k);

Double_t errRDATAFunction          (Double_t NoutSF,
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


//------------------------------------------------------------------------------
//
// DY
//
// channel = SFChannel, MuMuChannel, EEChannel
//
//------------------------------------------------------------------------------
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
  // Input files
  //----------------------------------------------------------------------------
  TString path = Form("%s/%djet/%s/", directory.Data(), njet, channel.Data());

  TFile* inputZSF    = new TFile(path + "DY.root");
  TFile* inputZZSF   = new TFile(path + "ZZ.root");
  TFile* inputWZSF   = new TFile(path + "WZ.root");
  TFile* inputDATASF = new TFile(path + "DataRun2012_Total.root");

  TFile* inputDATAOF    = new TFile(Form("%s/%djet/OF/DataRun2012_Total.root", directory.Data(), njet));
  TFile* inputZmumuMuMu = new TFile(Form("%s/%djet/MuMu/DY.root",              directory.Data(), njet));
  TFile* inputZeeEE     = new TFile(Form("%s/%djet/EE/DY.root",                directory.Data(), njet));


  // Declare histograms vs. mpmet cut
  //----------------------------------------------------------------------------
  TH1F* hNinZeventsZSF    [numberMetCuts];
  TH1F* hNinZeventsDATASF [numberMetCuts];
  TH1F* hNinZeventsDATAOF [numberMetCuts];
  TH1F* hNoutZeventsZSF   [numberMetCuts];
  TH1F* hNoutZeventsDATASF[numberMetCuts];
  TH1F* hNoutZeventsDATAOF[numberMetCuts];


  // Read histograms vs. mpmet cut
  //----------------------------------------------------------------------------
  for (UInt_t nC=0; nC<numberMetCuts; nC++) {

    hNinZeventsZSF   [nC] = (TH1F*) inputZSF   ->Get(Form("hNinZevents%.1f", MetCuts[nC]));
    hNinZeventsDATASF[nC] = (TH1F*) inputDATASF->Get(Form("hNinZevents%.1f", MetCuts[nC]));
    hNinZeventsDATAOF[nC] = (TH1F*) inputDATAOF->Get(Form("hNinZevents%.1f", MetCuts[nC]));

    hNoutZeventsZSF   [nC] = (TH1F*) inputZSF   ->Get(Form("hNoutZevents%.1f", MetCuts[nC]));     
    hNoutZeventsDATASF[nC] = (TH1F*) inputDATASF->Get(Form("hNoutZevents%.1f", MetCuts[nC]));
    hNoutZeventsDATAOF[nC] = (TH1F*) inputDATAOF->Get(Form("hNoutZevents%.1f", MetCuts[nC]));
  }


  // Read histograms at mpmet > 20 GeV
  //----------------------------------------------------------------------------
  TH1F* hNinZeventsZmumuMuMu = (TH1F*) inputZmumuMuMu->Get("hNinLooseZevents20.0");
  TH1F* hNinZeventsZeeEE     = (TH1F*) inputZeeEE    ->Get("hNinLooseZevents20.0");


  // Read histograms at analysis level mpmet cut
  //----------------------------------------------------------------------------
  TH1F* hExpectedDYSF = (TH1F*) inputZSF   ->Get("hWTopTagging");
  TH1F* hNinWZSF      = (TH1F*) inputWZSF  ->Get("hCountedMinvDYStudies");
  TH1F* hNinZZSF      = (TH1F*) inputZZSF  ->Get("hCountedMinvDYStudies");
  TH1F* hNinDATASF    = (TH1F*) inputDATASF->Get("hCountedMinvDYStudies");
  TH1F* hNinDATAOF    = (TH1F*) inputDATAOF->Get("hCountedMinvDYStudies");


  // Counters
  //----------------------------------------------------------------------------
  Double_t NinSF    [numberMetCuts];
  Double_t NinSFDATA[numberMetCuts];
  Double_t NinOFDATA[numberMetCuts];

  Double_t NoutSF    [numberMetCuts];
  Double_t NoutSFDATA[numberMetCuts];
  Double_t NoutOFDATA[numberMetCuts];

  Double_t NinMuMu = hNinZeventsZmumuMuMu->GetBinContent(2);
  Double_t NinEE   = hNinZeventsZeeEE    ->GetBinContent(2);

  Double_t NinCountedSFWZ   = hNinWZSF  ->GetBinContent(2);
  Double_t NinCountedSFZZ   = hNinZZSF  ->GetBinContent(2);
  Double_t NinCountedSFDATA = hNinDATASF->GetBinContent(2);
  Double_t NinCountedOFDATA = hNinDATAOF->GetBinContent(2);


  // k estimation
  //----------------------------------------------------------------------------
  Double_t k    = 0.5 * (sqrt(NinMuMu / NinEE) + sqrt(NinEE / NinMuMu));
  Double_t errk = errkSF(NinMuMu, NinEE);

  if (channel == "MuMuChannel") {
    k    = 0.5 * sqrt(NinMuMu / NinEE);
    errk = errkFunction(NinMuMu, NinEE);
  }
  else if (channel == "EEChannel") {
    k    = 0.5 * sqrt(NinEE / NinMuMu);
    errk = errkFunction(NinEE, NinMuMu);
  }


  // R
  //----------------------------------------------------------------------------
  Double_t R   [numberMetCuts];
  Double_t errR[numberMetCuts];

  Double_t RDATA   [numberMetCuts];
  Double_t errRDATA[numberMetCuts];


  // Loop over the mpmet cuts
  //----------------------------------------------------------------------------
  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    NinSF    [nC] = hNinZeventsZSF   [nC]->GetBinContent(2);
    NinSFDATA[nC] = hNinZeventsDATASF[nC]->GetBinContent(2);    
    NinOFDATA[nC] = hNinZeventsDATAOF[nC]->GetBinContent(2);

    NoutSF    [nC] = hNoutZeventsZSF   [nC]->GetBinContent(2);
    NoutSFDATA[nC] = hNoutZeventsDATASF[nC]->GetBinContent(2);    
    NoutOFDATA[nC] = hNoutZeventsDATAOF[nC]->GetBinContent(2);


    // R estimation
    //--------------------------------------------------------------------------
    R   [nC] = NoutSF[nC] / NinSF[nC]; 
    errR[nC] = errRFunction(NoutSF[nC], NinSF[nC]);

    RDATA   [nC] = RDATAFunction   (NoutSFDATA[nC], NoutOFDATA[nC], NinSFDATA[nC], NinOFDATA[nC], k);
    errRDATA[nC] = errRDATAFunction(NoutSFDATA[nC], NoutOFDATA[nC], NinSFDATA[nC], NinOFDATA[nC], k, errk);


    if (printLevel > 1) {
      printf("\n %.0f < mpmet < %.0f GeV\n", MetCuts[nC], MetCuts[nC+1]);
      printf(" -------------------------------------------------\n");
      printf("         N^{MC}_{out,SF}   = %6.1f\n", NoutSF[nC]);
      printf("         N^{MC}_{in, SF}   = %6.1f\n", NinSF[nC]);
      printf("         N^{data}_{out,SF} = %4.0f\n", NoutSFDATA[nC]);
      printf("         N^{data}_{out,OF} = %4.0f\n", NoutOFDATA[nC]);
      printf("         N^{data}_{in, SF} = %4.0f\n", NinSFDATA[nC]);
      printf("         N^{data}_{in, OF} = %4.0f\n", NinOFDATA[nC]);
      printf("         k                 = % 5.3f +- %5.3f\n", k,         errk);
      printf("         R^{MC}            = % 5.3f +- %5.3f\n", R[nC],     errR[nC]);
      printf("         R^{data}          = % 5.3f +- %5.3f\n", RDATA[nC], errRDATA[nC]);
    }
  }


  // Estimate the R systematic as the maximum difference between the R[mpmet] values
  //----------------------------------------------------------------------------
  Int_t iMaxR = 0; 
  Int_t iMinR = 0; 

  for (UInt_t nC=0; nC<numberMetCuts-1; nC++) {

    if (R[nC] > 0 && R[nC] > R[iMaxR]) iMaxR = nC;
    if (R[nC] > 0 && R[nC] < R[iMinR]) iMinR = nC;
  }

  Double_t RelDiffR = (R[iMaxR] > 0) ? fabs(R[iMaxR] - R[iMinR]) / R[iMaxR] : -999;


  // In the analysis, we use the difference between R[2] and R[3]
  //----------------------------------------------------------------------------
  Int_t theR = 2;

  RelDiffR = (R[theR] > 0) ? fabs(R[theR] - R[numberMetCuts-2]) / R[theR] : -999;


  if (printLevel > 0) {
    printf("\n [%s] R systematic uncertainty\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf("         min R              = %5.3f\n",      R[iMinR]);
    printf("         max R              = %5.3f\n",      R[iMaxR]);
    printf("         R[%d]               = %5.3f\n",     theR, R[theR]);
    printf("         R[%d]               = %5.3f\n",     numberMetCuts-2, R[numberMetCuts-2]);
    printf("         |R[%d]-R[%d]| / R[%d] = %.1f%s\n",  theR, numberMetCuts-2, theR, 1e2*RelDiffR, "%");
    printf("\n");
  }


  // Remove WZ and ZZ dibosons
  //----------------------------------------------------------------------------
  Double_t NinEstSFFinal    = NinCountedSFDATA - k*NinCountedOFDATA;
  Double_t errNinEstSFFinal = errNinEstFunction(NinCountedSFDATA, NinCountedOFDATA, k, errk);

  Double_t NestSFFinal    = R[theR] * NinEstSFFinal;
  Double_t errNestSFFinal = errNestFunction(NestSFFinal, R[theR], errR[theR], NinEstSFFinal, errNinEstSFFinal);

  Double_t NinEstSFNoDibosonFinal    = NinEstSFFinal - NinCountedSFWZ - NinCountedSFZZ;
  Double_t errNinEstSFNoDibosonFinal = errNinEstNoDibosonFunction(NinCountedSFDATA, k, errk, NinCountedOFDATA, NinCountedSFWZ, NinCountedSFZZ);

  Double_t NestSFNoDibosonFinal    = R[theR] * NinEstSFNoDibosonFinal;
  Double_t errNestSFNoDibosonFinal = errNestFunction(NestSFNoDibosonFinal, R[theR], errR[theR], NinEstSFNoDibosonFinal, errNinEstSFNoDibosonFinal);
  Double_t totalError              = sqrt(errNestSFNoDibosonFinal*errNestSFNoDibosonFinal + (RelDiffR*NestSFNoDibosonFinal)*(RelDiffR*NestSFNoDibosonFinal));


  Double_t SFsf = NestSFNoDibosonFinal / hExpectedDYSF->GetBinContent(2);


  if (printLevel > 1) {
    printf("\n Analysis results\n");
    printf(" -------------------------------------------------\n");
    printf("         N^{data}_{in,SF} = %4.0f\n", NinCountedSFDATA);
    printf("         N^{data}_{in,OF} = %4.0f\n", NinCountedOFDATA);
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
  scaleFactor = SFsf;


  // For the note
  //----------------------------------------------------------------------------
  if (printLevel > 0) {
    printf("\n [%s] DY values for the note\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf(" final state   &             R_{MC}  &  N^{control,data}  &     N_{DY}^{data}  &       N_{DY}^{MC}  &  data/MC\n");
    printf(" same flavour  &  %5.3f $\\pm$ %5.3f  &              %4.0f  &  %5.1f $\\pm$ %4.1f  &  %5.1f $\\pm$ %4.1f  &     %4.1f\n\n",
	 R[theR],
	   errR[theR],
	   NinCountedSFDATA,
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

      gRdata->SetPoint     (i, 0.5 * (MetDraw[i+1] + MetDraw[i]),    RDATA[i]);
      gRdata->SetPointError(i, 0.5 * (MetDraw[i+1] - MetDraw[i]), errRDATA[i]);

      if (absoluteMin > (R[i]     - errR[i]))     absoluteMin = R[i]     - errR[i];
      if (absoluteMin > (RDATA[i] - errRDATA[i])) absoluteMin = RDATA[i] - errRDATA[i];
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

    if      (channel == "SFChannel")   lmgR->SetHeader("ee + #mu#mu");
    else if (channel == "EEChannel")   lmgR->SetHeader("ee");
    else if (channel == "MuMuChannel") lmgR->SetHeader("#mu#mu");

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
// RDATAFunction
//------------------------------------------------------------------------------
Double_t RDATAFunction(Double_t NoutSF,
		       Double_t NoutOF,
		       Double_t NinSF,
		       Double_t NinOF,
		       Double_t k)
{
  Double_t R = (NoutSF - k*NoutOF) / (NinSF - k*NinOF);

  return R;
}


//------------------------------------------------------------------------------
// errRDATAFunction
//------------------------------------------------------------------------------
Double_t errRDATAFunction(Double_t NoutSF,
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

  Double_t errRDATA = sqrt(Term1*Term1 + Term2*Term2);

  return errRDATA;
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
