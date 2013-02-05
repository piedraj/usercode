const UInt_t nchannels = 5;

Double_t xs7tev         [nchannels] = {19.32, 19.81, 22.81, 18.57, 18.95};
Double_t xs7tevErrorStat[nchannels] = { 1.22,  2.20,  2.99,  2.32,  2.61};
Double_t xs7tevErrorSyst[nchannels] = { 0.71,  0.74,  0.98,  0.71,  0.72};
Double_t xs7tevErrorLumi[nchannels] = { 0.45,  0.47,  0.52,  0.43,  0.44};

Double_t xs8tev         [nchannels] = {999, 25.7, 24.0, 22.9, 22.9};
Double_t xs8tevErrorStat[nchannels] = {999,  1.4,  1.8,  1.5,  1.6};
Double_t xs8tevErrorSyst[nchannels] = {999,  0.0,  0.0,  0.0,  0.0};
Double_t xs8tevErrorLumi[nchannels] = {999,  1.1,  1.1,  1.0,  1.0};

TString label[nchannels] = {"inclusive", "#mu#mu#mu", "eee", "#mu#mue", "ee#mu"};


// Settings
//------------------------------------------------------------------------------
TString _format = "png";


//------------------------------------------------------------------------------
// ratios
//------------------------------------------------------------------------------
void ratios(Int_t ecm = 8)
{
  Double_t luminosity = (ecm == 8) ? 19602 : 4920;  // pb-1
  Double_t xsWplusZ   = (ecm == 8) ?  13.9 : 11.4;  // pb
  Double_t xsWminusZ  = (ecm == 8) ?   8.1 :  6.4;  // pb

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetEndErrorSize(5);

  gSystem->mkdir(_format, kTRUE);


  // Loop
  //----------------------------------------------------------------------------
  Inclusive(ecm);

  TGraphErrors* gStat = new TGraphErrors(nchannels);
  TGraphErrors* gSyst = new TGraphErrors(nchannels);
  TGraphErrors* gLumi = new TGraphErrors(nchannels);

  for (UInt_t i=0; i<nchannels; i++) {

    Double_t xs          = (ecm == 8) ? xs8tev         [i] : xs7tev         [i];
    Double_t xsErrorStat = (ecm == 8) ? xs8tevErrorStat[i] : xs7tevErrorStat[i];
    Double_t xsErrorSyst = (ecm == 8) ? xs8tevErrorSyst[i] : xs7tevErrorSyst[i];
    Double_t xsErrorLumi = (ecm == 8) ? xs8tevErrorLumi[i] : xs7tevErrorLumi[i];

    Double_t f = xs / (xsWplusZ + xsWminusZ);

    Double_t errorSquared = (xsErrorStat * xsErrorStat);

    gStat->SetPointError(i, f * sqrt(errorSquared) / xs, 0.0);

    errorSquared += (xsErrorSyst * xsErrorSyst);

    gSyst->SetPointError(i, f * sqrt(errorSquared) / xs, 0.0);

    errorSquared += (xsErrorLumi * xsErrorLumi);

    gLumi->SetPointError(i, f * sqrt(errorSquared) / xs, 0.0);

    gStat->SetPoint(i, f, i+1);
    gSyst->SetPoint(i, f, i+1);
    gLumi->SetPoint(i, f, i+1);
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
  TCanvas* canvas = new TCanvas(Form("canvas%d", ecm), Form("canvas%d", ecm));

  canvas->SetLeftMargin(canvas->GetRightMargin());

  Double_t xmin = 0.6;
  Double_t xmax = 2.2;
  Double_t ymin = 0.50;
  Double_t ymax = nchannels + ymin;
  
  TH2F* dummy = new TH2F(Form("dummy%d", ecm), "",
			 100, xmin, xmax,
			 100, ymin, ymax);

  dummy->Draw();


  // Vertical line at 1
  //----------------------------------------------------------------------------
  TLine* line = new TLine(1.0, ymin, 1.0, ymax);

  line->SetLineWidth(2);

  line->Draw("same");


  // Ratios
  //----------------------------------------------------------------------------
  gLumi->Draw("p||,same");
  gSyst->Draw("p||,same");
  gStat->Draw("p,same");


  // Labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nchannels; i++) {

    Double_t x = gStat->GetX()[i];
    Double_t y = gStat->GetY()[i];

    DrawTLatex(xmin+0.05, y, 0.035, 12, Form("%s", label[i].Data()));

    Double_t statError  = gStat->GetErrorX(i);
    Double_t systError  = gSyst->GetErrorX(i);
    Double_t lumiError  = gLumi->GetErrorX(i);

    lumiError = sqrt(lumiError*lumiError - systError*systError);

    systError = sqrt(systError*systError - statError*statError);

    DrawTLatex(xmax-0.05, y, 0.035, 32, Form("%.2f #pm %.2f #pm %.2f #pm %.2f",
					     x, statError, systError, lumiError));
  }

  DrawTLatex(0.940, 0.983, 0.05, 33,
	     Form("#sqrt{s} = %d TeV, L = %.1f fb^{-1}", ecm, luminosity/1e3), true);

  dummy->GetXaxis()->CenterTitle();
  dummy->GetXaxis()->SetTitleOffset(1.4);
  dummy->GetXaxis()->SetTitle("#sigma_{WZ}^{exp} / #sigma_{WZ}^{theo}");
  dummy->GetYaxis()->SetTitle("");


  // Remove y-axis labels
  //----------------------------------------------------------------------------
  TAxis* yaxis = dummy->GetYaxis();
  
  for (UInt_t j=1; j<yaxis->GetNbins(); j++) yaxis->SetBinLabel(j, "");


  // Save
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();

  canvas->SaveAs(Form("%s/ratios%dtev.%s",
		      _format.Data(),
		      ecm,
		      _format.Data()));
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Double_t    x,
		Double_t    y,
		Double_t    tsize,
		Short_t     align,
		const char* text,
		Bool_t      setndc = false)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont (    42);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}


