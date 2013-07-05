#include "DrawFunctions.h"


const UInt_t nEcm = 2;

enum {ecm7TeV, ecm8TeV};

TString sEcm[nEcm] = {"7", "8"};


const UInt_t nCharge = 3;

enum {WInclusive, WPlus, WMinus};

TString sCharge[nCharge] = {"WInclusive", "WPlus", "WMinus"};


const UInt_t nChannel = 5;

enum {EEE, EEM, MME, MMM, all};

TString sChannel[nChannel] = {"EEE", "EEM", "MME", "MMM", "combined"};

TString lChannel[nChannel] = {"eee", "ee#mu", "#mu#mue", "#mu#mu#mu", "combined"};


Double_t xs_nlo      [nEcm][nCharge] = {{17.8, 11.39, 6.40}, {21.91, 13.86, 8.04}};  // pb
Double_t xs_nlo_left [nEcm][nCharge] = {{ 0.5,  0.49, 0.27}, { 0.88,  0.55, 0.32}};  // pb
Double_t xs_nlo_right[nEcm][nCharge] = {{ 0.7,  0.60, 0.37}, { 1.17,  0.73, 0.44}};  // pb


Double_t xs_value[nCharge][nChannel];  // pb
Double_t xs_stat [nCharge][nChannel];  // pb
Double_t xs_syst [nCharge][nChannel];  // pb
Double_t xs_lumi [nCharge][nChannel];  // pb


Double_t luminosity[nEcm] = {4920., 19602.0};  // pb-1


const Float_t _bigLabelSize = 0.04;


