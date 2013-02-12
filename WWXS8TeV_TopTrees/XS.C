#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "THStack.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"
#include <vector>


// Input parameters for the WW cross section
//------------------------------------------------------------------------------
const Double_t xsWW_nlo  = 57.25;  // pb (arXiv:1105.0020v1)
const Double_t WW2lnln   = (3 * 0.108) * (3 * 0.108);

const Double_t ngen_qqWW = 1933235;
const Double_t ngen_ggWW = 109987;

const Double_t f_qqWW = 0.97;
const Double_t f_ggWW = 0.03;

const UInt_t nChannels = 4;

enum {MuMu, EE, EMu, MuE};

TString sChannel[] = {"MuMu", "EE", "EMu", "MuE"};


const UInt_t nCuts = 2;

enum {TwoLepton, WWLevel};

TString sCut[] = {"TwoLepton", "WWLevel"};


const UInt_t nProcesses = 11;

enum {
  Data,
  WWTo2L2Nu_Madgraph,
  ggWWto2L,
  ZZ,
  WgammaToLNuG,
  TW,
  TbarW,
  TTbar_Madgraph,
  WJets_Madgraph,
  ZJets_Madgraph,
  ZgammaToLLG
};


TFile* input[nProcesses];


TString process[nProcesses];

process[WWTo2L2Nu_Madgraph] = "WWTo2L2Nu_Madgraph";
process[ggWWto2L]           = "ggWWto2L";
process[ZZ]                 = "ZZ";
process[WgammaToLNuG]       = "WgammaToLNuG";
process[TW]                 = "TW";
process[TbarW]              = "TbarW";
process[TTbar_Madgraph]     = "TTbar_Madgraph";
process[WJets_Madgraph]     = "WJets_Madgraph";
process[ZJets_Madgraph]     = "ZJets_Madgraph";
process[ZgammaToLLG]        = "ZgammaToLLG";


Color_t color[nProcesses];

color[Data]               = kBlack;
color[WWTo2L2Nu_Madgraph] = kAzure-9;
color[ggWWto2L]           = kAzure-9;
color[ZZ]                 = kAzure-2;
color[WgammaToLNuG]       = kAzure-2;
color[TW]                 = kYellow;
color[TbarW]              = kYellow;
color[TTbar_Madgraph]     = kYellow;
color[WJets_Madgraph]     = kGray+1;
color[ZJets_Madgraph]     = kGreen+2;
color[ZgammaToLLG]        = kGreen+2;


Double_t systError[nProcesses];

systError[Data]               = 0.0;
systError[WWTo2L2Nu_Madgraph] = 0.0 / 1e2;
systError[ggWWto2L]           = 0.0 / 1e2;
systError[ZZ]                 = 0.0 / 1e2;
systError[WgammaToLNuG]       = 0.0 / 1e2;
systError[TW]                 = 0.0 / 1e2;
systError[TbarW]              = 0.0 / 1e2;
systError[TTbar_Madgraph]     = 0.0 / 1e2;
systError[WJets_Madgraph]     = 0.0 / 1e2;
systError[ZJets_Madgraph]     = 0.0 / 1e2;
systError[ZgammaToLLG]        = 0.0 / 1e2;


// Settings
//------------------------------------------------------------------------------
Bool_t   _setLogy    = false;
Double_t _luminosity = 19468.3;  // pb
Double_t _yoffset    = 0.048;
Int_t    _verbosity  = 3;
TString  _directory  = "results/Summer12_53X/NewJEC";
TString  _format     = "png";


vector<UInt_t> vprocess;


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(Int_t channel = MuMu)
{
  if (channel < 0 || channel >= nChannels) return; 

  if (channel == MuMu) process[Data] = "DoubleMu";
  if (channel == EE)   process[Data] = "DoubleElectron";
  if (channel == EMu)  process[Data] = "MuEG";
  if (channel == MuE)  process[Data] = "MuEG";

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetHatchesLineWidth(1.0);
  gStyle->SetHatchesSpacing  (0.7);
  
  gSystem->mkdir(_format, kTRUE);

  TH1::SetDefaultSumw2();


  vprocess.clear();
  vprocess.push_back(Data);
  vprocess.push_back(WWTo2L2Nu_Madgraph);
  vprocess.push_back(ggWWto2L);
  vprocess.push_back(ZZ);
  vprocess.push_back(WgammaToLNuG);
  vprocess.push_back(TW);
  vprocess.push_back(TbarW);
  vprocess.push_back(TTbar_Madgraph);
  vprocess.push_back(WJets_Madgraph);
  vprocess.push_back(ZJets_Madgraph);
  vprocess.push_back(ZgammaToLLG);


  // Read input files
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    input[j] = new TFile(_directory + "/" + process[j] + ".root");
  }


  // Do the work
  //----------------------------------------------------------------------------
  if (0) MeasureTheCrossSection(sChannel[channel]);

  TString suffix = "_" + sChannel[channel] + "_" + sCut[WWLevel];
    
  DrawHistogram("hNPV"       + suffix, "number of primary vertices", -1, 0, "NULL", 0, 30, false);
  DrawHistogram("hMET"       + suffix, "E_{T}^{miss}",                5, 0, "GeV");
  DrawHistogram("hPtLepton1" + suffix, "p_{T}^{leading lepton}",      5, 0, "GeV");
  DrawHistogram("hPtLepton2" + suffix, "p_{T}^{trailing lepton}",     5, 0, "GeV",  0, 100);
  DrawHistogram("hMll"       + suffix, "m_{#font[12]{ll}}",           5, 0, "GeV");
}


