#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"


const UInt_t nChannels = 4;

enum {iMMM, iEEE, iMME, iEEM};

TString sChannel[] = {"MMM", "EEE", "MME", "EEM"};


const UInt_t nCuts = 2;

enum {iZCandidate, iWCandidate};

TString sCut[] = {"ZCandidate", "WCandidate"};


const UInt_t nProcesses = 11;

enum {
  iData,
  iFakes,
  iZJets,
  iTW,
  iTbarW,
  iTTbar,
  //  iWJets,
  iZZ,
  iWW,
  iWgammaToLNuG,
  iZGToLL,
  iWZTo3LNu
};


TFile* input[nProcesses];


TString process[nProcesses];

process[iData]         = "Data";
process[iFakes]        = "Fakes";
process[iZJets]        = "ZJets_Madgraph";
process[iTW]           = "TW";
process[iTbarW]        = "TbarW";
process[iTTbar]        = "TTbar_Madgraph";
//process[iWJets]        = "WJets_Madgraph";
process[iZZ]           = "ZZ";
process[iWW]           = "WW";
process[iWgammaToLNuG] = "WgammaToLNuG";
process[iZGToLL]       = "ZgammaToLLG";
process[iWZTo3LNu]     = "WZTo3LNu";


Color_t color[nProcesses];

color[iData]         = kBlack;
color[iFakes]        = kAzure-7;
color[iZJets]        = kGreen+2;
color[iTW]           = kYellow;
color[iTbarW]        = kYellow;
color[iTTbar]        = kYellow;
//color[iWJets]        = kGray+1;
color[iZZ]           = kRed+3;
color[iWW]           = kAzure-9;
color[iWgammaToLNuG] = kAzure-2;
color[iZGToLL]       = kAzure-2;
color[iWZTo3LNu]     = kOrange-2;


Double_t systError[nProcesses];

systError[iData]         = 0.0;
systError[iFakes]        = 0.0 / 1e2;
systError[iZJets]        = 0.0 / 1e2;
systError[iTW]           = 0.0 / 1e2;
systError[iTbarW]        = 0.0 / 1e2;
systError[iTTbar]        = 0.0 / 1e2;
//systError[iWJets]        = 0.0 / 1e2;
systError[iZZ]           = 0.0 / 1e2;
systError[iWW]           = 0.0 / 1e2;
systError[iWgammaToLNuG] = 0.0 / 1e2;
systError[iZGToLL]       = 0.0 / 1e2;
systError[iWZTo3LNu]     = 0.0 / 1e2;


// Settings
//------------------------------------------------------------------------------
TString  _directory;
Double_t _luminosity;
TString  _format;
Bool_t   _setLogy;
Bool_t   _dataDriven;
Int_t    _verbosity = 1;
Double_t _yoffset   = 0.048;


//------------------------------------------------------------------------------
// XS
//------------------------------------------------------------------------------
void XS(TString  directory  = "Summer12_53X",
	Double_t luminosity = 12103.3,
	TString  format     = "png",
	Bool_t   setLogy    = false)
{
  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetHatchesLineWidth(1.00);
  gStyle->SetHatchesSpacing  (0.55);

  TH1::SetDefaultSumw2();

  gSystem->mkdir(_format + "/" + _directory, kTRUE);


  // Settings
  //----------------------------------------------------------------------------
  _directory  = directory,
  _luminosity = luminosity;
  _format     = format;
  _setLogy    = setLogy;
  _dataDriven = (_directory.Contains("Fakes")) ? true : false;


  // Read input files
  //----------------------------------------------------------------------------
  TString path = Form("/nfs/fanae/user/piedra/work/WZXS8TeV/rootfiles/%s/",
		      _directory.Data());

  for (UInt_t ip=0; ip<nProcesses; ip++) {
 
    if (_dataDriven) {
      if (
	  ip == iTW
	  || ip == iTbarW
	  || ip == iTTbar
	  || ip == iZJets
	  //	  || ip == iWJets
	  ) continue;
    } else {
      if (ip == iFakes) continue;
    }

    input[ip] = new TFile(path + process[ip] + ".root");
  }


  // Do the work
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannels; i++) {

    MeasureTheCrossSection(sChannel[i]);

    for (UInt_t j=0; j<nCuts; j++) {
	
      if (j == iZCandidate) continue;
	
      TString suffix = "_" + sChannel[i] + "_" + sCut[j];
	
      DrawHistogram("hMET"      + suffix, "E_{T}^{miss}",       4, 0, "GeV");
      DrawHistogram("hInvMassZ" + suffix, "m_{#font[12]{ll}}", -1, 0, "GeV", 70, 112);
    }
  }
}


