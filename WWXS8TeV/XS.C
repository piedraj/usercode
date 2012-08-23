#include "TFile.h"
#include "TH1F.h"
#include "TInterpreter.h"

#include "DY.C"
#include "Top.C"


const Double_t nlo8tev      = 57.25;
const Double_t nlo8tevPlus  = 4.1 * nlo8tev / 1e2;
const Double_t nlo8tevMinus = 2.8 * nlo8tev / 1e2;

const Double_t ggWW_xs = 0.18;
const Double_t qqWW_xs = 5.81;

const Double_t NTotalggWW =  109987;
const Double_t NTotalqqWW = 1933235;

const Double_t NFiducialggWW =   67977;  // pt > 20, |eta| < 2.5
const Double_t NFiducialqqWW = 1050807;  // pt > 20, |eta| < 2.5


const UInt_t Nsyst = 11;

                                
// systematic[i] = 1e2 * systematic / N
//------------------------------------------------------------------------------
TString  Systematics    [Nsyst] = {"trigger", "lepton", "#mu scale",  "e scale",  "MET", "jet veto", "PU", "Wg", "PDF", "corrections", "sample"};
Double_t SystematicsggWW[Nsyst] = {      1.5,      2.0,         1.5,        2.5,    2.0,        4.7,  2.3,  0.0,   0.8,          30.0,      3.1};
Double_t SystematicsqqWW[Nsyst] = {      1.5,      2.0,         1.5,        2.5,    2.0,        4.7,  2.3,  0.0,   2.3,           1.5,      1.1};
Double_t SystematicsVV  [Nsyst] = {      1.5,      2.0,         1.5,        1.9,    2.0,        4.7,  2.3,  0.0,   5.9,           3.3,      4.1};
Double_t SystematicsWg  [Nsyst] = {      1.5,      2.0,         1.5,        2.0,    2.0,        4.7,  2.3, 30.0,   0.0,           0.0,      8.4};
Double_t SystematicsH125[Nsyst] = {      1.5,      2.0,         1.5,        2.5,    2.0,        4.7,  2.3,  0.0,   2.3,           1.5,      1.1}; // [*]