//------------------------------------------------------------------------------
// MeasureTheCrossSection
//------------------------------------------------------------------------------
void MeasureTheCrossSection(TString channel = "MuMu")
{
  Double_t ndata       = 0;
  Double_t nbackground = 0;
  Double_t nexp_qqWW   = 0;
  Double_t nexp_ggWW   = 0;
  Double_t nexp_TTbar  = 0;
  Double_t nexp_WJets  = 0;
  Double_t nexp_DY     = 0;

  Double_t nraw_qqWW = ((TH1F*)input[WWTo2L2Nu_Madgraph]->Get("hCounterEff_" + channel + "_WWLevel"))->Integral();
  Double_t nraw_ggWW = ((TH1F*)input[ggWWto2L]          ->Get("hCounterEff_" + channel + "_WWLevel"))->Integral();

  TString hname = "hCounter_" + channel + "_WWLevel";

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    TH1F* dummy = (TH1F*)input[j]->Get(hname);

    if (j == Data) {
      ndata = dummy->Integral();
    }
    else if (j == WWTo2L2Nu_Madgraph) {
      nexp_qqWW = dummy->Integral();
    }
    else if (j == ggWWto2L) {
      nexp_ggWW = dummy->Integral();
    }
    else if (j == TW || j == TbarW || j == TTbar_Madgraph) {
      nbackground += dummy->Integral();
      nexp_TTbar  += dummy->Integral();
    }
    else if (j == WJets_Madgraph) {
      nbackground += dummy->Integral();
      nexp_WJets  += dummy->Integral();
    }
    else if (j == ZJets_Madgraph || j == ZgammaToLLG) {
      nbackground += dummy->Integral();
      nexp_DY     += dummy->Integral();
    }
    else {
      nbackground += dummy->Integral();
    }      
  }


  // Estimate the cross section
  //----------------------------------------------------------------------------
  Double_t efficiency = f_qqWW * nraw_qqWW / ngen_qqWW + f_ggWW * nraw_ggWW / ngen_ggWW;

  Double_t xs = (ndata - nbackground) / (_luminosity * efficiency * WW2lnln);


  // Relative errors
  //----------------------------------------------------------------------------
  Double_t xsRelativeErrorBackground = 0.0;
  Double_t xsRelativeErrorEfficiency = 0.0;

  Double_t xsRelativeErrorSyst = 0;
  xsRelativeErrorSyst += (xsRelativeErrorBackground * xsRelativeErrorBackground);
  xsRelativeErrorSyst += (xsRelativeErrorEfficiency * xsRelativeErrorEfficiency);
  xsRelativeErrorSyst = sqrt(xsRelativeErrorSyst);

  Double_t xsRelativeErrorStat = 1e2 * sqrt(ndata) / (ndata - nbackground);
  Double_t xsRelativeErrorLumi = 4.4;


  // Absolute errors
  //----------------------------------------------------------------------------
  Double_t xsErrorSyst = xs * xsRelativeErrorSyst / 1e2;  // pb
  Double_t xsErrorStat = xs * xsRelativeErrorStat / 1e2;  // pb
  Double_t xsErrorLumi = xs * xsRelativeErrorLumi / 1e2;  // pb


  // Print the results
  //----------------------------------------------------------------------------
  if (_verbosity > 0) {
    printf("\n");
    printf("                          ---------- %s ---------- ", channel.Data());
  }

  if (_verbosity > 2) {
    printf("\n");
    printf("                          luminosity = %.1f pb\n", _luminosity);
    printf("                        generated gg -> WW = %.0f\n", ngen_ggWW);
    printf("                        generated qq -> WW = %.0f\n", ngen_qqWW);
  }

  if (_verbosity > 1) {
    printf("\n");
    printf("                       WW efficiency = %.2f %s\n", 1e2 * efficiency, "%");
    printf("                         nbackground = %6.1f +- %5.1f\n", nbackground,       sqrt(nbackground));
    printf("                               ndata = %6.1f +- %5.1f\n", ndata,             sqrt(ndata));
    printf("                 ndata - nbackground = %6.1f +- %5.1f\n", ndata-nbackground, sqrt(ndata+nbackground));
    printf("                            qq -> WW = %6.1f +- %5.1f\n", nexp_qqWW,         sqrt(nexp_qqWW));
    printf("                            gg -> WW = %6.1f +- %5.1f\n", nexp_ggWW,         sqrt(nexp_ggWW));
    printf("                                 top = %6.1f +- %5.1f\n", nexp_TTbar,        sqrt(nexp_TTbar));
    printf("                              W+jets = %6.1f +- %5.1f\n", nexp_WJets,        sqrt(nexp_WJets));
    printf("                              Z+jets = %6.1f +- %5.1f\n", nexp_DY,           sqrt(nexp_DY));
  }

  if (_verbosity > 0) {
    printf("\n");
    printf("                         measured xs = %.2f +- %.2f (stat) +- %.2f (lumi) pb\n", xs, xsErrorStat, xsErrorLumi);
    printf("\n");
  }
}


