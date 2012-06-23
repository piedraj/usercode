const UInt_t nchannels = 5;

TString channelLabel[] = {"all", "#mu#mu", "ee", "e#mu", "#mue"};


void compareXS()
{
  // NLO values
  //----------------------------------------------------------------------------
  Double_t nlo7tev      = 47.04;
  Double_t nlo7tevPlus  = 4.3 * nlo7tev / 1e2;
  Double_t nlo7tevMinus = 3.2 * nlo7tev / 1e2;

  Double_t nlo8tev      = 57.25;
  Double_t nlo8tevPlus  = 4.1 * nlo8tev / 1e2;
  Double_t nlo8tevMinus = 2.8 * nlo8tev / 1e2;

  Double_t nlo7tevMcErr = 0.02;
  Double_t nlo8tevMcErr = 0.02;

  nlo7tevPlus  = sqrt((nlo7tevPlus *nlo7tevPlus)  + (nlo7tevMcErr*nlo7tevMcErr));
  nlo7tevMinus = sqrt((nlo7tevMinus*nlo7tevMinus) + (nlo7tevMcErr*nlo7tevMcErr));

  nlo8tevPlus  = sqrt((nlo8tevPlus *nlo8tevPlus)  + (nlo8tevMcErr*nlo8tevMcErr));
  nlo8tevMinus = sqrt((nlo8tevMinus*nlo8tevMinus) + (nlo8tevMcErr*nlo8tevMcErr));

  printf("\n");
  printf(" sigma(NLO, 7 TeV) = %6.3f +%5.3f -%5.3f pb\n", nlo7tev, nlo7tevPlus, nlo7tevMinus);
  printf(" sigma(NLO, 8 TeV) = %6.3f +%5.3f -%5.3f pb\n", nlo8tev, nlo8tevPlus, nlo8tevMinus);
  printf("\n");


  // CMS values
  //----------------------------------------------------------------------------
  Double_t xs7tev  [nchannels] = {52.37, 51.88, 60.19, 48.76, 52.60};
  Double_t stat7tev[nchannels] = { 1.99,  3.96,  5.66,  3.43,  3.67};
  Double_t syst7tev[nchannels] = { 4.11,  4.08,  5.39,  4.46,  4.27};
  Double_t lumi7tev[nchannels] = { 1.15,  1.14,  1.32,  1.07,  1.16};

  Double_t xs8tev  [nchannels] = {68.40, 62.66, 50.97, 74.35, 73.66};
  Double_t stat8tev[nchannels] = { 2.79,  5.87,  7.64,  4.76,  5.09};
  Double_t syst8tev[nchannels] = { 6.43,  7.56, 10.66,  6.83,  6.42};
  Double_t lumi8tev[nchannels] = { 3.42,  3.13,  2.55,  3.72,  3.68};

  TGraphAsymmErrors* ratio7tev = new TGraphAsymmErrors(nchannels);
  TGraphAsymmErrors* ratio8tev = new TGraphAsymmErrors(nchannels);

  
  // Loop
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nchannels; i++) {

    ratio7tev->SetPoint(i, i, RatioValue(xs7tev[i], nlo7tev));
    ratio8tev->SetPoint(i, i, RatioValue(xs8tev[i], nlo8tev));

    Double_t error7tev = sqrt(stat7tev[i]*stat7tev[i] + syst7tev[i]*syst7tev[i] + lumi7tev[i]*lumi7tev[i]);
    Double_t error8tev = sqrt(stat8tev[i]*stat8tev[i] + syst8tev[i]*syst8tev[i] + lumi8tev[i]*lumi8tev[i]);

    ratio7tev->SetPointError(i, 0.5, 0.5,
			     RatioError(xs7tev[i], nlo7tev, error7tev, nlo7tevMinus),
			     RatioError(xs7tev[i], nlo7tev, error7tev, nlo7tevPlus));

    ratio8tev->SetPointError(i, 0.5, 0.5,
			     RatioError(xs8tev[i], nlo8tev, error8tev, nlo8tevMinus),
			     RatioError(xs8tev[i], nlo8tev, error8tev, nlo8tevPlus));
  }

  
  // Cosmetics
  //----------------------------------------------------------------------------
  ratio7tev->SetMarkerStyle(kFullCircle);
  ratio8tev->SetMarkerStyle(kFullCircle);

  ratio8tev->SetLineColor  (kRed+1);
  ratio8tev->SetMarkerColor(kRed+1);

  ratio7tev->SetLineWidth(2);
  ratio8tev->SetLineWidth(2);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas();
  
  TMultiGraph* mg = new TMultiGraph();

  mg->Add(ratio7tev);
  mg->Add(ratio8tev);

  mg->Draw("ap");

  mg->GetYaxis()->SetTitle("#sigma_{WW} / #sigma_{WW}^{NLO}");
  mg->GetYaxis()->SetTitleOffset(1.6);

  mg->SetMaximum(1.9);

  canvas->Update();


  // x-axis label
  //----------------------------------------------------------------------------
  TAxis* xaxis = mg->GetXaxis();

  xaxis->SetLabelSize(0.07);

  for (UInt_t i=0; i<nchannels; i++)
    xaxis->SetBinLabel(xaxis->FindBin(i), channelLabel[i].Data());

  xaxis->CenterLabels();
  xaxis->LabelsOption("h");


  // Line at 1.0
  //----------------------------------------------------------------------------
  TLine* line = new TLine(canvas->GetUxmin(), 1.0, canvas->GetUxmax(), 1.0);

  line->SetLineStyle(3);
  line->SetLineWidth(3);


  // Legend
  //----------------------------------------------------------------------------
  TLegend* legend = new TLegend(0.42, 0.74, 0.69, 0.90);

  legend->SetFillColor(0);
  legend->SetTextFont (42);
  legend->SetTextSize (0.035);

  legend->AddEntry(ratio7tev,   " L = 4.92 fb^{-1} with #sqrt{s} = 7 TeV", "lp");
  legend->AddEntry(ratio8tev,   " L = 3.54 fb^{-1} with #sqrt{s} = 8 TeV", "lp");


  // Put everything together
  //----------------------------------------------------------------------------
  line  ->Draw("same");
  mg    ->Draw("p,same");
  legend->Draw("same");


  // And save it
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("wwxsRatio.png");
}


//------------------------------------------------------------------------------
// RatioValue
//------------------------------------------------------------------------------
Double_t RatioValue(Double_t num,
		    Double_t den)
{
  if (den == 0) return -999;

  Double_t rValue = num / den;

  return rValue;
}


//------------------------------------------------------------------------------
// RatioError
//------------------------------------------------------------------------------
Double_t RatioError(Double_t num,
		    Double_t den,
		    Double_t numErr,
		    Double_t denErr)
{
  if (num == 0) return -999;

  Double_t rValue = RatioValue(num, den);

  Double_t rError = (numErr/num)*(numErr/num) + (denErr/den)*(denErr/den);

  rError = rValue * sqrt(rError);

  return rError;
}