// [*] Using SystematicsqqWW as a reasonable placeholder


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(Double_t &xsValue,
	Double_t &xsStat,
	Double_t &xsSyst,
	Double_t &xsLumi,
	Double_t  luminosity,
	Int_t     njet,
	TString   channel,
	TString   directory,
	Bool_t    useDataDriven,
	Int_t     printLevel,
	Bool_t    fiducialXS)
{
  TString dyChannel = "SF";

  if (channel.Contains("EE"))   dyChannel = "EE";
  if (channel.Contains("MuMu")) dyChannel = "MuMu";

  Double_t dyScaleFactor       = -999;
  Double_t dyScaleFactorForTop = -999;
  Double_t topScaleFactor      = -999;

  Double_t NDY [3];
  Double_t NTop[3];


  // Needed to always have the SF DY scale factor in the top estimation
  //----------------------------------------------------------------------------
  DY(NDY[0],
     NDY[1],
     NDY[2],
     dyScaleFactorForTop,
     njet,
     "SF",  // "SF" --> dyChannel (individual topScaleFactor)
     directory,
     useDataDriven,
     0,
     false);

  Top(NTop[0],
      NTop[1],
      NTop[2],
      topScaleFactor,
      dyScaleFactorForTop,
      njet,
      "All",  // "All" --> channel (individual topScaleFactor)
      directory,
      false,  // useDataDriven
      printLevel);


  printf("\n >>>>>>> WARNING: Taking top from MC <<<<<<<\n");


  DY(NDY[0],
     NDY[1],
     NDY[2],
     dyScaleFactor,
     njet,
     dyChannel,
     directory,
     useDataDriven,
     printLevel,
     false);


  // Input files
  //----------------------------------------------------------------------------
  TString path = Form("%s/%djet/%s/", directory.Data(), njet, channel.Data());

  TFile* inputWW       = new TFile(path + "WW.root");  
  TFile* inputggWW     = new TFile(path + "ggWWto2L.root");  
  TFile* inputqqWW     = new TFile(path + "WWTo2L2Nu.root");  
  TFile* inputTT       = new TFile(path + "TTbar.root");  
  TFile* inputTW       = new TFile(path + "TW.root");  
  TFile* inputWj       = new TFile(path + "WJetsFakes_Total.root");
  TFile* inputWZ       = new TFile(path + "WZ.root");  
  TFile* inputZZ       = new TFile(path + "ZZ.root");  
  TFile* inputDY       = new TFile(path + "DY.root");  
  TFile* inputDYtautau = new TFile(path + "DYtautau.root");  
  TFile* inputWg       = new TFile(path + "Wgamma.root");  
  TFile* inputH125     = new TFile(path + "HWW125.root");  
  TFile* inputData     = new TFile(path + "DataRun2012_Total.root");

  TFile* inputDYOF = new TFile(Form("%s/%djet/OF/DY.root", directory.Data(), njet));


  //----------------------------------------------------------------------------
  //
  // Estimate WW cross-section
  //
  //----------------------------------------------------------------------------
  TH1F* hNWW       = (TH1F*) inputWW      ->Get("hWTopTagging");
  TH1F* hNggWW     = (TH1F*) inputggWW    ->Get("hWTopTagging");
  TH1F* hNqqWW     = (TH1F*) inputqqWW    ->Get("hWTopTagging");
  TH1F* hNTT       = (TH1F*) inputTT      ->Get("hWTopTagging");
  TH1F* hNTW       = (TH1F*) inputTW      ->Get("hWTopTagging");
  TH1F* hNWj       = (TH1F*) inputWj      ->Get("hWTopTagging");
  TH1F* hNWZ       = (TH1F*) inputWZ      ->Get("hWTopTagging");
  TH1F* hNZZ       = (TH1F*) inputZZ      ->Get("hWTopTagging");
  TH1F* hNDY       = (TH1F*) inputDY      ->Get("hWTopTagging");
  TH1F* hNDYtautau = (TH1F*) inputDYtautau->Get("hWTopTagging");
  TH1F* hNWg       = (TH1F*) inputWg      ->Get("hWTopTagging");
  TH1F* hNH125     = (TH1F*) inputH125    ->Get("hWTopTagging");
  TH1F* hNData     = (TH1F*) inputData    ->Get("hWTopTagging");
  TH1F* hNDYOF     = (TH1F*) inputDYOF    ->Get("hWTopTagging");


  // Yields
  //----------------------------------------------------------------------------
  Double_t NggWW[]     = {hNggWW    ->GetBinContent(2), hNggWW    ->GetBinError(2), 0.00};
  Double_t NqqWW[]     = {hNqqWW    ->GetBinContent(2), hNqqWW    ->GetBinError(2), 0.00};
  Double_t NWW[]       = {hNWW      ->GetBinContent(2), hNWW      ->GetBinError(2), 0.00};
  Double_t NWj[]       = {hNWj      ->GetBinContent(2), hNWj      ->GetBinError(2), 0.36*hNWj->GetBinContent(2)};
  Double_t NWZ[]       = {hNWZ      ->GetBinContent(2), hNWZ      ->GetBinError(2), 0.00};
  Double_t NZZ[]       = {hNZZ      ->GetBinContent(2), hNZZ      ->GetBinError(2), 0.00};
  Double_t NDYtautau[] = {hNDYtautau->GetBinContent(2), hNDYtautau->GetBinError(2), 0.50*hNDYtautau->GetBinContent(2)}; // [*]
  Double_t NWg[]       = {hNWg      ->GetBinContent(2), hNWg      ->GetBinError(2), 0.00};
  Double_t NH125[]     = {hNH125    ->GetBinContent(2), hNH125    ->GetBinError(2), 0.00};
  Double_t NData[]     = {hNData    ->GetBinContent(2), hNData    ->GetBinError(2)};


  // [*] Andrea: a normalization uncertainty of 50% has been added on final estimation of dytautau.


  // Apply Z -> tautau scale factor (1.33 +- 0.266) from TOP-12-007
  //----------------------------------------------------------------------------
  if (channel.Contains("OF")  ||
      channel.Contains("MuE") ||
      channel.Contains("EMu")) {
    
    NDYtautau[0] *= 1.0;  // 1.33;
    NDYtautau[1] *= 1.0;  // 1.33;
  }


  // Top yields for the non-inclusive channels
  //----------------------------------------------------------------------------
  if (!channel.Contains("All")) {

    Double_t NTT[] = {hNTT->GetBinContent(2), hNTT->GetBinError(2), 0.0};
    Double_t NTW[] = {hNTW->GetBinContent(2), hNTW->GetBinError(2), 0.0};

    NTop[2] = NTop[2] * topScaleFactor * (NTT[0] + NTW[0]) / NTop[0];  // syst.
    NTop[0] = topScaleFactor * (NTT[0] + NTW[0]);                      // yield
    NTop[1] = topScaleFactor * sqrt(NTT[1]*NTT[1] + NTW[1]*NTW[1]);    // stat.
  }


  // DY yields for the {OF, EMu, MuE} channels
  //----------------------------------------------------------------------------
  if (channel.Contains("OF")  ||
      channel.Contains("EMu") ||
      channel.Contains("MuE")) {

    NDY[2] = NDY[2] * hNDY->GetBinContent(2) / NDY[0];  // syst.
    NDY[0] = hNDY->GetBinContent(2);                    // yield
    NDY[1] = hNDY->GetBinError(2);                      // stat.
  }


  // DY yield for the All channel
  //----------------------------------------------------------------------------
  if (channel.Contains("All")) {

    NDY[2] = NDY[2] * (NDY[0] + hNDYOF->GetBinContent(2)) / NDY[0];                // syst.
    NDY[0] = NDY[0] + hNDYOF->GetBinContent(2);                                    // yield
    NDY[1] = sqrt(NDY[1]*NDY[1] + hNDYOF->GetBinError(2)*hNDYOF->GetBinError(2));  // stat.
  }


  // Add relative systematic uncertainties
  //----------------------------------------------------------------------------
  for (UInt_t iSyst=0; iSyst<Nsyst; iSyst++) {
    NggWW[2] += (SystematicsggWW[iSyst] * SystematicsggWW[iSyst]);
    NqqWW[2] += (SystematicsqqWW[iSyst] * SystematicsqqWW[iSyst]);
    NWZ  [2] += (SystematicsVV  [iSyst] * SystematicsVV  [iSyst]);
    NZZ  [2] += (SystematicsVV  [iSyst] * SystematicsVV  [iSyst]);
    NWg  [2] += (SystematicsWg  [iSyst] * SystematicsWg  [iSyst]);
    NH125[2] += (SystematicsH125[iSyst] * SystematicsH125[iSyst]);
  }

  NggWW[2] = sqrt(NggWW[2]);  // Relative systematic uncertainty
  NqqWW[2] = sqrt(NqqWW[2]);  // Relative systematic uncertainty

  NWW[2]  = (NggWW[2]*NggWW[0] * NggWW[2]*NggWW[0]);
  NWW[2] += (NqqWW[2]*NqqWW[0] * NqqWW[2]*NqqWW[0]);
  NWW[2]  = sqrt(NWW[2]) / NWW[0];  // Relative systematic uncertainty

  NWZ[2]   = sqrt(NWZ[2])   * NWZ[0]   / 1e2;  // Absolute systematic uncertainty
  NZZ[2]   = sqrt(NZZ[2])   * NZZ[0]   / 1e2;  // Absolute systematic uncertainty
  NWg[2]   = sqrt(NWg[2])   * NWg[0]   / 1e2;  // Absolute systematic uncertainty
  NH125[2] = sqrt(NH125[2]) * NH125[0] / 1e2;  // Absolute systematic uncertainty

  Double_t Background = NWj[0] + NWZ[0] + NZZ[0] + NWg[0] + NTop[0] + NDY[0] + NDYtautau[0] + NH125[0];
  
  Double_t statErrorB = sqrt(NWj      [1]*NWj      [1] +
			     NWZ      [1]*NWZ      [1] +
			     NZZ      [1]*NZZ      [1] +
			     NWg      [1]*NWg      [1] +
			     NTop     [1]*NTop     [1] +
			     NDY      [1]*NDY      [1] +
			     NDYtautau[1]*NDYtautau[1] +
			     NH125    [1]*NH125    [1]);

  Double_t systErrorB = sqrt(NWj      [2]*NWj      [2] +
			     NWZ      [2]*NWZ      [2] +
			     NZZ      [2]*NZZ      [2] +
			     NWg      [2]*NWg      [2] +
			     NTop     [2]*NTop     [2] +
			     NDY      [2]*NDY      [2] +
			     NDYtautau[2]*NDYtautau[2] +
			     NH125    [2]*NH125    [2]);

  Double_t totalErrorB = sqrt(statErrorB*statErrorB + systErrorB*systErrorB);


  //----------------------------------------------------------------------------
  //
  // Estimate WW efficiency
  //
  //----------------------------------------------------------------------------
  TH1F* hgg = (TH1F*) inputggWW->Get("hWeffTopTagging");
  TH1F* hqq = (TH1F*) inputqqWW->Get("hWeffTopTagging");

  Double_t NGenggWW = (fiducialXS) ? NFiducialggWW : NTotalggWW;
  Double_t NGenqqWW = (fiducialXS) ? NFiducialqqWW : NTotalqqWW;

  Double_t f_gg = ggWW_xs / (ggWW_xs + qqWW_xs);
  Double_t f_qq = 1 - f_gg;

  Double_t ggWW_efficiency = ratioValue(hgg->GetBinContent(2), NGenggWW);
  Double_t qqWW_efficiency = ratioValue(hqq->GetBinContent(2), NGenqqWW);
  Double_t   WW_efficiency = f_gg*ggWW_efficiency + f_qq*qqWW_efficiency;

  Double_t ggWW_efficiencyErr = ratioError(hgg->GetBinContent(2), NGenggWW, hgg->GetBinError(2), sqrt(NGenggWW));
  Double_t qqWW_efficiencyErr = ratioError(hqq->GetBinContent(2), NGenqqWW, hqq->GetBinError(2), sqrt(NGenqqWW));

  // Systematic component
  Double_t WW_efficiencyErr = (f_gg*NggWW[2]*NggWW[0] + f_qq*NqqWW[2]*NqqWW[0]) / NWW[0];
  WW_efficiencyErr *= WW_efficiency / 1e2;
  WW_efficiencyErr *= WW_efficiencyErr;

  // Statistical component (negligible)
  WW_efficiencyErr += (f_gg*ggWW_efficiencyErr)*(f_gg*ggWW_efficiencyErr);
  WW_efficiencyErr += (f_qq*qqWW_efficiencyErr)*(f_qq*qqWW_efficiencyErr);

  // Absolute efficiency uncertainty
  WW_efficiencyErr = sqrt(WW_efficiencyErr);


  if (printLevel > 0) {
    printf("\n signal efficiencies\n");
    printf(" -------------------------------------------------\n");
    printf("       ggWW efficiency = (%6.3f +- %5.3f)%s (stat.)\n", 1e2*ggWW_efficiency, 1e2*ggWW_efficiencyErr, "%");
    printf("       qqWW efficiency = (%6.3f +- %5.3f)%s (stat.)\n", 1e2*qqWW_efficiency, 1e2*qqWW_efficiencyErr, "%");
    printf("         WW efficiency = (%6.3f +- %5.3f)%s (total)\n", 1e2*  WW_efficiency, 1e2*  WW_efficiencyErr, "%");
  }


  // Estimate WW cross-section
  //----------------------------------------------------------------------------
  Double_t BR_WW_to_lnln = (3 * 0.108) * (3 * 0.108);

  Double_t xs = (NData[0] - Background) / (luminosity * WW_efficiency);

  if (!fiducialXS) xs /= BR_WW_to_lnln;


  // Relative errors
  //----------------------------------------------------------------------------
  Double_t errxsStats = 1e2 * sqrt(NData[0]) / (NData[0] - Background);
  Double_t errxsBkg   = 1e2 * totalErrorB    / (NData[0] - Background);
  Double_t errxsEff   = 1e2 * WW_efficiencyErr / WW_efficiency;
  Double_t errxsSyst  = sqrt(errxsBkg*errxsBkg + errxsEff*errxsEff);
  Double_t errxsLumi  = 4.4;


  if (printLevel > 0) {
    printf("\n [PAS]");
    printf(" The total uncertainty on the background estimation is about %.0f%s,",
	   1e2 * totalErrorB / Background, "%");
    printf(" [PAS]\n");
  }


  // Save the result
  //----------------------------------------------------------------------------
  xsValue = xs;
  xsStat  = xs * errxsStats / 1e2;
  xsSyst  = xs * errxsSyst  / 1e2;
  xsLumi  = xs * errxsLumi  / 1e2;


  // Print
  //----------------------------------------------------------------------------
  Double_t statErrorVV = sqrt(NWZ[1]*NWZ[1] + NZZ[1]*NZZ[1]);
  Double_t systErrorVV = sqrt(NWZ[2]*NWZ[2] + NZZ[2]*NZZ[2]);

  Double_t statErrorDYAll = sqrt(NDY[1]*NDY[1] + NDYtautau[1]*NDYtautau[1]);
  Double_t systErrorDYAll = sqrt(NDY[2]*NDY[2] + NDYtautau[2]*NDYtautau[2]);

  Double_t statErrorSPlusB = sqrt(NWW[1]*NWW[1]                           + statErrorB*statErrorB);
  Double_t systErrorSPlusB = sqrt((NWW[2]*NWW[0]/1e2)*(NWW[2]*NWW[0]/1e2) + systErrorB*systErrorB);


  if (printLevel > 0) {
    printf("\n [%s] yields\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf("       sample              &  yield $\\pm$ stat. $\\pm$ syst.\n");
    printf("       gg to WW            & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NggWW[0],            NggWW[1],        NggWW[2]*NggWW[0]/1e2);
    printf("       qq to WW            & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NqqWW[0],            NqqWW[1],        NqqWW[2]*NqqWW[0]/1e2);
    printf("       tt+tW               & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NTop[0],             NTop[1],         NTop[2]);
    printf("       W+jets              & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NWj[0],              NWj[1],          NWj[2]);
    printf("       WZ+ZZ               & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NWZ[0]+NZZ[0],       statErrorVV,     systErrorVV);
    printf("       Z/g*                & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f",   NDY[0]+NDYtautau[0], statErrorDYAll,  systErrorDYAll);
    printf(" (%.1f Z/g* + %.1f Z/g* -> tautau)\n",                           NDY[0],              NDYtautau[0]);
    printf("       Wg+Wg*              & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NWg[0],              NWg[1],          NWg[2]);
    printf("       H125                & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NH125[0],            NH125[1],        NH125[2]);
    printf("       total background    & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", Background,          statErrorB,      systErrorB);
    printf("       signal              & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NWW[0],              NWW[1],          NWW[2]*NWW[0]/1e2);
    printf("       signal + background & %6.1f $\\pm$ %5.1f $\\pm$ %5.1f\n", NWW[0]+Background,   statErrorSPlusB, systErrorSPlusB);
    printf("       data                & %4.0f\n",                           NData[0]);

    printf("\n [%s] WW cross-section\n", channel.Data());
    printf(" -------------------------------------------------\n");
  }

  printf("       sigmaWW(%s) = %5.2f +- %5.2f (stat.) +- %5.2f (syst.) +- %5.2f (lumi.) pb\n",
    	 channel.Data(), xsValue, xsStat, xsSyst, xsLumi);


  // Difference wrt. the theoretical value
  //----------------------------------------------------------------------------
  if (channel.Contains("All") && printLevel > 0) {

    Double_t deltaXS    = xsValue - nlo8tev;
    Double_t deltaXSErr = xsStat*xsStat + xsSyst*xsSyst + xsLumi*xsLumi + nlo8tevPlus*nlo8tevPlus;

    deltaXSErr = sqrt(deltaXSErr);

    printf("\n       sigma(NLO) = %.2f + %.2f - %.2f pb\n", nlo8tev, nlo8tevPlus, nlo8tevMinus);
    printf("\n       sigmaWW(%s) - sigma(NLO) = %.2f +- %.2f pb", channel.Data(), deltaXS, deltaXSErr);
    printf(" = (%.0f +- %.0f)\%s of the theoretical value\n\n", 1e2*deltaXS/nlo8tev, 1e2*deltaXSErr/nlo8tev, "%");
  }


  // Fiducial efficiency
  //----------------------------------------------------------------------------
  if (fiducialXS && printLevel > 0) {
    printf("\n pt > 20 GeV and |eta| < 2.5 fiducial efficiency\n");
    printf(" -------------------------------------------------\n");
    printf("       eff(gg to WW) = %.1f %s\n", 1e2 * ratioValue(NFiducialggWW, NTotalggWW), "%");
    printf("       eff(qq to WW) = %.1f %s\n", 1e2 * ratioValue(NFiducialqqWW, NTotalqqWW), "%");
    printf("\n");
  }


  // WW/Z cross-section ratio
  //----------------------------------------------------------------------------
  if (channel.Contains("All") && printLevel > 0) {

    Double_t wwzRatio = xsValue / (3.342e04);

    printf("\n [%s] WW/Z cross-section ratio\n", channel.Data());
    printf(" -------------------------------------------------\n");
    printf("       sigmaWW/sigmaZ = (%.2f +- %.2f) x 10^{-3}\n\n",
	   1e3*wwzRatio,
	   1e3*wwzRatio*9.4/1e2);
  }
}