//------------------------------------------------------------------------------
// DrawHistogram
//------------------------------------------------------------------------------
void DrawHistogram(TString  hname,
		   TString  xtitle,
		   Int_t    ngroup       = -1,
		   Int_t    precision    = 0,
		   TString  units        = "NULL",
		   Double_t xmin         = -999,
		   Double_t xmax         =  999,
		   Bool_t   moveOverflow = true)
{
  TCanvas* canvas = new TCanvas(hname, hname, 550, 900);

  TPad* pad1 = new TPad("pad1", "pad1", 0, 0.44, 1, 1.00);
  TPad* pad2 = new TPad("pad1", "pad1", 0, 0.22, 1, 0.44);
  TPad* pad3 = new TPad("pad1", "pad1", 0, 0.00, 1, 0.22);

  pad1->SetTopMargin   (0.08);
  pad1->SetBottomMargin(0.02);
  pad1->Draw();

  pad2->SetTopMargin   (0.08);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();

  pad3->SetTopMargin   (0.03);
  pad3->SetBottomMargin(0.35);
  pad3->Draw();


  //----------------------------------------------------------------------------
  // pad1
  //----------------------------------------------------------------------------
  pad1->cd();
  
  pad1->SetLogy(_setLogy);

  THStack* hstack = new THStack(hname, hname);

  TH1F* hist[nProcesses];

  for (UInt_t i=0; i<vprocess.size(); i++) {

    UInt_t j = vprocess.at(i);

    hist[j] = (TH1F*)input[j]->Get(hname);
    hist[j]->SetName(hname + process[j]);

    if (moveOverflow) MoveOverflowBins  (hist[j], xmin, xmax);
    else              ZeroOutOfRangeBins(hist[j], xmin, xmax);
    
    if (ngroup > 0) hist[j]->Rebin(ngroup);

    if (j == Data) {
      hist[j]->SetMarkerStyle(kFullCircle);
      hist[j]->SetTitle("");
    }
    else {
      hist[j]->SetFillColor(color[j]);
      hist[j]->SetFillStyle(1001);
      hist[j]->SetLineColor(color[j]);

      hstack->Add(hist[j]);
    }
  }


  // All MC
  //----------------------------------------------------------------------------
  TH1F* allmc = (TH1F*)hist[Data]->Clone("allmc");

  allmc->SetFillColor  (kGray+2);
  allmc->SetFillStyle  (   3345);
  allmc->SetLineColor  (kGray+2);
  allmc->SetMarkerColor(kGray+2);
  allmc->SetMarkerSize (      0);

  for (UInt_t ibin=1; ibin<=allmc->GetNbinsX(); ibin++) {

    Double_t binValue = 0;
    Double_t binError = 0;

    for (UInt_t i=0; i<vprocess.size(); i++) {

      UInt_t j = vprocess.at(i);

      if (j == Data) continue;

      Double_t binContent = hist[j]->GetBinContent(ibin);
      
      binValue += binContent;
      binError += (hist[j]->GetBinError(ibin) * hist[j]->GetBinError(ibin));
      binError += (systError[j]*binContent * systError[j]*binContent);
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

  if (pad1->GetLogy()) {

    theMax = TMath::Power(10, TMath::Log10(theMax) + 4);

    hist[Data]->SetMinimum(0.01);
  }
  else theMax *= 1.3;

  hist[Data]->SetMaximum(theMax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0     = 0.700;
  Double_t y0     = 0.834;
  Double_t delta  = _yoffset + 0.001;
  Double_t ndelta = 0;
  
  DrawLegend(x0, y0 - ndelta, hist[Data],               " data",          "lp"); ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[WWTo2L2Nu_Madgraph], " WW",            "f");  ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[ZZ],                 " VV",            "f");  ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[ZJets_Madgraph],     " Z + jets",      "f");  ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[WJets_Madgraph],     " W + jets",      "f");  ndelta += delta;
  DrawLegend(x0, y0 - ndelta, hist[TTbar_Madgraph],     " top",           "f");  ndelta += delta;
  DrawLegend(x0, y0 - ndelta, allmc,                    " #sigma_{stat}", "f");  ndelta += delta;


  // CMS titles
  //----------------------------------------------------------------------------
  TString leftTitle = "CMS preliminary";

  if (hname.Contains("MuMu")) leftTitle = "#mu#mu";
  if (hname.Contains("EE"))   leftTitle = "ee";
  if (hname.Contains("EMu"))  leftTitle = "e#mu";
  if (hname.Contains("MuE"))  leftTitle = "#mue";

  DrawTLatex(0.185, 0.975, 0.05, 13, leftTitle.Data());
  DrawTLatex(0.940, 0.983, 0.05, 33, Form("#sqrt{s} = 8 TeV, L = %.1f fb^{-1}", _luminosity/1e3));


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
  pad2->cd();
    
  TH1F* ratio       = hist[Data]->Clone("ratio");
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
    
  uaxis->SetRangeUser(xmin, xmax);
     
  uncertainty->Draw("e2");
  ratio->Draw("ep,same");

  uncertainty->GetYaxis()->SetRangeUser(0, 2.5);


  //----------------------------------------------------------------------------
  // pad3
  //----------------------------------------------------------------------------
  pad3->cd();

  TH1F* hdeviations = new TH1F("hdeviations", "", 25, 0, 2.5);

  hdeviations->SetName("hdeviations_" + hname);

  for (UInt_t i=1; i<=ratio->GetNbinsX(); i++)
    if (ratio->GetBinContent(i) > 0) hdeviations->Fill(ratio->GetBinContent(i));

  hdeviations->Draw("hist");

  hdeviations->SetFillColor(kGray+2);
  hdeviations->SetFillStyle(3354);
  hdeviations->SetLineColor(0);
  hdeviations->SetLineWidth(0);


  // Save
  //----------------------------------------------------------------------------
  pad2->cd(); SetAxis(uncertainty, hist[Data]->GetXaxis()->GetTitle(), "data / prediction", 0.10, 0.8);
  pad1->cd(); SetAxis(hist[Data], "", hist[Data]->GetYaxis()->GetTitle(),                   0.05, 1.6);
  pad3->cd(); SetAxis(hdeviations, "data / prediction", "entries / bin",                    0.10, 0.8);

  canvas->cd();

  TString suffixLogy = (_setLogy) ? "_log" : "";

  canvas->SaveAs(Form("%s/%s%s.%s",
		      _format.Data(),
		      hname.Data(),
		      suffixLogy.Data(),
		      _format.Data()));
}