//------------------------------------------------------------------------------
// Inclusive
//------------------------------------------------------------------------------
void Inclusive(Int_t ecm)
{
  if (ecm == 7) return;

  Double_t x     = 0;
  Double_t stat  = 0;
  Double_t syst  = 0;
  Double_t lumi  = 0;
  Double_t total = 0;

  for (UInt_t i=1; i<nchannels; i++) {

    Double_t xs          = (ecm == 8) ? xs8tev         [i] : xs7tev         [i];
    Double_t xsErrorStat = (ecm == 8) ? xs8tevErrorStat[i] : xs7tevErrorStat[i];
    Double_t xsErrorSyst = (ecm == 8) ? xs8tevErrorSyst[i] : xs7tevErrorSyst[i];
    Double_t xsErrorLumi = (ecm == 8) ? xs8tevErrorLumi[i] : xs7tevErrorLumi[i];

    Double_t xsErrorTotal = 0;

    xsErrorTotal += (xsErrorStat * xsErrorStat);
    xsErrorTotal += (xsErrorSyst * xsErrorSyst);
    xsErrorTotal += (xsErrorLumi * xsErrorLumi);

    xsErrorTotal = sqrt(xsErrorTotal);

    x += (xs8tev[i] / xsErrorTotal / xsErrorTotal);

    stat  += (1. / xsErrorStat  / xsErrorStat);
    syst  += (1. / xsErrorSyst  / xsErrorSyst);
    lumi  += (1. / xsErrorLumi  / xsErrorLumi);
    total += (1. / xsErrorTotal / xsErrorTotal);
  }

  xs8tev[0] = x / total;

  xs8tevErrorStat[0] = 1. / sqrt(stat);
  xs8tevErrorSyst[0] = 1. / sqrt(syst);
  xs8tevErrorLumi[0] = 1. / sqrt(lumi);
}