//------------------------------------------------------------------------------
//
// drawXS
//
//------------------------------------------------------------------------------
void drawXS(UInt_t theEcm    = ecm7TeV,
	    UInt_t theCharge = WInclusive)
{
  gStyle->SetEndErrorSize(5);

  if (theEcm == ecm7TeV)
    {
      // 7 TeV inclusive
      //------------------------------------------------------------------------
      xs_value[WInclusive][EEE] = 23.00;
      xs_value[WInclusive][EEM] = 19.67;
      xs_value[WInclusive][MME] = 19.81;
      xs_value[WInclusive][MMM] = 21.02;
      xs_value[WInclusive][all] = 20.76;
      
      xs_stat[WInclusive][EEE] = 3.10;
      xs_stat[WInclusive][EEM] = 2.73;
      xs_stat[WInclusive][MME] = 2.60;
      xs_stat[WInclusive][MMM] = 2.30;
      xs_stat[WInclusive][all] = 1.32;

      xs_syst[WInclusive][EEE] = 1.39;
      xs_syst[WInclusive][EEM] = 1.50;
      xs_syst[WInclusive][MME] = 1.55;
      xs_syst[WInclusive][MMM] = 1.47;
      xs_syst[WInclusive][all] = 1.13;
      
      xs_lumi[WInclusive][EEE] = 0.51;
      xs_lumi[WInclusive][EEM] = 0.43;
      xs_lumi[WInclusive][MME] = 0.44;
      xs_lumi[WInclusive][MMM] = 0.46;
      xs_lumi[WInclusive][all] = 0.46;


      // 7 TeV W+
      //------------------------------------------------------------------------
      xs_value[WPlus][EEE] = 13.39;
      xs_value[WPlus][EEM] = 13.18;
      xs_value[WPlus][MME] = 14.14;
      xs_value[WPlus][MMM] = 11.43;
      xs_value[WPlus][all] = 12.73;

      xs_stat[WPlus][EEE] = 2.39;
      xs_stat[WPlus][EEM] = 2.24;
      xs_stat[WPlus][MME] = 2.19;
      xs_stat[WPlus][MMM] = 1.71;
      xs_stat[WPlus][all] = 1.04;

      xs_syst[WPlus][EEE] = 0.75;
      xs_syst[WPlus][EEM] = 0.64;
      xs_syst[WPlus][MME] = 0.74;
      xs_syst[WPlus][MMM] = 0.53;
      xs_syst[WPlus][all] = 0.59;

      xs_lumi[WPlus][EEE] = 0.29;
      xs_lumi[WPlus][EEM] = 0.29;
      xs_lumi[WPlus][MME] = 0.31;
      xs_lumi[WPlus][MMM] = 0.25;
      xs_lumi[WPlus][all] = 0.28;


      // 7 TeV W-
      //------------------------------------------------------------------------
      xs_value[WMinus][EEE] = 9.49;
      xs_value[WMinus][EEM] = 6.51;
      xs_value[WMinus][MME] = 5.73;
      xs_value[WMinus][MMM] = 9.48;
      xs_value[WMinus][all] = 7.46;

      xs_stat[WMinus][EEE] = 1.95;
      xs_stat[WMinus][EEM] = 1.58;
      xs_stat[WMinus][MME] = 1.40;
      xs_stat[WMinus][MMM] = 1.52;
      xs_stat[WMinus][all] = 0.79;

      xs_syst[WMinus][EEE] = 0.60;
      xs_syst[WMinus][EEM] = 0.37;
      xs_syst[WMinus][MME] = 0.37;
      xs_syst[WMinus][MMM] = 0.50;
      xs_syst[WMinus][all] = 0.40;

      xs_lumi[WMinus][EEE] = 0.21;
      xs_lumi[WMinus][EEM] = 0.14;
      xs_lumi[WMinus][MME] = 0.13;
      xs_lumi[WMinus][MMM] = 0.21;
      xs_lumi[WMinus][all] = 0.16;

    }
  else if (theEcm == ecm8TeV)
    {
      // 8 TeV inclusive
      //------------------------------------------------------------------------
      xs_value[WInclusive][EEE] = 24.92;
      xs_value[WInclusive][EEM] = 23.42;
      xs_value[WInclusive][MME] = 24.40;
      xs_value[WInclusive][MMM] = 25.71;
      xs_value[WInclusive][all] = 24.68;

      xs_stat[WInclusive][EEE] = 1.83;
      xs_stat[WInclusive][EEM] = 1.59;
      xs_stat[WInclusive][MME] = 1.46;
      xs_stat[WInclusive][MMM] = 1.27;
      xs_stat[WInclusive][all] = 0.76;

      xs_syst[WInclusive][EEE] = 1.27;
      xs_syst[WInclusive][EEM] = 1.12;
      xs_syst[WInclusive][MME] = 1.33;
      xs_syst[WInclusive][MMM] = 1.23;
      xs_syst[WInclusive][all] = 1.18;
      
      xs_lumi[WInclusive][EEE] = 1.10;
      xs_lumi[WInclusive][EEM] = 1.03;
      xs_lumi[WInclusive][MME] = 1.07;
      xs_lumi[WInclusive][MMM] = 1.13;
      xs_lumi[WInclusive][all] = 1.09;


      // 8 TeV W+
      //------------------------------------------------------------------------
      xs_value[WPlus][EEE] = 15.27;
      xs_value[WPlus][EEM] = 15.35;
      xs_value[WPlus][MME] = 16.46;
      xs_value[WPlus][MMM] = 16.55;
      xs_value[WPlus][all] = 15.98;

      xs_stat[WPlus][EEE] = 1.43;
      xs_stat[WPlus][EEM] = 1.28;
      xs_stat[WPlus][MME] = 1.19;
      xs_stat[WPlus][MMM] = 1.02;
      xs_stat[WPlus][all] = 0.61;

      xs_syst[WPlus][EEE] = 0.66;
      xs_syst[WPlus][EEM] = 0.59;
      xs_syst[WPlus][MME] = 0.75;
      xs_syst[WPlus][MMM] = 0.73;
      xs_syst[WPlus][all] = 0.65;

      xs_lumi[WPlus][EEE] = 0.67;
      xs_lumi[WPlus][EEM] = 0.68;
      xs_lumi[WPlus][MME] = 0.72;
      xs_lumi[WPlus][MMM] = 0.73;
      xs_lumi[WPlus][all] = 0.70;


      // 8 TeV W-
      //------------------------------------------------------------------------
      xs_value[WMinus][EEE] = 9.63;
      xs_value[WMinus][EEM] = 8.11;
      xs_value[WMinus][MME] = 8.01;
      xs_value[WMinus][MMM] = 9.18;
      xs_value[WMinus][all] = 8.68;

      xs_stat[WMinus][EEE] = 1.13;
      xs_stat[WMinus][EEM] = 0.95;
      xs_stat[WMinus][MME] = 0.85;
      xs_stat[WMinus][MMM] = 0.77;
      xs_stat[WMinus][all] = 0.45;

      xs_syst[WMinus][EEE] = 0.46;
      xs_syst[WMinus][EEM] = 0.38;
      xs_syst[WMinus][MME] = 0.43;
      xs_syst[WMinus][MMM] = 0.41;
      xs_syst[WMinus][all] = 0.39;

      xs_lumi[WMinus][EEE] = 0.42;
      xs_lumi[WMinus][EEM] = 0.36;
      xs_lumi[WMinus][MME] = 0.35;
      xs_lumi[WMinus][MMM] = 0.40;
      xs_lumi[WMinus][all] = 0.38;
    }


  // Do the work
  //----------------------------------------------------------------------------
  TGraphErrors* gStat = new TGraphErrors(nChannel);
  TGraphErrors* gSyst = new TGraphErrors(nChannel);
  TGraphErrors* gLumi = new TGraphErrors(nChannel);

  for (UInt_t i=0; i<nChannel; i++)
    {
      Double_t errorSquared = (xs_stat[theCharge][i] * xs_stat[theCharge][i]);

      gStat->SetPointError(i, sqrt(errorSquared) / xs_nlo[theEcm][theCharge], 0.0);
      
      errorSquared += (xs_syst[theCharge][i] * xs_syst[theCharge][i]);

      gSyst->SetPointError(i, sqrt(errorSquared) / xs_nlo[theEcm][theCharge], 0.0);

      errorSquared += (xs_lumi[theCharge][i] * xs_lumi[theCharge][i]);

      gLumi->SetPointError(i, sqrt(errorSquared) / xs_nlo[theEcm][theCharge], 0.0);

      gStat->SetPoint(i, xs_value[theCharge][i] / xs_nlo[theEcm][theCharge], nChannel-i-1);
      gSyst->SetPoint(i, xs_value[theCharge][i] / xs_nlo[theEcm][theCharge], nChannel-i-1);
      gLumi->SetPoint(i, xs_value[theCharge][i] / xs_nlo[theEcm][theCharge], nChannel-i-1);
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
  TString suffix = sEcm[theEcm] + "TeV_" + sCharge[theCharge];

  TCanvas* canvas = new TCanvas("ratioNLO_" + suffix, "ratioNLO_" + suffix);

  canvas->SetLeftMargin(canvas->GetRightMargin());

  Double_t xmin =  0.0;
  Double_t xmax =  2.0;
  Double_t ymin = -0.5;
  Double_t ymax = nChannel - 0.5;
  
  TH2F* h2 = new TH2F("h2_" + suffix, "", 100, xmin, xmax, 100, ymin, ymax);

  h2->Draw();
  
  
  // NLO WZ cross-section
  //----------------------------------------------------------------------------
  TBox* nlo = new TBox(1. - xs_nlo_left [theEcm][theCharge] / xs_nlo[theEcm][theCharge], ymin,
		       1. + xs_nlo_right[theEcm][theCharge] / xs_nlo[theEcm][theCharge], ymax);

  nlo->SetLineColor(0);
  nlo->SetFillColor(kGray);
  nlo->SetFillStyle(1001);

  TLine* line = new TLine(1., ymin, 1., ymax);

  line->SetLineColor(kGray+3);
  line->SetLineStyle(3);
  line->SetLineWidth(3);

  nlo ->Draw("e2,same");
  line->Draw("same");


  // Cross sections
  //----------------------------------------------------------------------------
  gLumi->Draw("p,same");
  gSyst->Draw("p,same");
  gStat->Draw("p,same");


  // Labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {

    Double_t x = gStat->GetX()[i];
    Double_t y = gStat->GetY()[i];

    Double_t gStatError  = gStat->GetErrorX(i);
    Double_t gSystError  = gSyst->GetErrorX(i);
    Double_t gLumiError  = gLumi->GetErrorX(i);

    DrawTLatex(xmin+0.06, y+0.15, 0.035, 12,
	       Form("%s %.2f #pm %.2f",
		    lChannel[i].Data(), x, gLumiError), 0);

    gLumiError = sqrt(gLumiError*gLumiError - gSystError*gSystError);
    gSystError = sqrt(gSystError*gSystError - gStatError*gStatError);

    DrawTLatex(xmin+0.06, y-0.15, 0.025, 12,
	       Form("%.2f #pm %.2f #pm %.2f #pm %.2f",
		    x, gStatError, gSystError, gLumiError), 0);
  }

  DrawTLatex(0.050, 0.975, _bigLabelSize, 13, "CMS Preliminary");

  DrawTLatex(0.940, 0.983, _bigLabelSize, 33, 
	     Form("#sqrt{s} = %s TeV, L = %.1f fb^{-1}",
		  sEcm[theEcm].Data(), luminosity[theEcm]/1e3));

  TString swz = "";

  if      (theCharge == WPlus)  swz = "W^{+}Z";
  else if (theCharge == WMinus) swz = "W^{-}Z";
  else                          swz = "W^{#pm}Z";
  
  h2->GetXaxis()->CenterTitle();
  h2->GetXaxis()->SetTitleOffset(1.4);
  h2->GetXaxis()->SetTitle(Form("#sigma_{%s}^{exp} / #sigma_{%s}^{theo}",
				   swz.Data(),
				   swz.Data()));
  h2->GetYaxis()->SetTitle("");


  // Remove y-axis labels
  //----------------------------------------------------------------------------
  TAxis* yaxis = h2->GetYaxis();
  
  for (Int_t j=1; j<yaxis->GetNbins(); j++) yaxis->SetBinLabel(j, "");


  // Save
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();

  canvas->SaveAs(Form("pdf/ratioNLO_%s.pdf", suffix.Data()));
  canvas->SaveAs(Form("png/ratioNLO_%s.png", suffix.Data()));
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