//------------------------------------------------------------------------------
// GetMaximumIncludingErrors
//------------------------------------------------------------------------------
Float_t GetMaximumIncludingErrors(TH1F*    h,
				  Double_t xmin = -999,
				  Double_t xmax = -999)
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
void MoveOverflowBins(TH1* h,
		      Double_t xmin,
		      Double_t xmax) const
{
  UInt_t nbins = h->GetNbinsX();

  TAxis* axis = (TAxis*)h->GetXaxis();
  
  Int_t firstBin = (xmin != -999) ? axis->FindBin(xmin) : 1;
  Int_t lastBin  = (xmax != -999) ? axis->FindBin(xmax) : nbins;

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
// ZeroOutOfRangeBins
//------------------------------------------------------------------------------
void ZeroOutOfRangeBins(TH1* h, Double_t xmin, Double_t xmax) const
{
  UInt_t nbins = h->GetNbinsX();

  TAxis* axis = (TAxis*)h->GetXaxis();
  
  Int_t firstBin = (xmin != -999) ? axis->FindBin(xmin) : 1;
  Int_t lastBin  = (xmax != -999) ? axis->FindBin(xmax) : nbins;

  for (UInt_t i=0; i<=nbins+1; i++) {

    if (i < firstBin || i > lastBin) {
      h->SetBinContent(i, 0);
      h->SetBinError  (i, 0);
    }
  }
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
		const char* text)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC();
  tl->SetTextAlign(align);
  tl->SetTextFont (   42);
  tl->SetTextSize (tsize);

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
		    Float_t tsize   = 0.035,
		    Float_t xoffset = 0.200,
		    Float_t yoffset = _yoffset)
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
