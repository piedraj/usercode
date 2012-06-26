#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"


const UInt_t nProcesses = 9;

enum {iData, iWW, iWZ, iZZ, iWg, itt, itW, iWj, iZj};

TFile*  input     [nProcesses];
TString process   [nProcesses];
Color_t color     [nProcesses];
Double_t systError[nProcesses];

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
color[itW]   = kYellow;
color[iWW]   = kAzure-9;
color[iWZ]   = kAzure-2;
color[iZZ]   = kAzure-2;
color[iWg]   = kAzure-2;
color[iWj]   = kGray+1;
color[iZj]   = kGreen+2;

systError[iData] = 0.0;
systError[itt]   = 0.18;
systError[itW]   = 0.18;
systError[iWW]   = 0.078;
systError[iWZ]   = 0.094;
systError[iZZ]   = 0.094;
systError[iWg]   = 0.30;
systError[iWj]   = 0.36;
systError[iZj]   = 0.50;


Double_t luminosity = 3540;
TString  format     = "pdf";
Bool_t   drawRatio  = true;
Bool_t   dataDriven = true;


//------------------------------------------------------------------------------
// drawDistributions
//------------------------------------------------------------------------------
void drawDistributions(TString channel = "All")
{
  gStyle->SetHatchesLineWidth(1.00);
  gStyle->SetHatchesSpacing  (0.55);


  TString path = Form("rootfiles.%.3ffb/0jet/%s/", luminosity/1e3, channel.Data());

  for (UInt_t ip=0; ip<nProcesses; ip++)
    input[ip] = new TFile(path + process[ip] + ".root", "read");


  // Top
  //----------------------------------------------------------------------------
  if (0) {
    DrawHistogram("hbTagDisNTopTaggedTopControlRegion", "2^{nd} jet TCHE",   5, 1, "NULL");
    DrawHistogram("hbTagDisNTopControlRegion",          "2^{nd} jet TCHE",   5, 1, "NULL");
    DrawHistogram("hbTagDisTopTaggedEvents",            "2^{nd} jet TCHE",   5, 1, "NULL");
    DrawHistogram("h_jetpt1",                           "1^{st} jet p_{T}", 20, 0, "GeV");
  }


  // PAS
  //----------------------------------------------------------------------------
  DrawHistogram("hPtLepton1WWLevel",  "p_{T}^{max}", 5, 0, "GeV", 0, 160);
  DrawHistogram("hPtLepton2WWLevel",  "p_{T}^{min}", 5, 0, "GeV", 0,  80);
  DrawHistogram("hPtDiLeptonWWLevel", "p_{T}^{ll}",  5, 0, "GeV", 0, 120);
  DrawHistogram("hMinvWWLevel",       "m_{ll}",      5, 0, "GeV");
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   TString  xtitle,
		   Int_t    ngroup    = -1,
		   Int_t    precision = 1,
		   TString  units     = "NULL",
		   Double_t xmin      = -999,
		   Double_t xmax      = -999)
{
  TCanvas* canvas;

  TPad* pad1;
  TPad* pad2;

  if (drawRatio) {
    canvas = new TCanvas(hname, hname, 550, 1.2*600);

    pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin   (0.05);
    pad1->SetBottomMargin(0.02);
    pad1->Draw();

    pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3); 
    pad2->SetTopMargin   (0.08);
    pad2->SetBottomMargin(0.35);
    pad2->Draw();
  }
  else {
    canvas = new TCanvas(hname, hname, 550, 600);
    pad1 = (TPad*)canvas->GetPad(0);
  }


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();

  THStack* hstack = new THStack(hname, hname);

  TH1F* hist[nProcesses];

  for (UInt_t ip=0; ip<nProcesses; ip++) {

    hist[ip] = (TH1F*)input[ip]->Get(hname);
    hist[ip]->SetName(hname + process[ip]);

    MoveOverflowBins(hist[ip]);

    if (ngroup > 0) hist[ip]->Rebin(ngroup);

    if (ip == iData) {
      hist[ip]->SetMarkerStyle(kFullCircle);
    }
    else {
      hist[ip]->SetFillColor(color[ip]);
      hist[ip]->SetFillStyle(1001);
      hist[ip]->SetLineColor(color[ip]);

      if (dataDriven && ip == itt) hist[ip]->Scale(1.1);
      if (dataDriven && ip == itW) hist[ip]->Scale(1.1);
      if (dataDriven && ip == iWW) hist[ip]->Scale(1.2);
      if (dataDriven && ip == iZj) hist[ip]->Scale(4.5);

      hstack->Add(hist[ip]);
    }
  }


  // All MC
  //----------------------------------------------------------------------------
  TH1F* allmc = (TH1F*)hist[iData]->Clone("allmc");

  allmc->SetFillColor  (kRed+2);
  allmc->SetFillStyle  (  3354);
  allmc->SetLineColor  (kRed+2);
  allmc->SetMarkerColor(kRed+2);
  allmc->SetMarkerSize (     0);

  for (UInt_t ibin=1; ibin<=allmc->GetNbinsX(); ibin++) {

    Double_t binValue = 0;
    Double_t binError = 0;

    for (UInt_t ip=0; ip<nProcesses; ip++) {

      if (ip == iData) continue;

      Double_t binContent = hist[ip]->GetBinContent(ibin);
      
      binValue += binContent;
      binError += (hist[ip]->GetBinError(ibin) * hist[ip]->GetBinError(ibin));

      if (dataDriven)
	binError += (systError[ip]*binContent * systError[ip]*binContent);
    }
    
    binError = sqrt(binError);

    allmc->SetBinContent(ibin, binValue);
    allmc->SetBinError  (ibin, binError);
  }


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


  // Draw
  //----------------------------------------------------------------------------
  if (xmin > -999 && xmax > -999) xaxis->SetRangeUser(xmin, xmax);

  hist[iData]->Draw("ep");
  hstack     ->Draw("hist,same");
  allmc      ->Draw("e2,same");
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


  // Legend
  //----------------------------------------------------------------------------
  Double_t yoffset = (drawRatio) ? 0.054 : 0.048;
  Double_t x0      = (drawRatio) ? 0.370 : 0.400; 

  TString allmcTitle = (dataDriven) ? " stat #oplus syst" : " #sigma_{stat}";

  DrawLegend(0.23, 0.74 + 2.*(yoffset+0.001), hist[iData], " data",    "lp", 0.035, 0.2, yoffset);
  DrawLegend(0.23, 0.74 + 1.*(yoffset+0.001), hist[iWW],   " WW",      "f",  0.035, 0.2, yoffset);
  DrawLegend(0.23, 0.74,                      hist[iWZ],   " VV",      "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74 + 2.*(yoffset+0.001), hist[iZj],   " Z+jets",  "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74 + 1.*(yoffset+0.001), hist[iWj],   " W+jets",  "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74,                      hist[itt],   " top",     "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74 - 1.*(yoffset+0.001), allmc,       allmcTitle, "f",  0.035, 0.2, yoffset);


  // Additional titles
  //----------------------------------------------------------------------------
  Double_t deltaY = (drawRatio) ? 0.02 : 0.0;

  DrawTLatex(0.9, 0.860 + deltaY, 0.04, "CMS preliminary");
  DrawTLatex(0.9, 0.815 + deltaY, 0.03, Form("L = %.3f fb^{-1}", luminosity/1e3));
  DrawTLatex(0.9, 0.770 + deltaY, 0.03, "measured WW cross section");


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  if (drawRatio) {

    pad2->cd();
    
    TH1F* ratio       = hist[iData]->Clone("ratio");
    TH1F* uncertainty = allmc->Clone("uncertainty");

    for (UInt_t ibin=1; ibin<=ratio->GetNbinsX(); ibin++) {

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
    
    if (xmin > -999 && xmax > -999) uaxis->SetRangeUser(xmin, xmax);
    
    
    uncertainty->Draw("e2");
    ratio      ->Draw("ep,same");

    uncertainty->GetYaxis()->SetRangeUser(0, 2);

    Pad2TAxis(uncertainty, hist[iData]->GetXaxis()->GetTitle(), "data / MC");
  }


  // Save
  //----------------------------------------------------------------------------
  pad1->cd();
  pad1->GetFrame()->DrawClone();
  pad1->RedrawAxis();

  if (drawRatio) {
    pad2->cd();
    pad2->GetFrame()->DrawClone();
    pad2->RedrawAxis();
  }

  canvas->cd();
  canvas->SaveAs(hname + "." + format);
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


//------------------------------------------------------------------------------
// MoveOverflowBins
//------------------------------------------------------------------------------
void MoveOverflowBins(TH1* h) const
{
  UInt_t nbinsx = h->GetNbinsX();
  UInt_t nbinsy = h->GetNbinsY();

  Double_t underVal, underErr;
  Double_t firstVal, firstErr;
  Double_t overVal,  overErr;
  Double_t lastVal,  lastErr;
 
  for (UInt_t i=1; i<=nbinsy; ++i) {

    // Get underflow
    underVal = h->GetBinContent(0, i);
    underErr = h->GetBinError  (0, i);

    // Get first bin
    firstVal = h->GetBinContent(1, i);
    firstErr = h->GetBinError  (1, i);

    // Zero underflow
    h->SetBinContent(0, i, 0);
    h->SetBinError  (0, i, 0);

    // Set first bin
    h->SetBinContent(1, i, (underVal+firstVal));
    h->SetBinError  (1, i, (sqrt(underErr*underErr + firstErr*firstErr)));

    // Get overflow
    overVal = h->GetBinContent(nbinsx+1, i);
    overErr = h->GetBinError  (nbinsx+1, i);

    // Get last bin
    lastVal = h->GetBinContent(nbinsx, i);
    lastErr = h->GetBinError  (nbinsx, i);

    // Zero overflow
    h->SetBinContent(nbinsx+1, i, 0);
    h->SetBinError  (nbinsx+1, i, 0);

    // Set last bin
    h->SetBinContent(nbinsx, i, (lastVal+overVal));
    h->SetBinError  (nbinsx, i, (sqrt(lastErr*lastErr + overErr*overErr)));
  }
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Double_t x, Double_t y, Double_t tsize, const char* text)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC();
  tl->SetTextAlign(   32);
  tl->SetTextFont (   42);
  tl->SetTextSize (tsize);

  tl->Draw("same");
}


//------------------------------------------------------------------------------
// Pad2TAxis
//------------------------------------------------------------------------------
void Pad2TAxis(TH1* hist, TString xtitle, TString ytitle)
{
  TAxis* xaxis = (TAxis*)hist->GetXaxis();
  TAxis* yaxis = (TAxis*)hist->GetYaxis();

  xaxis->SetLabelFont  (    42);
  xaxis->SetLabelOffset( 0.025);
  xaxis->SetLabelSize  (   0.1);
  xaxis->SetNdivisions (   505);
  xaxis->SetTitle      (xtitle);
  xaxis->SetTitleFont  (    42);
  xaxis->SetTitleOffset(  1.35);
  xaxis->SetTitleSize  (  0.11);
  
  yaxis->CenterTitle   (      );
  yaxis->SetLabelFont  (    42);
  yaxis->SetLabelOffset(  0.02);
  yaxis->SetLabelSize  (   0.1);
  yaxis->SetNdivisions (   505);
  yaxis->SetTitle      (ytitle);
  yaxis->SetTitleFont  (    42);
  yaxis->SetTitleOffset(  0.75);
  yaxis->SetTitleSize  (  0.11);
}
