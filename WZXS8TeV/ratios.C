const UInt_t nchannels = 4;


Double_t xs7tev         [nchannels] = {      19.5,   23.3,      19.7,    18.1};
Double_t xs7tevErrorStat[nchannels] = {       2.2,    3.4,       2.5,     2.7};
Double_t xs7tevErrorLumi[nchannels] = {       0.5,    0.5,       0.5,     0.4};
Double_t xs8tev         [nchannels] = {      27.6,   29.3,      27.3,    26.0};
Double_t xs8tevErrorStat[nchannels] = {       1.7,    2.1,       1.9,     1.9};
Double_t xs8tevErrorLumi[nchannels] = {       1.2,    1.3,       1.2,     1.1};
TString  label          [nchannels] = {"#mu#mu#mu", "eee", "#mu#mue", "ee#mu"};


// Settings
//------------------------------------------------------------------------------
TString _directory = "Summer12_53X";
TString _format    = "png";


//------------------------------------------------------------------------------
// ratios
//------------------------------------------------------------------------------
void ratios(Int_t ecm = 8)
{
  Double_t luminosity = (ecm == 8) ? 12103 : 4920;  // pb-1
  Double_t xsWplusZ   = (ecm == 8) ?  13.9 : 11.4;  // pb
  Double_t xsWminusZ  = (ecm == 8) ?   8.1 :  6.4;  // pb

  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetEndErrorSize(5);

  gSystem->mkdir(_format + "/" + _directory, kTRUE);


  // Loop
  //----------------------------------------------------------------------------
  TGraphErrors* gStat = new TGraphErrors(nchannels);
  TGraphErrors* gLumi = new TGraphErrors(nchannels);

  for (UInt_t i=0; i<nchannels; i++) {

    Double_t xs          = (ecm == 8) ? xs8tev         [i] : xs7tev         [i];
    Double_t xsErrorStat = (ecm == 8) ? xs8tevErrorStat[i] : xs7tevErrorStat[i];
    Double_t xsErrorLumi = (ecm == 8) ? xs8tevErrorLumi[i] : xs7tevErrorLumi[i];

    Double_t f = xs / (xsWplusZ + xsWminusZ);

    Double_t error = 0;
    error += (xsErrorStat * xsErrorStat);
    error += (xsErrorLumi * xsErrorLumi);
    error = sqrt(error);

    gStat->SetPoint(i, f, i+1);
    gLumi->SetPoint(i, f, i+1);

    gStat->SetPointError(i, f * xsErrorStat / xs, 0.0);
    gLumi->SetPointError(i, f *       error / xs, 0.0);
  }


  // Cosmetics
  //----------------------------------------------------------------------------
  gStat->SetLineWidth  (2);
  gStat->SetMarkerSize (1.3);
  gStat->SetMarkerStyle(kFullCircle);

  gLumi->SetLineColor  (kBlue);
  gLumi->SetLineWidth  (2);
  gLumi->SetMarkerSize (1.3);
  gLumi->SetMarkerStyle(kFullCircle);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas();

  canvas->SetLeftMargin(canvas->GetRightMargin());

  //  Double_t xmin = 0.75;
  //  Double_t xmax = 2.00;
  //  Double_t ymin = 0.50;
  Double_t xmin = 0.6;
  Double_t xmax = 2.6;
  Double_t ymin = 0.50;
  Double_t ymax = nchannels + ymin;
  
  TH2F* dummy = new TH2F("dummy", "", 100, xmin, xmax, 100, ymin, ymax);

  dummy->Draw();


  // Vertical line at 1
  //----------------------------------------------------------------------------
  TLine* line = new TLine(1.0, ymin, 1.0, ymax);

  line->SetLineColor(kRed+1);
  line->SetLineWidth(2);

  line->Draw("same");


  // Ratios
  //----------------------------------------------------------------------------
  gLumi->Draw("p||,same");
  gStat->Draw("p,same");


  // Labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nchannels; i++) {

    Double_t x = gStat->GetX()[i];
    Double_t y = gStat->GetY()[i];

    DrawTLatex(xmin+0.05, y, 0.035, 12, Form("%s", label[i].Data()));

    Double_t statError  = gStat->GetErrorX(i);
    Double_t totalError = gLumi->GetErrorX(i);
    Double_t lumiError  = sqrt(totalError*totalError - statError*statError);

    DrawTLatex(xmax-0.05, y, 0.035, 32, Form("%.2f #pm %.2f (stat) #pm %.2f (lumi)",
					     x, statError, lumiError));
  }

  DrawTLatex(0.940, 0.983, 0.05, 33, Form("#sqrt{s} = %d TeV, L = %.1f fb^{-1}", ecm, luminosity/1e3), true);

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

  canvas->SaveAs(Form("%s/%s/ratios%dtev.%s",
		      _format.Data(),
		      _directory.Data(),
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