//------------------------------------------------------------------------------
// MeasureTheCrossSection
//------------------------------------------------------------------------------
void MeasureTheCrossSection(TString channel = "MMM")
{
  Double_t xsWplusZ    = 14.48;  // pb
  Double_t xsWminusZ   = 8.40;   // pb
  Double_t W2e         = 0.1075;
  Double_t W2m         = 0.1057;
  Double_t W2tau       = 0.1125;
  Double_t Z2ll        = 0.033658;
  Double_t WZ23lnu     = 3 * Z2ll * (W2e + W2m + W2tau);
  Double_t ngenWZ      = 2017979;
  Double_t phase       = 0.72;
  Double_t nWZ         = 0;
  Double_t ndata       = 0;
  Double_t nsignal     = 0;
  Double_t nbackground = 0;


  // Read the histograms
  //----------------------------------------------------------------------------
  for (UInt_t ip=0; ip<nProcesses; ip++) {

    if (_dataDriven) {
      if (
	  ip == iTW
	  || ip == iTbarW
	  || ip == iTTbar
	  || ip == iZJets
	  //	  || ip == iWJets
	  ) continue;
    } else {
      if (ip == iFakes) continue;
    }

    TH1F* dummy = (TH1F*)input[ip]->Get("hCounter_" + channel + "_WCandidate");

    if (ip == iData)
      ndata = dummy->Integral();
    else if (ip == iWZTo3LNu) {
      nsignal = dummy->Integral();
      nWZ     = dummy->GetEntries();
    }
    else
      nbackground += dummy->Integral();
  }


  // Estimate the cross section
  //----------------------------------------------------------------------------
  Double_t efficiency = nWZ / (ngenWZ * phase);

  Double_t xs   = (ndata - nbackground) / (_luminosity * efficiency * WZ23lnu);
  Double_t xsmc =               nsignal / (_luminosity * efficiency * WZ23lnu);

  if (_verbosity > 2) {
    printf("\n");
    printf("                     BR(W  -> e   nu) = %5.2f %s\n", 1e2 * W2e,     "%");
    printf("                     BR(W  -> mu  nu) = %5.2f %s\n", 1e2 * W2m,     "%");
    printf("                     BR(W  -> tau nu) = %5.2f %s\n", 1e2 * W2tau,   "%");
    printf("                     BR(Z  -> ll)     = %7.4f %s\n", 1e2 * Z2ll,    "%");
    printf("                     BR(WZ -> 3l nu)  = %5.2f %s\n", 1e2 * WZ23lnu, "%");
    printf("                           luminosity = %.1f pb\n", _luminosity);
    printf("               number of generated WZ = %.0f\n", ngenWZ);
    printf(" fraction in phase space [71,111] GeV = %.2f\n", phase);
  }

  if (_verbosity > 1) {
    printf("\n");
    printf("                number of selected WZ = %.0f\n", nWZ);
    printf("                        WZ efficiency = %.2f %s\n", 1e2 * efficiency, "%");
    printf("                                ndata = %.0f\n", ndata);
    printf("                          nbackground = %.1f\n", nbackground);
    printf("                              nsignal = %.1f\n", nsignal);
  }

  if (_verbosity > 0) {
    printf("\n");
    printf("                     measured xs(%s) = %.2f pb\n", channel.Data(), xs);
    printf("                     measured xs(%s) = %.2f pb (signal from MC)\n", channel.Data(), xsmc);
    printf("                               NLO xs = %.2f pb (not corrected for phase space)\n", xsWplusZ+xsWminusZ);
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

  for (UInt_t ip=0; ip<nProcesses; ip++) {

    if (_dataDriven) {
      if (
	  ip == iTW
	  || ip == iTbarW
	  || ip == iTTbar
	  || ip == iZJets
	  //	  || ip == iWJets
	  ) continue;
    } else {
      if (ip == iFakes) continue;
    }

    hist[ip] = (TH1F*)input[ip]->Get(hname);
    hist[ip]->SetName(hname + process[ip]);

    if (ngroup > 0) hist[ip]->Rebin(ngroup);

    if (moveOverflow) MoveOverflowBins  (hist[ip], xmin, xmax);
    else              ZeroOutOfRangeBins(hist[ip], xmin, xmax);
    
    if (ip == iData) {
      hist[ip]->SetMarkerStyle(kFullCircle);
      hist[ip]->SetTitle("");
    }
    else {
      hist[ip]->SetFillColor(color[ip]);
      hist[ip]->SetFillStyle(1001);
      hist[ip]->SetLineColor(color[ip]);

      hstack->Add(hist[ip]);
    }
  }


  // All MC
  //----------------------------------------------------------------------------
  TH1F* allmc = (TH1F*)hist[iData]->Clone("allmc");

  allmc->SetFillColor  (kGray+2);
  allmc->SetFillStyle  (   3345);
  allmc->SetLineColor  (kGray+2);
  allmc->SetMarkerColor(kGray+2);
  allmc->SetMarkerSize (      0);

  for (UInt_t ibin=1; ibin<=allmc->GetNbinsX(); ibin++) {

    Double_t binValue = 0;
    Double_t binError = 0;

    for (UInt_t ip=0; ip<nProcesses; ip++) {

      if (ip == iData) continue;

      if (_dataDriven) {
	if (
	    ip == iTW
	    || ip == iTbarW
	    || ip == iTTbar
	    || ip == iZJets
	    //	    || ip == iWJets
	    ) continue;
      } else {
	if (ip == iFakes) continue;
      }

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
  xaxis->SetRangeUser(xmin, xmax);

  hist[iData]->Draw("ep");
  hstack     ->Draw("hist,same");
  //  allmc      ->Draw("e2,same");
  hist[iData]->Draw("ep,same");


  // Adjust scale
  //----------------------------------------------------------------------------
  Float_t theMax   = GetMaximumIncludingErrors(hist[iData], xmin, xmax);
  Float_t theMaxMC = GetMaximumIncludingErrors(allmc,       xmin, xmax);

  if (theMaxMC > theMax) theMax = theMaxMC;

  if (pad1->GetLogy()) {

    theMax = TMath::Power(10, TMath::Log10(theMax) + 4);

    hist[iData]->SetMinimum(0.01);
  }
  else theMax *= 1.45;

  hist[iData]->SetMaximum(theMax);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0    = 0.23;
  Double_t y0    = 0.74;
  Double_t delta = _yoffset + 0.001;

  DrawLegend(x0,        y0 + 2.*delta, hist[iData],     " data",    "lp");
  DrawLegend(x0,        y0 + 1.*delta, hist[iWZTo3LNu], " WZ",      "f");
  DrawLegend(x0 + 0.22, y0 + 2.*delta, hist[iWW],       " WW",      "f");
  DrawLegend(x0 + 0.22, y0 + 1.*delta, hist[iZGToLL],   " V#gamma", "f");
  DrawLegend(x0 + 0.22, y0 + 0.*delta, hist[iZZ],       " ZZ",      "f");

  if (_dataDriven)
    DrawLegend(x0, y0 + 0.*delta, hist[iFakes], " data-driven", "f");
  else {
    DrawLegend(x0,        y0 + 0.*delta, hist[iTTbar], " top",      "f");
    DrawLegend(x0,        y0 - 1.*delta, hist[iZJets], " Z + jets", "f");
    //    DrawLegend(x0 + 0.22, y0 - 1.*delta, hist[iWJets], " W + jets", "f");
  }


  // CMS titles
  //----------------------------------------------------------------------------
  TString leftTitle = "CMS preliminary";

  if (hname.Contains("MMM")) leftTitle = "#mu#mu#mu";
  if (hname.Contains("EEE")) leftTitle = "eee";
  if (hname.Contains("MME")) leftTitle = "#mu#mue";
  if (hname.Contains("EEM")) leftTitle = "ee#mu";

  DrawTLatex(0.185, 0.975, 0.05, 13, leftTitle.Data());
  DrawTLatex(0.940, 0.983, 0.05, 33, Form("L = %.3f fb^{-1}", _luminosity/1e3));


  //----------------------------------------------------------------------------
  // pad2
  //----------------------------------------------------------------------------
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

  for (Int_t i=1; i<=ratio->GetNbinsX(); i++)
    if (ratio->GetBinContent(i) > 0) hdeviations->Fill(ratio->GetBinContent(i));

  hdeviations->Draw("hist");

  hdeviations->SetFillColor(kGray+2);
  hdeviations->SetFillStyle(3354);
  hdeviations->SetLineColor(0);
  hdeviations->SetLineWidth(0);

  DrawTLatex(0.22, 0.89, 0.1, 13, Form("#mu = %.2f", hdeviations->GetMean()));


  // Save
  //----------------------------------------------------------------------------
  pad2->cd(); SetAxis(uncertainty, hist[iData]->GetXaxis()->GetTitle(), "data / prediction", 0.10, 0.8);
  pad1->cd(); SetAxis(hist[iData], "", hist[iData]->GetYaxis()->GetTitle(),                  0.05, 1.6);
  pad3->cd(); SetAxis(hdeviations, "data / prediction", "entries / bin",                     0.10, 0.8);

  canvas->cd();

  TString suffixLogy = (_setLogy) ? "_log" : "";

  canvas->SaveAs(Form("%s/%s/%s%s.%s",
		      _format.Data(),
		      _directory.Data(),
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
