const UInt_t nchannels = 5;

TString channelLabel[] = {"all", "#mu#mu", "ee", "e#mu", "#mue"};


// Settings
//------------------------------------------------------------------------------
Double_t _luminosity;


//------------------------------------------------------------------------------
// compareXS
//------------------------------------------------------------------------------
void compareXS(Double_t luminosity = 5064)
{
  _luminosity = luminosity;


  // NLO values
  //----------------------------------------------------------------------------
  Double_t nlo7tev      = 47.04;
  Double_t nlo7tevPlus  = 4.3 * nlo7tev / 1e2;
  Double_t nlo7tevMinus = 3.2 * nlo7tev / 1e2;

  Double_t nlo8tev      = 57.25;
  Double_t nlo8tevPlus  = 4.1 * nlo8tev / 1e2;
  Double_t nlo8tevMinus = 2.8 * nlo8tev / 1e2;

  printf("\n");
  printf(" sigma(NLO, 7 TeV) = %6.3f +%5.3f -%5.3f pb\n", nlo7tev, nlo7tevPlus, nlo7tevMinus);
  printf(" sigma(NLO, 8 TeV) = %6.3f +%5.3f -%5.3f pb\n", nlo8tev, nlo8tevPlus, nlo8tevMinus);
  printf("\n");


  // CMS values
  //----------------------------------------------------------------------------
  Double_t xs7tev_0jet  [nchannels] = {52.37, 51.88, 60.19, 48.76, 52.60};
  Double_t stat7tev_0jet[nchannels] = { 1.99,  3.96,  5.66,  3.43,  3.67};
  Double_t syst7tev_0jet[nchannels] = { 4.11,  4.08,  5.39,  4.46,  4.27};
  Double_t lumi7tev_0jet[nchannels] = { 1.15,  1.14,  1.32,  1.07,  1.16};

  Double_t xs8tev_0jet  [nchannels] = {65.87, 66.28, 56.36, 68.27, 69.50};
  Double_t stat8tev_0jet[nchannels] = { 2.35,  5.06,  6.37,  3.98,  4.30};
  Double_t syst8tev_0jet[nchannels] = { 5.37,  5.59,  5.90,  5.83,  5.34};
  Double_t lumi8tev_0jet[nchannels] = { 2.90,  2.92,  2.48,  3.00,  3.06};

  Double_t xs8tev_1jet  [nchannels] = {51.59, 52.19, 58.77, 58.07, 47.98};
  Double_t stat8tev_1jet[nchannels] = { 5.02, 12.71, 15.99,  8.00,  8.31};
  Double_t syst8tev_1jet[nchannels] = {10.63, 14.53, 12.33, 10.76, 10.10};
  Double_t lumi8tev_1jet[nchannels] = { 2.27,  2.30,  2.59,  2.56,  2.09};

  TGraphAsymmErrors* ratio8tev_0jet = new TGraphAsymmErrors(nchannels);
  TGraphAsymmErrors* ratio8tev_1jet = new TGraphAsymmErrors(nchannels);

  
  // Loop
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nchannels; i++) {

    ratio8tev_0jet->SetPoint(i, i, xs8tev_0jet[i]);
    ratio8tev_1jet->SetPoint(i, i, xs8tev_1jet[i]);

    ratio8tev_0jet->SetPointError(i, 0.5, 0.5, stat8tev_0jet[i], stat8tev_0jet[i]);
    ratio8tev_1jet->SetPointError(i, 0.5, 0.5, stat8tev_1jet[i], stat8tev_1jet[i]);
  }

  
  // Cosmetics
  //----------------------------------------------------------------------------
  ratio8tev_0jet->SetLineWidth  (2);
  ratio8tev_0jet->SetMarkerSize (1.4);
  ratio8tev_0jet->SetMarkerStyle(kFullCircle);

  ratio8tev_1jet->SetLineColor  (kBlue);
  ratio8tev_1jet->SetLineWidth  (2);
  ratio8tev_1jet->SetMarkerColor(kBlue);
  ratio8tev_1jet->SetMarkerSize (1.4);
  ratio8tev_1jet->SetMarkerStyle(kFullSquare);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas();
  
  TMultiGraph* mg = new TMultiGraph();

  mg->Add(ratio8tev_1jet);
  mg->Add(ratio8tev_0jet);

  mg->Draw("ap");

  mg->GetYaxis()->SetTitle("#sigma_{WW} [pb]");
  mg->GetYaxis()->SetTitleOffset(1.6);

  canvas->Update();


  // x-axis label
  //----------------------------------------------------------------------------
  TAxis* xaxis = mg->GetXaxis();

  xaxis->SetLabelSize(0.07);

  for (UInt_t i=0; i<nchannels; i++)
    xaxis->SetBinLabel(xaxis->FindBin(i), channelLabel[i].Data());

  xaxis->CenterLabels();
  xaxis->LabelsOption("h");


  // NLO WW cross-section
  //----------------------------------------------------------------------------
  TH1F* nlo = new TH1F("nlo", "nlo", nchannels, canvas->GetUxmin(), canvas->GetUxmax());

  for(UInt_t i=0; i<=nchannels; i++) {
    nlo->SetBinContent(i, nlo8tev + 0.5 * (nlo8tevPlus - nlo8tevMinus));
    nlo->SetBinError  (i,           0.5 * (nlo8tevPlus + nlo8tevMinus));
  }

  nlo->SetLineColor  (0);
  nlo->SetFillColor  (kRed+1);
  nlo->SetFillStyle  (3354);
  nlo->SetMarkerColor(0);

  TLine* line = new TLine(canvas->GetUxmin(), nlo8tev, canvas->GetUxmax(), nlo8tev);

  line->SetLineColor(kRed+1);
  line->SetLineWidth(2);


  // Legend
  //----------------------------------------------------------------------------
  TLegend* legend = new TLegend(0.225, 0.705, 0.500, 0.895);

  legend->SetFillColor(0);
  legend->SetTextFont (42);
  legend->SetTextSize (0.035);

  legend->AddEntry(nlo,            " NLO",   "f");
  legend->AddEntry(ratio8tev_0jet, " 0-jet", "lp");
  legend->AddEntry(ratio8tev_1jet, " 1-jet", "lp");


  // Put everything together
  //----------------------------------------------------------------------------
  legend->Draw("same");
  nlo   ->Draw("e2,same");
  line  ->Draw("same");
  mg    ->Draw("p,same");


  mg->SetMaximum(90);
  mg->SetMinimum(35);


  // Additional titles
  //----------------------------------------------------------------------------
  DrawTLatex(0.9, 0.860, 0.04, "CMS preliminary");
  DrawTLatex(0.9, 0.815, 0.03, Form("L = %.3f fb^{-1}", _luminosity/1e3));


  // And save it
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("wwxs0jet1jet.png");
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
