const Double_t nlo7tev      = 47.04;  // [pb]
const Double_t nlo7tevPlus  = 4.3 * nlo7tev / 1e2;
const Double_t nlo7tevMinus = 3.2 * nlo7tev / 1e2;

const Double_t nlo8tev      = 57.25;  // [pb]
const Double_t nlo8tevPlus  = 4.1 * nlo8tev / 1e2;
const Double_t nlo8tevMinus = 2.8 * nlo8tev / 1e2;


const UInt_t nchannels = 5;

TString channelLabel[] = {"all", "#mu#mu", "ee", "e#mu", "#mue"};


Double_t luminosity = 5.064;  // [fb-1]


void compareXS()
{
  Double_t xs7tev_0jet  [nchannels] = {52.37, 51.88, 60.19, 48.76, 52.60};
  Double_t stat7tev_0jet[nchannels] = { 1.99,  3.96,  5.66,  3.43,  3.67};
  Double_t syst7tev_0jet[nchannels] = { 4.11,  4.08,  5.39,  4.46,  4.27};
  Double_t lumi7tev_0jet[nchannels] = { 1.15,  1.14,  1.32,  1.07,  1.16};

  Double_t xs8tev_0jet  [nchannels] = {65.51, 65.93, 56.01, 67.91, 69.14};
  Double_t stat8tev_0jet[nchannels] = { 2.35,  5.06,  6.37,  3.98,  4.30};

  Double_t xs8tev_1jet  [nchannels] = {52.76, 53.50, 59.94, 59.19, 48.63};
  Double_t stat8tev_1jet[nchannels] = { 5.02, 12.71, 15.99,  8.00,  8.31};

  Double_t xs8tev_incl  [nchannels] = {61.82, 62.67, 57.00, 65.28, 62.81};
  Double_t stat8tev_incl[nchannels] = { 2.21,  4.95,  6.25,  3.68,  3.93};


  TGraphErrors* g8tev_0jet = new TGraphErrors(nchannels);
  TGraphErrors* g8tev_1jet = new TGraphErrors(nchannels);
  TGraphErrors* g8tev_incl = new TGraphErrors(nchannels);

  
  // Loop
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nchannels; i++) {

    g8tev_0jet->SetPoint(i, i, xs8tev_0jet[i]);
    g8tev_1jet->SetPoint(i, i, xs8tev_1jet[i]);
    g8tev_incl->SetPoint(i, i, xs8tev_incl[i]);

    g8tev_0jet->SetPointError(i, 0.5, stat8tev_0jet[i]);
    g8tev_1jet->SetPointError(i, 0.5, stat8tev_1jet[i]);
    g8tev_incl->SetPointError(i, 0.5, stat8tev_incl[i]);
  }


  // Cosmetics
  //----------------------------------------------------------------------------
  g8tev_0jet->SetLineWidth  (2);
  g8tev_0jet->SetMarkerSize (1.4);
  g8tev_0jet->SetMarkerStyle(kFullCircle);

  g8tev_1jet->SetLineColor  (kBlue);
  g8tev_1jet->SetLineWidth  (2);
  g8tev_1jet->SetMarkerColor(kBlue);
  g8tev_1jet->SetMarkerSize (1.4);
  g8tev_1jet->SetMarkerStyle(kFullSquare);

  g8tev_incl->SetLineColor  (kGreen+1);
  g8tev_incl->SetLineWidth  (2);
  g8tev_incl->SetMarkerColor(kGreen+1);
  g8tev_incl->SetMarkerSize (1.4);
  g8tev_incl->SetMarkerStyle(kOpenCircle);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas();
  
  TMultiGraph* mg = new TMultiGraph();

  mg->Add(g8tev_1jet);
  mg->Add(g8tev_0jet);
  mg->Add(g8tev_incl);

  mg->Draw("ap");

  mg->GetYaxis()->SetTitle("#sigma_{WW} (pb)");
  mg->GetYaxis()->SetTitleOffset(1.5);


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
  TLegend* legend = new TLegend(0.223, 0.673, 0.467, 0.895);

  legend->SetFillColor(0);
  legend->SetTextFont (42);
  legend->SetTextSize (0.035);

  legend->AddEntry(nlo,        " NLO",   "f");
  legend->AddEntry(g8tev_0jet, " 0-jet", "lp");
  legend->AddEntry(g8tev_1jet, " 1-jet", "lp");
  legend->AddEntry(g8tev_incl, " inclusive", "lp");


  // Put everything together
  //----------------------------------------------------------------------------
  legend->Draw("same");
  nlo   ->Draw("e2,same");
  line  ->Draw("same");
  mg    ->Draw("p,same");


  mg->SetMaximum(91);
  mg->SetMinimum(38);


  // Additional titles
  //----------------------------------------------------------------------------
  DrawTLatex(0.185, 0.970, 0.04, 13, "CMS preliminary");
  DrawTLatex(0.940, 0.978, 0.04, 33, Form("#sqrt{s} = 8 TeV, L = %.3f fb^{-1}", luminosity));


  // And save it
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("wwxs013.png");
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
