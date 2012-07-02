#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"


const UInt_t nProcesses = 9;

enum {iData, iWW, iWZ, iZZ, iWg, itt, itW, iWj, iZj};

TFile* input[nProcesses];


TString process[nProcesses];

process[iData] = "DataRun2012_Total";
process[itt]   = "TTbar";
process[itW]   = "TW";
process[iWW]   = "WW";
process[iWZ]   = "WZ";
process[iZZ]   = "ZZ";
process[iWg]   = "Wgamma";
process[iWj]   = "WJetsFakes_Total";
process[iZj]   = "DY";


Color_t color[nProcesses];

color[iData] = kBlack;
color[itt]   = kYellow;
color[itW]   = kYellow;
color[iWW]   = kAzure-9;
color[iWZ]   = kAzure-2;
color[iZZ]   = kAzure-2;
color[iWg]   = kAzure-2;
color[iWj]   = kGray+1;
color[iZj]   = kGreen+2;


Double_t systError[nProcesses];

systError[iData] = 0.0;

// syst. / yield from SMP-12-013
systError[itt] = 19.5 / 131.6;
systError[itW] = 19.5 / 131.6;
systError[iWW] = 49.3 / 683.6;
systError[iWZ] =  2.9 /  27.4;
systError[iZZ] =  2.9 /  27.4;
systError[iWg] =  4.3 /  13.6;
systError[iWj] = 21.6 /  60.0;
systError[iZj] =  9.9 /  42.5;


// Settings
//------------------------------------------------------------------------------
TString  _channel;
Int_t    _njet;
Double_t _luminosity;
TString  _format;
Bool_t   _drawRatio;
Bool_t   _dataDriven;
Bool_t   _savePlots;


