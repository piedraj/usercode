//------------------------------------------------------------------------------
//
// Vector boson pair production at the LHC, arXiv:1105.0020
//
// All cross sections are in pb, unless stated otherwise.
//
//------------------------------------------------------------------------------
const Double_t WplusZ_xs7tev_nlo            = 11.88;
const Double_t WplusZ_xs7tev_nlo_upperError = 5.5 * WplusZ_xs7tev_nlo / 1e2;
const Double_t WplusZ_xs7tev_nlo_lowerError = 4.2 * WplusZ_xs7tev_nlo / 1e2;

const Double_t WminusZ_xs7tev_nlo            = 6.69;
const Double_t WminusZ_xs7tev_nlo_upperError = 5.6 * WplusZ_xs7tev_nlo / 1e2;
const Double_t WminusZ_xs7tev_nlo_lowerError = 4.3 * WplusZ_xs7tev_nlo / 1e2;

const Double_t WW_xs7tev_nlo            = 47.04;
const Double_t WW_xs7tev_nlo_upperError = 4.3 * WW_xs7tev_nlo / 1e2;
const Double_t WW_xs7tev_nlo_lowerError = 3.2 * WW_xs7tev_nlo / 1e2;

const Double_t WW_xs8tev_nlo            = 57.25;
const Double_t WW_xs8tev_nlo_upperError = 4.1 * WW_xs8tev_nlo / 1e2;
const Double_t WW_xs8tev_nlo_lowerError = 2.8 * WW_xs8tev_nlo / 1e2;


//------------------------------------------------------------------------------
//
// compareXS
//
//------------------------------------------------------------------------------
const UInt_t nchannels = 5;

TString channelLabel[nchannels] = {"all", "#mu#mu", "ee", "e#mu", "#mue"};


