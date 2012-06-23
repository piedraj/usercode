#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TTree.h"


const UInt_t nProcesses = 9;

enum {iData, itt, itW, iWW, iWZ, iZZ, iWg, iWj, iZj};

TFile*  input  [nProcesses];
TString process[nProcesses];
Color_t color  [nProcesses];

process[iData] = "DataRun2012_Total";
process[itt]   = "TTbar";
process[itW]   = "TW";
process[iWW]   = "WW";
process[iWZ]   = "WZ";
process[iZZ]   = "ZZ";
process[iWg]   = "Wgamma";
process[iWj]   = "WJetsFakes_Total";
process[iZj]   = "DY";

color[iData] = kBlack;
color[itt]   = kYellow;
color[itW]   = kOrange;
color[iWW]   = kAzure-9;
color[iWZ]   = kAzure-9;
color[iZZ]   = kAzure-9;
color[iWg]   = kAzure-9;
color[iWj]   = kGray+1;
color[iZj]   = kGreen+2;


Double_t luminosity = 3540;
TString  format     = "png";
Bool_t   rescale    = true;


//------------------------------------------------------------------------------
// drawTop
//------------------------------------------------------------------------------
void drawTop(TString channel = "All")
{
  TString path = Form("rootfiles.%.3ffb/0jet/%s/", luminosity/1e3, channel.Data());

  for (UInt_t iP=0; iP<nProcesses; iP++)
    input[iP] = new TFile(path + process[iP] + ".root", "read");

  //  DrawHistogram(hname,                            xtitle,             title,                                 ngroup, precision, units);
  DrawHistogram("hbTagDisNTopTaggedTopControlRegion", "2^{nd} jet TCHE",  "1-jet and 1-btag and low p_{T} btag",  5,     1,         "NULL");
  DrawHistogram("hbTagDisNTopControlRegion",          "2^{nd} jet TCHE",  "1-jet and 1-btag",                     5,     1,         "NULL");
  DrawHistogram("hbTagDisTopTaggedEvents",            "2^{nd} jet TCHE",  "0-jet and btag (IP or #mu)",           5,     1,         "NULL");
  DrawHistogram("h_jetpt1",                           "1^{st} jet p_{T}", "0-jet and btag (IP)",                 20,     0,         "GeV");
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString hname,
		   TString xtitle,
		   TString title,
		   Int_t   ngroup    = -1,
		   Int_t   precision = 1,
		   TString units     = "NULL")
{
  TCanvas* canvas = new TCanvas(hname, hname);
  THStack* hstack = new THStack(hname, hname);

  TH1F* hist[nProcesses];

  for (UInt_t iP=0; iP<nProcesses; iP++) {

    hist[iP] = (TH1F*)input[iP]->Get(hname);
    hist[iP]->SetName(hname + process[iP]);

    if (ngroup > 0) hist[iP]->Rebin(ngroup);

    if (iP == iData) {
      hist[iP]->SetMarkerStyle(kFullCircle);
    }
    else if (iP != itW) {
      hist[iP]->SetFillColor(color[iP]);
      hist[iP]->SetFillStyle(1001);
      hist[iP]->SetLineColor(color[iP]);

      hstack->Add(hist[iP]);
    }
  }

  hist[itt]->Add(hist[itW]);

  
  // Axis labels
  //----------------------------------------------------------------------------
  TAxis* xaxis = hist[iData]->GetXaxis();
  TAxis* yaxis = hist[iData]->GetYaxis();

  TString ytitle = Form("entries / %s.%df", "%", precision);

  xaxis->SetTitle(xtitle);
  yaxis->SetTitle(Form(ytitle.Data(), hist[iData]->GetBinWidth(0)));
  yaxis->SetTitleOffset(1.6);

  if (!units.Contains("NULL")) {
    
    xaxis->SetTitle(Form("%s [%s]", xaxis->GetTitle(), units.Data()));
    yaxis->SetTitle(Form("%s %s",   yaxis->GetTitle(), units.Data()));
  }


  // Rescale top (tt+tW) to data - backgrounds
  //----------------------------------------------------------------------------
  Double_t scaleFactor = 0;

  if (rescale) {
  
    Double_t backgroundIntegral = 0;

    for (UInt_t iP=0; iP<nProcesses; iP++) {

      if (iP == iData || iP == itt || iP == itW) continue;

      backgroundIntegral += hist[iP]->Integral();
    }

    scaleFactor = (hist[iData]->Integral() - backgroundIntegral);

    scaleFactor /= hist[itt]->Integral();

    hist[itt]->Scale(scaleFactor);
  }


  // Draw
  //----------------------------------------------------------------------------
  hist[iData]->Draw("ep");
  hstack->Draw("hist,same");
  hist[iData]->Draw("ep,same");


  // Adjust scale
  //----------------------------------------------------------------------------
  Float_t theMax = GetMaximumIncludingErrors(hist[iData]);

  if (canvas->GetLogy()) {
    hist[iData]->SetMaximum(500 * theMax);
    hist[iData]->SetMinimum(0.05);
  } else {
    hist[iData]->SetMaximum(1.55 * theMax);
  }

  hist[iData]->GetXaxis()->SetNdivisions(510);


  // Legend
  //----------------------------------------------------------------------------
  Double_t yoffset = 0.048;

  TString topTitle = " top";

  if (rescale) topTitle += Form(" #times%.1f", scaleFactor);

  DrawLegend(0.23, 0.74 + 2.*(yoffset+0.001), hist[iData], " data",   "lp", 0.04, 0.2, yoffset);
  DrawLegend(0.23, 0.74 + 1.*(yoffset+0.001), hist[iWW],   " VV",     "f",  0.04, 0.2, yoffset);
  DrawLegend(0.23, 0.74,                      hist[iZj],   " Z+jets", "f",  0.04, 0.2, yoffset);
  DrawLegend(0.41, 0.74 + 1.*(yoffset+0.001), hist[iWj],   " W+jets", "f",  0.04, 0.2, yoffset);
  DrawLegend(0.41, 0.74,                      hist[itt],   topTitle,  "f",  0.04, 0.2, yoffset);

  DrawExtras(title);


  // Save
  //----------------------------------------------------------------------------
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();

  canvas->SaveAs(hname + "." + format);
}


//------------------------------------------------------------------------------
// DrawExtras
//------------------------------------------------------------------------------
void DrawExtras(TString title)
{
  TLatex* latex = new TLatex(0.94, 0.96, title);
  latex->SetNDC();
  latex->SetTextAlign(  32);
  latex->SetTextFont (  42);
  latex->SetTextSize (0.04);
  latex->Draw("same");

  TLatex* preliminary = new TLatex(0.9, 0.85, "CMS preliminary");
  preliminary->SetNDC();
  preliminary->SetTextAlign(  32);
  preliminary->SetTextFont (  42);
  preliminary->SetTextSize (0.04);
  preliminary->Draw("same");

  TLatex* lumi = new TLatex(0.9, 0.81, Form("L = %.3f fb^{-1}", luminosity/1e3));
  lumi->SetNDC();
  lumi->SetTextAlign(  32);
  lumi->SetTextFont (  42);
  lumi->SetTextSize (0.03);
  lumi->Draw("same");
}


//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Float_t GetMaximumIncludingErrors(TH1F* h)
{
  Float_t maxWithErrors = 0;

  for (Int_t i=1; i<=h->GetNbinsX(); i++) {

    Float_t binHeight = h->GetBinContent(i) + h->GetBinError(i);

    if (binHeight > maxWithErrors) maxWithErrors = binHeight;
  }

  return maxWithErrors;
}
