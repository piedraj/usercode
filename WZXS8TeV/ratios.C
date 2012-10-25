//------------------------------------------------------------------------------
//
// MCFM with 71 < mZ < 111 GeV
//
//------------------------------------------------------------------------------
const Double_t xsWplusZ  = 13.89;  // pb
const Double_t xsWminusZ =  8.06;  // pb


const UInt_t nchannels = 4;


// TTbar_Madgraph
//------------------------------------------------------------------------------
Double_t xs         [nchannels] = {      26.25, 21.79,     22.15,   22.99};
Double_t xsErrorStat[nchannels] = {       1.59,  1.91,      1.75,    1.77};
Double_t xsErrorLumi[nchannels] = {       1.15,  0.96,      0.97,    1.01};
TString  label      [nchannels] = {"#mu#mu#mu", "eee", "#mu#mue", "ee#mu"};


Double_t luminosity = 12103.3;  // pb
TString  format     = "png";


void ratios()
{
  gInterpreter->ExecuteMacro("HiggsPaperStyle.C");

  gStyle->SetEndErrorSize(5);

  gSystem->mkdir(format, kTRUE);


  // Loop
  //----------------------------------------------------------------------------
  TGraphErrors* gStat = new TGraphErrors(nchannels);
  TGraphErrors* gLumi = new TGraphErrors(nchannels);

  for (UInt_t i=0; i<nchannels; i++) {

    Double_t f = xs[i] / (xsWplusZ + xsWminusZ);

    Double_t error = 0;
    error += (xsErrorStat[i] * xsErrorStat[i]);
    error += (xsErrorLumi[i] * xsErrorLumi[i]);
    error = sqrt(error);

    gStat->SetPoint(i, f, i+1);
    gLumi->SetPoint(i, f, i+1);

    gStat->SetPointError(i, f * xsErrorStat[i] / xs[i], 0.0);
    gLumi->SetPointError(i, f *          error / xs[i], 0.0);
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

  Double_t xmin = 0.75;
  Double_t xmax = 2.00;
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

    DrawTLatex(xmin+0.03, y, 0.035, 12, Form("%s", label[i].Data()));

    Double_t statError  = gStat->GetErrorX(i);
    Double_t totalError = gLumi->GetErrorX(i);
    Double_t lumiError  = sqrt(totalError*totalError - statError*statError);

    DrawTLatex(xmax-0.03, y, 0.035, 32, Form("%.2f #pm %.2f (stat) #pm %.2f (lumi)",
					     x, statError, lumiError));
  }

  DrawTLatex(0.940, 0.983, 0.05, 33, Form("#sqrt{s} = 8 TeV, L = %.1f fb^{-1}", luminosity/1e3), true);

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

  canvas->SaveAs(format + "/ratios." + format);
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