//------------------------------------------------------------------------------
// drawDistributions
//------------------------------------------------------------------------------
void drawDistributions(TString  channel    = "All",
		       Int_t    njet       = 0,
		       Double_t luminosity = 3540,
		       TString  format     = "pdf",
		       Bool_t   drawRatio  = true,
		       Bool_t   dataDriven = true,
		       Bool_t   savePlots  = true)
{
  _channel    = channel;
  _njet       = njet;
  _luminosity = luminosity;
  _format     = format;
  _drawRatio  = drawRatio;
  _dataDriven = dataDriven;
  _savePlots  = savePlots;
  
  gStyle->SetHatchesLineWidth(1.00);
  gStyle->SetHatchesSpacing  (0.55);


  TString path = Form("rootfiles.%.3ffb/%djet/%s/",
		      _luminosity/1e3, _njet, _channel.Data());

  for (UInt_t ip=0; ip<nProcesses; ip++)
    input[ip] = new TFile(path + process[ip] + ".root", "read");


  // Top
  //----------------------------------------------------------------------------
  if (0) {
    DrawHistogram("hbTagDisNTopTaggedTopControlRegion", "2^{nd} jet TCHE",   5, 1, "NULL", -999, -999, false);
    DrawHistogram("hbTagDisNTopControlRegion",          "2^{nd} jet TCHE",   5, 1, "NULL", -999, -999, false);
    DrawHistogram("hbTagDisTopTaggedEvents",            "2^{nd} jet TCHE",   5, 1, "NULL", -999, -999, false);
    DrawHistogram("h_jetpt1",                           "1^{st} jet p_{T}", 20, 0, "GeV",  -999, -999, false);
  }


  // PAS
  //----------------------------------------------------------------------------
  DrawHistogram("hPtLepton1WWLevel",  "p_{T}^{max}",           5, 0, "GeV",  0, 160);
  DrawHistogram("hPtLepton2WWLevel",  "p_{T}^{min}",           5, 0, "GeV", 15,  80);
  DrawHistogram("hPtDiLeptonWWLevel", "p_{T}^{#font[12]{ll}}", 5, 0, "GeV", 40, 120);
  DrawHistogram("hMinvWWLevel",       "m_{#font[12]{ll}}",     5, 0, "GeV");
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   TString  xtitle,
		   Int_t    ngroup       = -1,
		   Int_t    precision    = 1,
		   TString  units        = "NULL",
		   Double_t xmin         = -999,
		   Double_t xmax         =  999,
		   Bool_t   moveOverflow = true)
{
  TCanvas* canvas;

  TPad* pad1;
  TPad* pad2;

  if (_drawRatio) {
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

    if (moveOverflow) MoveOverflowBins(hist[ip], xmin, xmax);

    if (ngroup > 0) hist[ip]->Rebin(ngroup);

    if (ip == iData) {
      hist[ip]->SetMarkerStyle(kFullCircle);
    }
    else {
      hist[ip]->SetFillColor(color[ip]);
      hist[ip]->SetFillStyle(1001);
      hist[ip]->SetLineColor(color[ip]);

      if (_dataDriven && ip == itt) hist[ip]->Scale(1.1);
      if (_dataDriven && ip == itW) hist[ip]->Scale(1.1);
      if (_dataDriven && ip == iWW) hist[ip]->Scale(1.2);
      if (_dataDriven && ip == iZj) hist[ip]->Scale(4.0);

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

      if (_dataDriven)
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
  //  allmc      ->Draw("e2,same");
  hist[iData]->Draw("ep,same");


  // Adjust scale
  //----------------------------------------------------------------------------
  Float_t theMax   = GetMaximumIncludingErrors(hist[iData]);
  Float_t theMaxMC = GetMaximumIncludingErrors(allmc);

  if (theMaxMC > theMax) theMax = theMaxMC;

  if (canvas->GetLogy()) {
    hist[iData]->SetMaximum(500 * theMax);
    hist[iData]->SetMinimum(0.05);
  } else {
    hist[iData]->SetMaximum(1.55 * theMax);
  }


  // Legend
  //----------------------------------------------------------------------------
  Double_t yoffset = (_drawRatio) ? 0.054 : 0.048;
  Double_t x0      = (_drawRatio) ? 0.370 : 0.400; 

  TString allmcTitle = (_dataDriven) ? " stat #oplus syst" : " #sigma_{stat}";

  DrawLegend(0.23, 0.74 + 2.*(yoffset+0.001), hist[iData], " data",    "lp", 0.035, 0.2, yoffset);
  DrawLegend(0.23, 0.74 + 1.*(yoffset+0.001), hist[iWW],   " WW",      "f",  0.035, 0.2, yoffset);
  DrawLegend(0.23, 0.74,                      hist[iWZ],   " VV",      "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74 + 2.*(yoffset+0.001), hist[iZj],   " Z+jets",  "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74 + 1.*(yoffset+0.001), hist[iWj],   " W+jets",  "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74,                      hist[itt],   " top",     "f",  0.035, 0.2, yoffset);
  DrawLegend(  x0, 0.74 - 1.*(yoffset+0.001), allmc,       allmcTitle, "f",  0.035, 0.2, yoffset);


  // Additional titles
  //----------------------------------------------------------------------------
  Double_t deltaY = (_drawRatio) ? 0.02 : 0.0;

  DrawTLatex(0.9, 0.860 + deltaY, 0.04, "CMS preliminary");
  DrawTLatex(0.9, 0.815 + deltaY, 0.03, Form("L = %.3f fb^{-1}", _luminosity/1e3));

  if (_dataDriven) {
    DrawTLatex(0.9, 0.770 + deltaY, 0.03, "data-driven normalization");
  }


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  if (_drawRatio) {

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

    Pad2TAxis(uncertainty, hist[iData]->GetXaxis()->GetTitle(), "data / prediction");
  }


  // Save
  //----------------------------------------------------------------------------
  pad1->cd();
  pad1->GetFrame()->DrawClone();
  pad1->RedrawAxis();

  if (_drawRatio) {
    pad2->cd();
    pad2->GetFrame()->DrawClone();
    pad2->RedrawAxis();
  }

  if (_savePlots) {
    canvas->cd();
    canvas->SaveAs(Form("%s_%djet.%s", hname.Data(), _njet, _format.Data()));
  }
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
void MoveOverflowBins(TH1* h, Double_t xmin, Double_t xmax) const
{
  UInt_t nbins = h->GetNbinsX();

  TAxis* axis = (TAxis*)h->GetXaxis();
  
  Int_t firstBin = (xmin > -999) ? axis->FindBin(xmin) : 1;
  Int_t lastBin  = (xmax <  999) ? axis->FindBin(xmax) : nbins;

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