void compareXS(Int_t energy = 7)
{
  Double_t luminosity = 5.064;

  if (energy == 7) luminosity = 4.92;

  Double_t xs7tev_0jet  [nchannels] = {52.37, 51.88, 60.19, 48.76, 52.60};
  Double_t stat7tev_0jet[nchannels] = { 1.99,  3.96,  5.66,  3.43,  3.67};
  Double_t syst7tev_0jet[nchannels] = { 4.11,  4.08,  5.39,  4.46,  4.27};
  Double_t lumi7tev_0jet[nchannels] = { 1.15,  1.14,  1.32,  1.07,  1.16};

  Double_t xs8tev_0jet  [nchannels] = {65.31, 65.90, 55.98, 67.59, 68.86};
  Double_t stat8tev_0jet[nchannels] = { 2.35,  5.06,  6.37,  3.98,  4.30};

  Double_t xs8tev_1jet  [nchannels] = {50.17, 53.13, 59.63, 55.97, 44.68};
  Double_t stat8tev_1jet[nchannels] = { 5.02, 12.71, 15.99,  8.00,  8.31};

  Double_t xs8tev_2jet  [nchannels] = {55.87, 31.54, 95.17, 65.82, 50.50};
  Double_t stat8tev_2jet[nchannels] = {12.14, 30.07, 38.74, 18.85, 19.93};

  Double_t xs8tev_incl  [nchannels] = {61.68, 62.55, 56.90, 65.11, 62.66};
  Double_t stat8tev_incl[nchannels] = { 2.21,  4.95,  6.25,  3.68,  3.93};


  // Loop
  //----------------------------------------------------------------------------
  Double_t absoluteMax = -999;

  TGraphErrors* g7tev_0jet = new TGraphErrors(nchannels);
  TGraphErrors* g8tev_0jet = new TGraphErrors(nchannels);
  TGraphErrors* g8tev_1jet = new TGraphErrors(nchannels);
  TGraphErrors* g8tev_2jet = new TGraphErrors(nchannels);
  TGraphErrors* g8tev_incl = new TGraphErrors(nchannels);

  for (UInt_t i=0; i<nchannels; i++) {

    g7tev_0jet->SetPoint(i, i, xs7tev_0jet[i]);
    g8tev_0jet->SetPoint(i, i, xs8tev_0jet[i]);
    g8tev_1jet->SetPoint(i, i, xs8tev_1jet[i]);
    g8tev_2jet->SetPoint(i, i, xs8tev_2jet[i]);
    g8tev_incl->SetPoint(i, i, xs8tev_incl[i]);

    g7tev_0jet->SetPointError(i, 0.5, stat7tev_0jet[i]);
    g8tev_0jet->SetPointError(i, 0.5, stat8tev_0jet[i]);
    g8tev_1jet->SetPointError(i, 0.5, stat8tev_1jet[i]);
    g8tev_2jet->SetPointError(i, 0.5, stat8tev_2jet[i]);
    g8tev_incl->SetPointError(i, 0.5, stat8tev_incl[i]);

    if (energy == 7) {
      if (absoluteMax < xs7tev_0jet[i] + stat7tev_0jet[i])
	absoluteMax = xs7tev_0jet[i] + stat7tev_0jet[i];
    }
    else if (energy == 8) {
      if (absoluteMax < xs8tev_0jet[i] + stat8tev_0jet[i])
	absoluteMax = xs8tev_0jet[i] + stat8tev_0jet[i];
      if (absoluteMax < xs8tev_1jet[i] + stat8tev_1jet[i])
	absoluteMax = xs8tev_1jet[i] + stat8tev_1jet[i];
    }
  }


  // Cosmetics
  //----------------------------------------------------------------------------
  g7tev_0jet->SetLineWidth  (2);
  g7tev_0jet->SetMarkerSize (1.4);
  g7tev_0jet->SetMarkerStyle(kFullCircle);

  g8tev_0jet->SetLineWidth  (2);
  g8tev_0jet->SetMarkerSize (1.4);
  g8tev_0jet->SetMarkerStyle(kFullCircle);

  g8tev_1jet->SetLineColor  (kBlue);
  g8tev_1jet->SetLineWidth  (2);
  g8tev_1jet->SetMarkerColor(kBlue);
  g8tev_1jet->SetMarkerSize (1.4);
  g8tev_1jet->SetMarkerStyle(kFullTriangleUp);

  g8tev_2jet->SetLineColor  (kGreen+1);
  g8tev_2jet->SetLineWidth  (2);
  g8tev_2jet->SetMarkerColor(kGreen+1);
  g8tev_2jet->SetMarkerSize (1.4);
  g8tev_2jet->SetMarkerStyle(kFullSquare);

  g8tev_incl->SetLineColor  (kGreen+1);
  g8tev_incl->SetLineWidth  (2);
  g8tev_incl->SetMarkerColor(kGreen+1);
  g8tev_incl->SetMarkerSize (1.4);
  g8tev_incl->SetMarkerStyle(kOpenCircle);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas();
  
  TMultiGraph* mg = new TMultiGraph();

  if (energy == 7) {
    mg->Add(g7tev_0jet);
  }
  else if (energy == 8) {
    mg->Add(g8tev_1jet);
    mg->Add(g8tev_0jet);
  }

  mg->Draw("ap");

  mg->SetMaximum(1.15 * absoluteMax);

  if (energy == 7) mg->SetMinimum(41);

  mg->GetYaxis()->SetTitle("#sigma_{WW} (pb)");
  mg->GetYaxis()->SetTitleOffset(1.5);


  // x-axis label
  //----------------------------------------------------------------------------
  TAxis* xaxis = mg->GetXaxis();

  xaxis->SetLabelSize(0.07);

  for (UInt_t i=0; i<nchannels; i++)
    xaxis->SetBinLabel(xaxis->FindBin(i), AlignLabel(channelLabel[i]));

  xaxis->CenterLabels();
  xaxis->LabelsOption("h");


  // NLO WW cross-section
  //----------------------------------------------------------------------------
  TH1F* nlo;
  
  if (energy == 7) {

    TH1F* wz = DrawBand(68.89 - 17.81, 8.71, 8.71, "wz", kGreen-10, 3375, kGreen+1, 1, 2);
    
    nlo = DrawBand(WW_xs7tev_nlo, WW_xs7tev_nlo_upperError, WW_xs7tev_nlo_lowerError);
  }
  else if (energy == 8)
    nlo = DrawBand(WW_xs8tev_nlo, WW_xs8tev_nlo_upperError, WW_xs8tev_nlo_lowerError);


  // Legend
  //----------------------------------------------------------------------------
  Double_t x0 = 0.22; 
  Double_t y0 = 0.84;

  DrawTLegend(x0, y0, nlo, " NLO", "f");

  if (energy == 7) {
    DrawTLegend(x0, y0 - 1.* 0.053, g7tev_0jet, " 0-jet",           "lp");
    DrawTLegend(x0, y0 - 2.* 0.053, wz,         " WZ+WW - WZ(NLO)", "f");
  }
  else if (energy == 8) {
    DrawTLegend(x0, y0 - 1.* 0.053, g8tev_0jet, " 0-jet", "lp");
    DrawTLegend(x0, y0 - 2.* 0.053, g8tev_1jet, " 1-jet", "lp");
  }


  // Additional titles
  //----------------------------------------------------------------------------
  DrawTLatex(0.185, 0.970, 0.04, 13, "CMS preliminary");
  DrawTLatex(0.940, 0.978, 0.04, 33, Form("#sqrt{s} = %d TeV, L = %.3f fb^{-1}",
					  energy, luminosity));


  // And save it
  //----------------------------------------------------------------------------
  mg->Draw("p,same");

  canvas->Update();
  canvas->GetFrame()->DrawClone();

  canvas->SaveAs("xs.png");
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
// AlignLabel
//------------------------------------------------------------------------------
const char* AlignLabel(TString label)
{
  const char* patch = "#color[0]{l}";

  const char* result = Form("%s%s%s", patch, label.Data(), patch);

  return result;
}


//------------------------------------------------------------------------------
// DrawBand
//------------------------------------------------------------------------------
TH1F* DrawBand(Double_t centralValue,
	       Double_t upperError,
	       Double_t lowerError,
	       TString  name   = "hist",
	       Color_t  hcolor = kRed+1,
	       Style_t  hstyle = 3354,
	       Color_t  lcolor = kRed+1,
	       Style_t  lstyle = 1,
	       Width_t  lwidth = 2)
{
  TH1F* hist = new TH1F(name, "", nchannels, gPad->GetUxmin(), gPad->GetUxmax());

  for (UInt_t i=0; i<=nchannels; i++) {
    hist->SetBinContent(i, centralValue + 0.5 * (upperError - lowerError));
    hist->SetBinError  (i,                0.5 * (upperError + lowerError));
  }

  hist->SetLineColor  (0);
  hist->SetFillColor  (hcolor);
  hist->SetFillStyle  (hstyle);
  hist->SetMarkerColor(hcolor);

  hist->Draw("e2,same");

  TLine* line = new TLine(gPad->GetUxmin(), centralValue,
			  gPad->GetUxmax(), centralValue);

  line->SetLineColor(lcolor);
  line->SetLineStyle(lstyle);
  line->SetLineWidth(lwidth);

  line->Draw("same");

  return hist;
}


//------------------------------------------------------------------------------
// DrawTLegend
//------------------------------------------------------------------------------
TLegend* DrawTLegend(Float_t  x1,
		     Float_t  y1,
		     TObject* obj,
		     TString  label,
		     TString  option,
		     Float_t  tsize   = 0.04,
		     Float_t  xoffset = 0.22,
		     Float_t  yoffset = 0.05)
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

  legend->AddEntry(obj, label.Data(), option.Data());
  legend->Draw();

  return legend;
}
