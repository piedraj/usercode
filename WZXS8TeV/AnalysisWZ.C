#include "AnalysisWZ.h"


#if !defined(__CINT__)
ClassImp(AnalysisWZ);
#endif


AnalysisWZ::AnalysisWZ(TTree* tree) : CMSAnalysisSelectorMiniTrees(tree) {}


//------------------------------------------------------------------------------
// Initialise
//------------------------------------------------------------------------------
void AnalysisWZ::Initialise()
{
  GetParameters();

  fPUWeight = new PUWeight(pu_luminosity, Summer12_53X, "2012"); 


  // Histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (UInt_t i=0; i<nChannel; i++) {

    for (UInt_t j=0; j<nCut; j++) {

      TString suffix = "_" + sChannel[i] + "_" + sCut[j];
      
      for (UInt_t k=0; k<nComposition; k++) {

	hCounterRaw[i][j][k] = CreateH1D("hCounterRaw" + suffix + "_" + sComposition[k], "", 3, 0, 3);
	hCounterPU [i][j][k] = CreateH1D("hCounterPU"  + suffix + "_" + sComposition[k], "", 3, 0, 3);
	hCounterEff[i][j][k] = CreateH1D("hCounterEff" + suffix + "_" + sComposition[k], "", 3, 0, 3);
	hCounter   [i][j][k] = CreateH1D("hCounter"    + suffix + "_" + sComposition[k], "", 3, 0, 3);
      }

      hLeptonWeight [i][j] = CreateH1D("hLeptonWeight"  + suffix, "", 90, 0.75, 1.05);
      hTriggerWeight[i][j] = CreateH1D("hTriggerWeight" + suffix, "", 90, 0.75, 1.05);
      hTotalWeight  [i][j] = CreateH1D("hTotalWeight"   + suffix, "", 90, 0.75, 1.05);

      hNPV         [i][j] = CreateH1D("hNPV"          + suffix, "",  50,  0,  50);
      hMET         [i][j] = CreateH1D("hMET"          + suffix, "", 200,  0, 200);
      hSumCharges  [i][j] = CreateH1D("hSumCharges"   + suffix, "",   9, -4,   5);
      hInvMass2Lep [i][j] = CreateH1D("hInvMass2Lep"  + suffix, "", 200,  0, 200);
      hInvMass3Lep [i][j] = CreateH1D("hInvMass3Lep"  + suffix, "", 400,  0, 400);
      hPtLepton1   [i][j] = CreateH1D("hPtLepton1"    + suffix, "", 200,  0, 200);
      hPtLepton2   [i][j] = CreateH1D("hPtLepton2"    + suffix, "", 200,  0, 200);
      hPtLepton3   [i][j] = CreateH1D("hPtLepton3"    + suffix, "", 200,  0, 200);    
      hPtLeadingJet[i][j] = CreateH1D("hPtLeadingJet" + suffix, "", 200,  0, 200);    
      hDPhiZLeptons[i][j] = CreateH1D("hDPhiZLeptons" + suffix, "", 320,  0, 3.2);    
      hPtZLepton1  [i][j] = CreateH1D("hPtZLepton1"   + suffix, "", 200,  0, 200);
      hPtZLepton2  [i][j] = CreateH1D("hPtZLepton2"   + suffix, "", 200,  0, 200);
      hPtZ         [i][j] = CreateH1D("hPtZ"          + suffix, "", 400,  0, 400);
      hPtWLepton   [i][j] = CreateH1D("hPtWLepton"    + suffix, "", 200,  0, 200);    
      hDRWZLepton1 [i][j] = CreateH1D("hDRWZLepton1"  + suffix, "", 200,  0,   6);    
      hDRWZLepton2 [i][j] = CreateH1D("hDRWZLepton2"  + suffix, "", 200,  0,   6);    
      hMtW         [i][j] = CreateH1D("hMtW"          + suffix, "", 200,  0, 200);    
      hNJet30      [i][j] = CreateH1D("hNJet30"       + suffix, "",  10,  0,  10);    
      hNBJet30     [i][j] = CreateH1D("hNBJet30"      + suffix, "",  10,  0,  10);    
    }
  }

  for (UInt_t i=0; i<2; i++) {

    TString suffix = (i == Muon) ? "MM" : "EE";

    hInvMass2LepBB[i] = CreateH1D("hInvMass2LepBB_" + suffix, "", 2000, 0, 200);
    hInvMass2LepBE[i] = CreateH1D("hInvMass2LepBE_" + suffix, "", 2000, 0, 200);
    hInvMass2LepEE[i] = CreateH1D("hInvMass2LepEE_" + suffix, "", 2000, 0, 200);
  }


  // SF, FR, PR and trigger efficiencies histograms
  //----------------------------------------------------------------------------
  MuonSF = LoadHistogram("MuSF_2012",  "h2inverted", "MuonSF");
  ElecSF = LoadHistogram("EleSF_2012", "h2inverted", "ElecSF");
  MuonPR = LoadHistogram("MuPR_2012",  "h2inverted", "MuonPR");
  ElecPR = LoadHistogram("ElePR_2012", "h2inverted", "ElecPR");

  MuonFR[MuonJet15] = LoadHistogram("MuFR_Moriond13_jet15_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet15");
  MuonFR[MuonJet20] = LoadHistogram("MuFR_Moriond13_jet20_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet20");
  MuonFR[MuonJet30] = LoadHistogram("MuFR_Moriond13_jet30_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet30");
  MuonFR[MuonJet50] = LoadHistogram("MuFR_Moriond13_jet50_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet50");

  ElecFR[ElecJet15] = LoadHistogram("EleFR_Moriond13_jet15_EWKcorr", "fakeElH2", "ElecFR_Jet15");
  ElecFR[ElecJet35] = LoadHistogram("EleFR_Moriond13_jet35_EWKcorr", "fakeElH2", "ElecFR_Jet35");
  ElecFR[ElecJet50] = LoadHistogram("EleFR_Moriond13_jet50_EWKcorr", "fakeElH2", "ElecFR_Jet50");

  DoubleElLead  = LoadHistogram("triggerEfficiencies", "DoubleElLead",  "DoubleElLead");
  DoubleMuLead  = LoadHistogram("triggerEfficiencies", "DoubleMuLead",  "DoubleMuLead");
  DoubleElTrail = LoadHistogram("triggerEfficiencies", "DoubleElTrail", "DoubleElTrail");
  DoubleMuTrail = LoadHistogram("triggerEfficiencies", "DoubleMuTrail", "DoubleMuTrail");


  // Electron energy scale systematics
  //----------------------------------------------------------------------------
  hScaleInEB  = GetHistogramFromGraph("hScaleInEB",  "gScaleInEB");
  hScaleOutEB = GetHistogramFromGraph("hScaleOutEB", "gScaleOutEB");
  hScaleEE    = GetHistogramFromGraph("hScaleEE",    "gScaleEE");
}


//------------------------------------------------------------------------------
// InsideLoop
//------------------------------------------------------------------------------
void AnalysisWZ::InsideLoop()
{
  if (sample.Contains("DoubleElectron") ||
      sample.Contains("DoubleMu") ||
      sample.Contains("MuEG"))
    {
      isData = true;
    }
  else
    {
      isData = false;
    }

  pu_weight = (isData) ? 1. : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

  efficiency_weight = 1.;
  mc_lepton_weight  = 1.;
  mc_trigger_weight = 1.;
  mc_total_weight   = 1.;

  AnalysisLeptons.clear();

  invMass2Lep    = 999.;
  invMass3Lep    = 999.;
  transverseMass = 999.;
  sumCharges     = 0.;

  nJet30    = 0;
  nBJet30   = 0;
  nElectron = 0;
  nTight    = 0;

  for (UInt_t iMuon=0; iMuon<nFakeRateMuon; iMuon++)
    for (UInt_t iElec=0; iElec<nFakeRateElec; iElec++)
      ddweight[iMuon][iElec] = 1.;


  // MC filters
  //----------------------------------------------------------------------------
  if (sample.Contains("WGstar") && WgammaFilter()) return;

  if (sample.Contains("ZJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;

  if (sample.Contains("WJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;


  // HLT
  //----------------------------------------------------------------------------
  if (!PassTrigger()) return;


  // Loop over muons
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Muon_Px->size(); i++) {

    TLorentzVector tmp(T_Muon_Px->at(i),
		       T_Muon_Py->at(i),
		       T_Muon_Pz->at(i),
		       T_Muon_Energy->at(i));
    
    Double_t eta = fabs(tmp.Eta());

    if (eta >= 2.4) continue;

    if (tmp.Pt() <= 8.) continue;

    Double_t pt = ScaleLepton(Muon, tmp.Pt());

    if (pt <= 10.) continue;

    TLorentzVector MuonVector = (pt / tmp.Pt()) * tmp;

    if (fabs(T_Muon_IP2DBiasedPV->at(i)) >= 0.2) continue;

    if (fabs(T_Muon_dzPVBiasedPV->at(i)) >= 0.1) continue;

    if (T_Muon_MVARings->at(i) <= -0.6) continue;

    if (!MuonID(i)) continue;

    Double_t MaxMuIP = (pt < 20) ? 0.01 : 0.02;

    Bool_t passIP = (fabs(T_Muon_IP2DBiasedPV->at(i)) < MaxMuIP);

    UInt_t muon_type = (MuonIsolation(i) && passIP) ? Tight : Fail;

    const Double_t sfMax = MuonSF->GetXaxis()->GetBinCenter(MuonSF->GetNbinsX());
    const Double_t prMax = MuonPR->GetXaxis()->GetBinCenter(MuonPR->GetNbinsX());
    const Double_t trMax = DoubleMuLead->GetXaxis()->GetBinCenter(DoubleMuLead->GetNbinsX());

    Lepton AnalysisMuon;

    AnalysisMuon.index  = i;
    AnalysisMuon.flavor = Muon;
    AnalysisMuon.type   = muon_type;
    AnalysisMuon.charge = T_Muon_Charge->at(i);
    AnalysisMuon.vraw   = tmp;
    AnalysisMuon.v      = MuonVector;
    AnalysisMuon.sf     = MuonSF->GetBinContent(MuonSF->FindBin(min(pt,sfMax),eta));
    AnalysisMuon.pr     = MuonPR->GetBinContent(MuonPR->FindBin(min(pt,prMax),eta));
    AnalysisMuon.lead   = DoubleMuLead ->GetBinContent(DoubleMuLead ->FindBin(min(pt,trMax),eta));
    AnalysisMuon.trail  = DoubleMuTrail->GetBinContent(DoubleMuTrail->FindBin(min(pt,trMax),eta));

    for (UInt_t j=0; j<nFakeRateMuon; j++)
      AnalysisMuon.frMuon[j] = MuonFR[j]->GetBinContent(MuonFR[j]->FindBin(min(pt,35.),eta));

    AnalysisLeptons.push_back(AnalysisMuon);
  }


  // Loop over electrons
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Elec_Px->size(); i++) {

    TLorentzVector tmp(T_Elec_Px->at(i),
		       T_Elec_Py->at(i),
		       T_Elec_Pz->at(i),
		       T_Elec_Energy->at(i));

    Double_t eta = fabs(tmp.Eta());

    if (eta >= 2.5) continue;

    if (tmp.Pt() <= 8.) continue;

    Double_t pt = ScaleLepton(Electron, tmp.Pt(), eta);

    if (pt <= 10.) continue;

    TLorentzVector ElectronVector = (pt / tmp.Pt()) * tmp;

    if (!ElectronID(i)) continue;

    Bool_t passIP = (fabs(T_Elec_IP2DBiasedPV->at(i)) < 0.02);

    passIP &= (fabs(T_Elec_dzPVBiasedPV->at(i)) < 0.1);

    UInt_t electron_type = (ElectronBDT(i) && ElectronIsolation(i) && passIP) ? Tight : Fail;

    const Double_t sfMax = ElecSF->GetXaxis()->GetBinCenter(ElecSF->GetNbinsX());
    const Double_t prMax = ElecPR->GetXaxis()->GetBinCenter(ElecPR->GetNbinsX());
    const Double_t trMax = DoubleElLead->GetXaxis()->GetBinCenter(DoubleElLead->GetNbinsX());

    Lepton AnalysisElectron;

    AnalysisElectron.index  = i;
    AnalysisElectron.flavor = Electron;
    AnalysisElectron.type   = electron_type;
    AnalysisElectron.charge = T_Elec_Charge->at(i);
    AnalysisElectron.vraw   = tmp;
    AnalysisElectron.v      = ElectronVector;
    AnalysisElectron.sf     = ElecSF->GetBinContent(ElecSF->FindBin(min(pt,sfMax),eta));
    AnalysisElectron.pr     = ElecPR->GetBinContent(ElecPR->FindBin(min(pt,prMax),eta));
    AnalysisElectron.lead   = DoubleElLead ->GetBinContent(DoubleElLead ->FindBin(min(pt,trMax),eta));
    AnalysisElectron.trail  = DoubleElTrail->GetBinContent(DoubleElTrail->FindBin(min(pt,trMax),eta));

    for (UInt_t j=0; j<nFakeRateElec; j++)
      AnalysisElectron.frElec[j] = ElecFR[j]->GetBinContent(ElecFR[j]->FindBin(min(pt,35.),eta));

    AnalysisLeptons.push_back(AnalysisElectron);
  }


  // Sort the leptons by pt
  //----------------------------------------------------------------------------
  std::sort(AnalysisLeptons.begin(), AnalysisLeptons.end());
  
  std::reverse(AnalysisLeptons.begin(), AnalysisLeptons.end());


  // Require at least two leptons
  //----------------------------------------------------------------------------
  if (AnalysisLeptons.size() < 2) return;


  // Apply DeltaR cut
  //----------------------------------------------------------------------------
  Double_t smallestDeltaR = 999.;

  for (UInt_t i=0; i<AnalysisLeptons.size(); i++) {

    for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++) {

      Double_t tmpDeltaR = AnalysisLeptons[i].v.DeltaR(AnalysisLeptons[j].v);

      if (tmpDeltaR < smallestDeltaR) smallestDeltaR = tmpDeltaR;
    }
  }

  if (smallestDeltaR < 0.001) return;


  // Make Z and W candidates
  //----------------------------------------------------------------------------
  Bool_t isZee = false;
  Bool_t isZmm = false;

  Bool_t foundDileptonWithLowMll = false;

  for (UInt_t i=0; i<AnalysisLeptons.size(); i++) {

    for (UInt_t j=i+1; j<AnalysisLeptons.size(); j++) {
      
      if (AnalysisLeptons[i].flavor != AnalysisLeptons[j].flavor) continue;

      if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0) continue;

      Double_t inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

      if (inv_mass < 12.) foundDileptonWithLowMll = true;

      if (fabs(inv_mass - Z_MASS) < fabs(invMass2Lep - Z_MASS)) {

	invMass2Lep = inv_mass;

	ZLepton1 = AnalysisLeptons[i].v;
	ZLepton2 = AnalysisLeptons[j].v;

	isZee = (AnalysisLeptons[i].flavor == Electron) ? true : false; 
	isZmm = (AnalysisLeptons[i].flavor == Muon)     ? true : false; 

	if (AnalysisLeptons.size() >= 3) {
	
	  for (UInt_t k=0; k<3; k++) {
	
	    if (k == i) continue;
	    if (k == j) continue;

	    WLepton = AnalysisLeptons[k].v;
	  }
	}
      }
    }
  }

  if (foundDileptonWithLowMll) return;

  if (!isZee & !isZmm) return;


  // Fill Z invariant mass at two-lepton level
  //----------------------------------------------------------------------------
  UInt_t index = (isZee) ? Electron : Muon;

  if (fabs(ZLepton1.Eta()) < 1.479 && fabs(ZLepton2.Eta()) < 1.479)
    {
      hInvMass2LepBB[index]->Fill(invMass2Lep, pu_weight * xs_weight);
    }
  else if (fabs(ZLepton1.Eta()) < 1.479 || fabs(ZLepton2.Eta()) < 1.479)
    {
      hInvMass2LepBE[index]->Fill(invMass2Lep, pu_weight * xs_weight);
    }
  else
    {
      hInvMass2LepEE[index]->Fill(invMass2Lep, pu_weight * xs_weight);
    }


  // Require at least three leptons
  //----------------------------------------------------------------------------
  if (AnalysisLeptons.size() < 3) return;


  // Set the MET of the event
  //----------------------------------------------------------------------------
  EventMET = GetMET();

  if (systematic == muonUpSyst ||
      systematic == muonDownSyst ||
      systematic == electronUpSyst ||
      systematic == electronDownSyst)
    {
      for (UInt_t i=0; i<3; i++)
	{
	  EventMET += (AnalysisLeptons[i].vraw - AnalysisLeptons[i].v);
	}
    }


  // Classify the channels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<3; i++)
    {
      if (AnalysisLeptons[i].flavor == Electron) nElectron++;
      if (AnalysisLeptons[i].type   == Tight)    nTight++;

      sumCharges += AnalysisLeptons[i].charge;
    }

  if (mode == RAW && nTight != 3) return;

  invMass3Lep = (AnalysisLeptons[0].v + AnalysisLeptons[1].v + AnalysisLeptons[2].v).M();

  if      (nElectron == 0) theChannel = MMM;
  else if (nElectron == 1) theChannel = MME;
  else if (nElectron == 2) theChannel = EEM;
  else if (nElectron == 3) theChannel = EEE;


  // Apply lepton SF and trigger efficiencies
  //----------------------------------------------------------------------------
  if (!isData)
    {
      for (UInt_t i=0; i<3; i++) mc_lepton_weight *= AnalysisLeptons[i].sf;

      mc_trigger_weight = GetTriggerWeight();

      mc_total_weight = mc_lepton_weight * mc_trigger_weight;

      efficiency_weight *= mc_total_weight;
    }


  // Data-driven estimates
  //----------------------------------------------------------------------------
  for (UInt_t iMuon=0; iMuon<nFakeRateMuon; iMuon++) {
    for (UInt_t iElec=0; iElec<nFakeRateElec; iElec++) {

      if (mode == PPP)
	{
	  ddweight[iMuon][iElec] = GetPPPWeight(iMuon,iElec);
	}
      else if (mode == PPF)
	{
	  ddweight[iMuon][iElec] =
	    GetPPFWeight(iMuon,iElec) +
	    GetPFFWeight(iMuon,iElec) +
	    GetFFFWeight(iMuon,iElec);
	}
    }
  }


  // Deal with jets
  //----------------------------------------------------------------------------
  ptLeadingJet = 0.;

  for (UInt_t i=0; i<T_JetAKCHS_Px->size(); i++) {
	
    TLorentzVector Jet(T_JetAKCHS_Px->at(i),
		       T_JetAKCHS_Py->at(i),
		       T_JetAKCHS_Pz->at(i),
		       T_JetAKCHS_Energy->at(i));
    
    if (Jet.Pt() > ptLeadingJet) ptLeadingJet = Jet.Pt();

    if (Jet.Pt() < 30.) continue;

    if (fabs(Jet.Eta()) > 4.7) continue;

    Bool_t isThisJetALepton = false;

    for (UInt_t lep=0; lep<3; lep++)
      if (Jet.DeltaR(AnalysisLeptons[lep].v) < 0.3) isThisJetALepton = true;

    if (isThisJetALepton) continue;

    nJet30++;

    if (T_JetAKCHS_Tag_CombSVtx->at(i) < 0.244) continue;

    nBJet30++;
  }


  // Compute the transverse mass
  //----------------------------------------------------------------------------
  const Double_t met  = EventMET.Et();
  const Double_t lWEt = WLepton.Et();
  
  transverseMass = lWEt*lWEt + met*met - 2.*lWEt*met*cos(WLepton.Angle(EventMET.Vect()));
  
  transverseMass = sqrt(transverseMass);


  // AtLeast3Leptons
  //----------------------------------------------------------------------------
  FillHistograms(theChannel, AtLeast3Leptons, ddweight[MuonJet20][ElecJet35]);


  // Exactly3Leptons
  //----------------------------------------------------------------------------
  if (AnalysisLeptons.size() != 3) return;

  if (AnalysisLeptons[2].v.Pt() <= 20) return;

  FillHistograms(theChannel, Exactly3Leptons, ddweight[MuonJet20][ElecJet35]);


  // HasZ20
  //----------------------------------------------------------------------------
  if (fabs(invMass2Lep - Z_MASS) > 20.) return;

  FillHistograms(theChannel, HasZ20, ddweight[MuonJet20][ElecJet35]);


  // HasZ10
  //----------------------------------------------------------------------------
  if (fabs(invMass2Lep - Z_MASS) > 10.) return;
  
  FillHistograms(theChannel, HasZ10, ddweight[MuonJet20][ElecJet35]);


  // MET30
  //----------------------------------------------------------------------------
  if (EventMET.Et() > 30.)
    {
      FillHistograms(theChannel, MET30, ddweight[MuonJet20][ElecJet35]);
    }
  else
    {
      FillHistograms(theChannel, ClosureTest, ddweight[MuonJet20][ElecJet35]);
      
      return;
    }


  // MET40
  //----------------------------------------------------------------------------
  if (EventMET.Et() <= 40.) return;

  FillHistograms(theChannel, MET40, ddweight[MuonJet20][ElecJet35]);


  // MET40AntiBtag
  //----------------------------------------------------------------------------
  if (nBJet30 > 0) return;

  FillHistograms(theChannel, MET40AntiBtag, ddweight[MuonJet20][ElecJet35]);
}


//------------------------------------------------------------------------------
// SetDataMembersAtTermination
//------------------------------------------------------------------------------
void AnalysisWZ::SetDataMembersAtTermination()
{
  for (UInt_t i=0; i<nChannel; i++) {
    for (UInt_t j=0; j<nCut; j++) {
      for (UInt_t k=0; k<nComposition; k++) {

	TString suffix = "_" + sChannel[i] + "_" + sCut[j] + "_" + sComposition[k];

	hCounterRaw[i][j][k] = (TH1D*)FindOutput("hCounterRaw" + suffix);
	hCounterPU [i][j][k] = (TH1D*)FindOutput("hCounterPU"  + suffix);
	hCounterEff[i][j][k] = (TH1D*)FindOutput("hCounterEff" + suffix);
	hCounter   [i][j][k] = (TH1D*)FindOutput("hCounter"    + suffix);
      }
    }
  }
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary()
{
  GetParameters();

  outputfile.open(Form("%s/%s.txt", directory.Data(), sample.Data()));

  outputfile << Form("\n %39s results with %7.1f pb\n", sample.Data(), luminosity);

  CounterSummary("No weights");
  CounterSummary("All weights");

  outputfile.close();
}


//------------------------------------------------------------------------------
// CounterSummary
//------------------------------------------------------------------------------
void AnalysisWZ::CounterSummary(TString title)
{
  outputfile << Form("\n %30s [%s]\n", " ", title.Data());

  outputfile << Form("\n %23s %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  for (UInt_t i=0; i<nCut; i++) {
      
    for (UInt_t k=nComposition; k-- > 0; ) {

      TString cut = (k == nComposition-1) ? sCut[i] : "";

      outputfile << Form(" %19s", cut.Data());

      for (UInt_t j=0; j<nChannel; j++) {

	TString composition = (j == 0) ? sComposition[k] + " " : "";

	Float_t integral = hCounter[j][i][k]->Integral();

	if      (title.Contains("No")) integral = hCounterRaw[j][i][k]->Integral();
	else if (title.Contains("PU")) integral = hCounterPU [j][i][k]->Integral();

	outputfile << Form(" %s%10.0f", composition.Data(), integral);
      }
      
      outputfile << "\n";
    }

    outputfile << "\n";
  }
}


//------------------------------------------------------------------------------
// GetParameters
//------------------------------------------------------------------------------
void AnalysisWZ::GetParameters()
{
  directory = GetInputParameters()->TheNamedString("directory");
  sample    = GetInputParameters()->TheNamedString("sample");

  GetInputParameters()->TheNamedDouble("luminosity",    luminosity);
  GetInputParameters()->TheNamedDouble("pu_luminosity", pu_luminosity);
  GetInputParameters()->TheNamedDouble("xs_weight",     xs_weight);
  GetInputParameters()->TheNamedInt   ("mode",          mode);
  GetInputParameters()->TheNamedInt   ("runAtOviedo",   runAtOviedo);
  GetInputParameters()->TheNamedInt   ("systematic",    systematic);
}


//------------------------------------------------------------------------------
// ElectronBDT
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::ElectronBDT(UInt_t iElec)
{
  Double_t eta = fabs(T_Elec_SC_Eta->at(iElec));

  Double_t pt = ScaleLepton(Electron, T_Elec_Pt->at(iElec), eta);
	
  Double_t mvaCut = 999;

  if (pt < 20)
    {
      if      (eta <= 0.8)                {mvaCut = 0.00;}
      else if (eta > 0.8 && eta <= 1.479) {mvaCut = 0.10;}
      else                                {mvaCut = 0.62;}
    }
  else
    {
      if      (eta <= 0.8)                 {mvaCut = 0.94;}
      else if (eta > 0.8 && eta <= 1.479 ) {mvaCut = 0.85;}
      else                                 {mvaCut = 0.92;}
    }

  Bool_t pass = (T_Elec_MVA->at(iElec) > mvaCut);
  
  return pass;
}


//------------------------------------------------------------------------------
// ElectronID
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::ElectronID(UInt_t iElec)
{
  Double_t eta           = T_Elec_SC_Eta       ->at(iElec);
  Double_t sigmaietaieta = T_Elec_sigmaIetaIeta->at(iElec);
  Double_t deltaPhiIn    = T_Elec_deltaPhiIn   ->at(iElec);
  Double_t deltaEtaIn    = T_Elec_deltaEtaIn   ->at(iElec);
  Double_t HtoE          = T_Elec_HtoE         ->at(iElec);
  Double_t trkIso03      = T_Elec_dr03TkSumPt  ->at(iElec);
  Double_t hadIso03      = T_Elec_dr03HcalSumEt->at(iElec);
  Double_t emIso03       = T_Elec_dr03EcalSumEt->at(iElec);
  Double_t max_emIso03   = std::max(emIso03-1., 0.);

  Double_t pt = ScaleLepton(Electron, T_Elec_Pt->at(iElec), eta);

  Bool_t pass = false;
	
  if (fabs(eta) < 1.479)
    {
      pass = sigmaietaieta   < 0.01
	&& fabs(deltaPhiIn)  < 0.15
	&& fabs(deltaEtaIn)  < 0.007
	&& HtoE              < 0.12
	// Isolation
	&& trkIso03    / pt  < 0.2
	&& max_emIso03 / pt  < 0.2
	&& hadIso03    / pt  < 0.2;
    }
  else
    {
      pass = sigmaietaieta   < 0.03
	&& fabs(deltaPhiIn)  < 0.1
	&& fabs(deltaEtaIn)  < 0.009
	&& HtoE              < 0.1
	// Isolation
	&& trkIso03 / pt     < 0.2
	&& emIso03  / pt     < 0.2
	&& hadIso03 / pt     < 0.2;
    }

  pass &= T_Elec_passConversionVeto->at(iElec);

  pass &= (T_Elec_nHits->at(iElec) == 0);

  return pass;
}


//------------------------------------------------------------------------------
// ElectronIsolation
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::ElectronIsolation(UInt_t iElec) 
{
  TLorentzVector Elec(T_Elec_Px->at(iElec),
		      T_Elec_Py->at(iElec),
		      T_Elec_Pz->at(iElec),
		      T_Elec_Energy->at(iElec));

  Double_t pt = ScaleLepton(Electron, Elec.Pt(), Elec.Eta());

  Double_t relIso = T_Elec_Iso->at(iElec) / pt;

  Bool_t pass = (relIso < 0.15);

  return pass;
}


//------------------------------------------------------------------------------
// MuonID
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonID(UInt_t iMuon)
{
  Double_t pt = ScaleLepton(Muon, T_Muon_Pt->at(iMuon));

  Bool_t passcutsforGlb = T_Muon_IsGlobalMuon->at(iMuon);

  passcutsforGlb &= (T_Muon_NormChi2GTrk   ->at(iMuon) < 10);
  passcutsforGlb &= (T_Muon_NumOfMatches   ->at(iMuon) >  1);
  passcutsforGlb &= (T_Muon_NValidHitsSATrk->at(iMuon) >  0);
	
  Bool_t passcutsforSA = T_Muon_IsAllTrackerMuons->at(iMuon);

  passcutsforSA &= T_Muon_IsTMLastStationTight->at(iMuon);

  Bool_t pass = (passcutsforGlb || passcutsforSA);

  pass &= (fabs(T_Muon_deltaPt->at(iMuon) / pt) < 0.1);
	
  pass &= T_Muon_isPFMuon->at(iMuon);

  pass &= (T_Muon_NLayers->at(iMuon) > 5);

  pass &= (T_Muon_trkKink->at(iMuon) < 20);

  pass &= (T_Muon_NValidPixelHitsInTrk->at(iMuon) > 0);  // NEW

  return pass;
}


//------------------------------------------------------------------------------
// MuonIsolation
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonIsolation(UInt_t iMuon) 
{
  Double_t eta = T_Muon_Eta->at(iMuon);

  Double_t pt = ScaleLepton(Muon, T_Muon_Pt->at(iMuon));

  Double_t isoCut = -1;

  if (pt <= 20) isoCut = (fabs(eta) < 1.479) ? 0.86 : 0.82;
  else          isoCut = (fabs(eta) < 1.479) ? 0.82 : 0.86;

  Bool_t pass = (T_Muon_MVARings->at(iMuon) > isoCut);

  return pass;
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(UInt_t   iChannel,
				UInt_t   iCut,
				Double_t dd_weight)
{
  // Counters
  //----------------------------------------------------------------------------  
  hCounterRaw[iChannel][iCut][nTight]->Fill(1);
  hCounterPU [iChannel][iCut][nTight]->Fill(1,             efficiency_weight * xs_weight * dd_weight);
  hCounterEff[iChannel][iCut][nTight]->Fill(1, pu_weight * efficiency_weight             * dd_weight);
  hCounter   [iChannel][iCut][nTight]->Fill(1, pu_weight * efficiency_weight * xs_weight * dd_weight);

  hCounterRaw[iChannel][iCut][LLL]->Fill(1);
  hCounterPU [iChannel][iCut][LLL]->Fill(1,             efficiency_weight * xs_weight * dd_weight);
  hCounterEff[iChannel][iCut][LLL]->Fill(1, pu_weight * efficiency_weight             * dd_weight);
  hCounter   [iChannel][iCut][LLL]->Fill(1, pu_weight * efficiency_weight * xs_weight * dd_weight);


  // MC weight histograms
  //----------------------------------------------------------------------------
  hLeptonWeight [theChannel][iCut]->Fill(mc_lepton_weight,  pu_weight);
  hTriggerWeight[theChannel][iCut]->Fill(mc_trigger_weight, pu_weight);
  hTotalWeight  [theChannel][iCut]->Fill(mc_total_weight,   pu_weight);


  // Analysis histograms
  //----------------------------------------------------------------------------  
  Double_t hweight = pu_weight * efficiency_weight * xs_weight * dd_weight;

  Double_t deltaPhi = ZLepton1.DeltaPhi(ZLepton2);
  Double_t deltaR1  = WLepton.DeltaR(ZLepton1);
  Double_t deltaR2  = WLepton.DeltaR(ZLepton2);

  hNPV         [iChannel][iCut]->Fill(T_Vertex_z->size(),         hweight);
  hMET         [iChannel][iCut]->Fill(EventMET.Et(),              hweight);
  hSumCharges  [iChannel][iCut]->Fill(sumCharges,                 hweight);
  hInvMass2Lep [iChannel][iCut]->Fill(invMass2Lep,                hweight);
  hInvMass3Lep [iChannel][iCut]->Fill(invMass3Lep,                hweight);
  hPtLepton1   [iChannel][iCut]->Fill(AnalysisLeptons[0].v.Pt(),  hweight);
  hPtLepton2   [iChannel][iCut]->Fill(AnalysisLeptons[1].v.Pt(),  hweight);
  hPtLepton3   [iChannel][iCut]->Fill(AnalysisLeptons[2].v.Pt(),  hweight);
  hPtLeadingJet[iChannel][iCut]->Fill(ptLeadingJet,               hweight);
  hDPhiZLeptons[iChannel][iCut]->Fill(fabs(deltaPhi),             hweight);
  hPtZLepton1  [iChannel][iCut]->Fill(ZLepton1.Pt(),              hweight);
  hPtZLepton2  [iChannel][iCut]->Fill(ZLepton2.Pt(),              hweight);
  hPtZ         [iChannel][iCut]->Fill((ZLepton1 + ZLepton2).Pt(), hweight);
  hPtWLepton   [iChannel][iCut]->Fill(WLepton.Pt(),               hweight);
  hDRWZLepton1 [iChannel][iCut]->Fill(deltaR1,                    hweight);
  hDRWZLepton2 [iChannel][iCut]->Fill(deltaR2,                    hweight);
  hMtW         [iChannel][iCut]->Fill(transverseMass,             hweight);
  hNJet30      [iChannel][iCut]->Fill(nJet30,                     hweight);
  hNBJet30     [iChannel][iCut]->Fill(nBJet30,                    hweight);
}


//------------------------------------------------------------------------------
// WgammaFilter
//------------------------------------------------------------------------------
const Bool_t AnalysisWZ::WgammaFilter() const
{
  const UInt_t ntauSt3  = T_Gen_TauSt3_PID->size();
  const UInt_t nmuonSt3 = T_Gen_MuonSt3_PID->size();
  const UInt_t nelecSt3 = T_Gen_ElecSt3_PID->size();

  Bool_t low  = false;
  Bool_t high = false;

  if (ntauSt3 >= 2) {
    for (UInt_t i=0; i<ntauSt3; ++i) {
      for (UInt_t j=i+1; j<ntauSt3; ++j) {

	if (T_Gen_TauSt3_PID->at(i) * T_Gen_TauSt3_PID->at(j) > 0) continue;

	TLorentzVector lep1(T_Gen_TauSt3_Px->at(i),
			    T_Gen_TauSt3_Py->at(i),
			    T_Gen_TauSt3_Pz->at(i),
			    T_Gen_TauSt3_Energy->at(i));

	TLorentzVector lep2(T_Gen_TauSt3_Px->at(j),
			    T_Gen_TauSt3_Py->at(j),
			    T_Gen_TauSt3_Pz->at(j),
			    T_Gen_TauSt3_Energy->at(j));
      
	((lep1 + lep2).M() > 12.) ? high = true : low = true;
      }
    }
  }

  if (nmuonSt3 >= 2) {
    for (UInt_t i=0; i<nmuonSt3; ++i) {
      for (UInt_t j=i+1; j<nmuonSt3; ++j) {

	if (T_Gen_MuonSt3_PID->at(i) * T_Gen_MuonSt3_PID->at(j) > 0) continue;

	TLorentzVector lep1(T_Gen_MuonSt3_Px->at(i),
			    T_Gen_MuonSt3_Py->at(i),
			    T_Gen_MuonSt3_Pz->at(i),
			    T_Gen_MuonSt3_Energy->at(i));

	TLorentzVector lep2(T_Gen_MuonSt3_Px->at(j),
			    T_Gen_MuonSt3_Py->at(j),
			    T_Gen_MuonSt3_Pz->at(j),
			    T_Gen_MuonSt3_Energy->at(j));
      
	((lep1 + lep2).M() > 12.) ? high = true : low = true;
      }
    }
  }

  if (nelecSt3 >= 2) {
    for (UInt_t i=0; i<nelecSt3; ++i) {
      for (UInt_t j=i+1; j<nelecSt3; ++j) {

	if (T_Gen_ElecSt3_PID->at(i) * T_Gen_ElecSt3_PID->at(j) > 0) continue;

	TLorentzVector lep1(T_Gen_ElecSt3_Px->at(i),
			    T_Gen_ElecSt3_Py->at(i),
			    T_Gen_ElecSt3_Pz->at(i),
			    T_Gen_ElecSt3_Energy->at(i));

	TLorentzVector lep2(T_Gen_ElecSt3_Px->at(j),
			    T_Gen_ElecSt3_Py->at(j),
			    T_Gen_ElecSt3_Pz->at(j),
			    T_Gen_ElecSt3_Energy->at(j));
      
	((lep1 + lep2).M() > 12.) ? high = true : low = true;
      }
    }
  }

  return (high && !low);
}


//------------------------------------------------------------------------------
// GetPPPWeight
//------------------------------------------------------------------------------
// Weight rules
//
// Tight estimated as PROMPT: p(1-f)
// Tight estimated as   FAKE: f(1-p)
// Fail  estimated as PROMPT: pf
// Fail  estimated as   FAKE: pf
//
// common factor: 1/(p-f)
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetPPPWeight(UInt_t muonJetPt, UInt_t elecJetPt)
{
  Double_t promptProbability[3];

  for (UInt_t i=0; i<3; i++) {
    
    Lepton lep = AnalysisLeptons[i];

    Double_t f = (lep.flavor == Muon) ? lep.frMuon[muonJetPt] : lep.frElec[elecJetPt];

    Double_t p = lep.pr;

    if (lep.type == Tight)
      {
	promptProbability[i] = p * (1 - f);
      }
    else if (lep.type == Fail)
      {
	promptProbability[i] = p * f;
      }

    promptProbability[i] /= (p - f);
  }

  Double_t PPP = promptProbability[0] * promptProbability[1] * promptProbability[2];

  if (nTight == 0 || nTight == 2) PPP *= -1.;

  return PPP;
}


//------------------------------------------------------------------------------
// GetPPFWeight
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetPPFWeight(UInt_t muonJetPt, UInt_t elecJetPt)
{
  Double_t promptProbability[3];
  Double_t fakeProbability[3];

  for (UInt_t i=0; i<3; i++) {
    
    Lepton lep = AnalysisLeptons[i];

    Double_t f = (lep.flavor == Muon) ? lep.frMuon[muonJetPt] : lep.frElec[elecJetPt];

    Double_t p = lep.pr;

    if (lep.type == Tight)
      {
	promptProbability[i] = p * (1 - f);
	fakeProbability[i]   = f * (1 - p);
      }
    else if (lep.type == Fail)
      {
	promptProbability[i] = p * f;
	fakeProbability[i]   = p * f;
      }

    promptProbability[i] /= (p - f);
    fakeProbability[i]   /= (p - f);
  }

  Double_t PPF = promptProbability[0] * promptProbability[1] * fakeProbability[2];
  Double_t PFP = promptProbability[0] * fakeProbability[1]   * promptProbability[2];
  Double_t FPP = fakeProbability[0]   * promptProbability[1] * promptProbability[2];

  Double_t result = PPF + PFP + FPP;

  if (nTight == 1 || nTight == 3) result *= -1.;

  return result;
}


//------------------------------------------------------------------------------
// GetPFFWeight
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetPFFWeight(UInt_t muonJetPt, UInt_t elecJetPt)
{
  Double_t promptProbability[3];
  Double_t fakeProbability[3];

  for (UInt_t i=0; i<3; i++) {

    Lepton lep = AnalysisLeptons[i];

    Double_t f = (lep.flavor == Muon) ? lep.frMuon[muonJetPt] : lep.frElec[elecJetPt];

    Double_t p = lep.pr;

    if (lep.type == Tight)
      {
	promptProbability[i] = p * (1 - f);
	fakeProbability[i]   = f * (1 - p);
      }
    else if (lep.type == Fail)
      {
	promptProbability[i] = p * f;
	fakeProbability[i]   = p * f;
      }

    promptProbability[i] /= (p - f);
    fakeProbability[i]   /= (p - f);
  }

  Double_t PFF = promptProbability[0] * fakeProbability[1]   * fakeProbability[2];
  Double_t FPF = fakeProbability[0]   * promptProbability[1] * fakeProbability[2];
  Double_t FFP = fakeProbability[0]   * fakeProbability[1]   * promptProbability[2];

  Double_t result = PFF + FPF + FFP;

  if (nTight == 0 || nTight == 2) result *= -1.;

  return result;
}


//------------------------------------------------------------------------------
// GetFFFWeight
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetFFFWeight(UInt_t muonJetPt, UInt_t elecJetPt)
{
  Double_t fakeProbability[3];

  for (UInt_t i=0; i<3; i++) {
    
    Lepton lep = AnalysisLeptons[i];

    Double_t f = (lep.flavor == Muon) ? lep.frMuon[muonJetPt] : lep.frElec[elecJetPt];

    Double_t p = lep.pr;

    if (lep.type == Tight)
      {
	fakeProbability[i] = f * (1 - p);
      }
    else if (lep.type == Fail)
      {
	fakeProbability[i] = p * f;
      }

    fakeProbability[i] /= (p - f);
  }

  Double_t FFF = fakeProbability[0] * fakeProbability[1] * fakeProbability[2];

  if (nTight == 1 || nTight == 3) FFF *= -1.;

  return FFF;
}


//------------------------------------------------------------------------------
// LoadHistogram
//------------------------------------------------------------------------------
TH2F* AnalysisWZ::LoadHistogram(TString filename,
				TString hname,
				TString cname)
{
  TString path = (runAtOviedo) ? "/nfs/fanae/user" : "/gpfs/csic_users";

  TFile* inputfile = TFile::Open(path + "/piedra/work/PAF/AuxiliaryFilesWZXS8TeV/" + filename + ".root");

  TH2F* hist = (TH2F*)inputfile->Get(hname)->Clone(cname);
  
  hist->SetDirectory(0);
  
  inputfile->Close();

  return hist;
}


//------------------------------------------------------------------------------
// GetMET
//------------------------------------------------------------------------------
TLorentzVector AnalysisWZ::GetMET()
{
  Double_t px = T_METPFTypeI_ET * cos(T_METPFTypeI_Phi);
  Double_t py = T_METPFTypeI_ET * sin(T_METPFTypeI_Phi);

  if (systematic == metSyst)
    {
      TRandom* random = new TRandom();
      
      px = random->Gaus(px, 0.1 * px);
      py = random->Gaus(py, 0.1 * py);
    }

  Double_t met = sqrt(px*px + py*py);

  TLorentzVector metv(px, py, 0., met);

  return metv;
}


//------------------------------------------------------------------------------
// GetTriggerWeight
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetTriggerWeight()
{
  Double_t eL[3];
  Double_t eT[3];

  for (UInt_t i=0; i<3; i++) {

    Lepton lep = AnalysisLeptons[i];

    eL[i] = lep.lead;
    eT[i] = lep.trail;
  }

  Double_t r1 = (1. - eL[0]) * (1. - eL[1]) * (1. - eL[2]);
  Double_t r2 =       eL[0]  * (1. - eT[1]) * (1. - eT[2]);
  Double_t r3 =       eL[1]  * (1. - eT[0]) * (1. - eT[2]);
  Double_t r4 =       eL[2]  * (1. - eT[0]) * (1. - eT[1]); 

  Double_t triggerWeight = 1. - (r1 + r2 + r3 + r4);
  
  return triggerWeight;
}


//------------------------------------------------------------------------------
// PassTrigger
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::PassTrigger()
{
  if (!isData) return true;
  
  Bool_t pass = false;

  if (sample.Contains("MuEG"))
    {
      pass = T_passTriggerElMu;
    }
  else if (sample.Contains("DoubleMu"))
    {
      pass = (!T_passTriggerElMu && T_passTriggerDoubleMu);
    }
  else if (sample.Contains("DoubleElectron"))
    {
      pass = (!T_passTriggerElMu && !T_passTriggerDoubleMu && T_passTriggerDoubleEl);
    }

  return pass;
}


//------------------------------------------------------------------------------
// ScaleLepton
//------------------------------------------------------------------------------
Double_t AnalysisWZ::ScaleLepton(UInt_t flavor, Double_t pt, Double_t eta)
{
  if (systematic != muonUpSyst     &&
      systematic != muonDownSyst   &&
      systematic != electronUpSyst &&
      systematic != electronDownSyst) return pt;

  Double_t scale = 0.0;

  if (flavor == Muon &&
      (systematic == muonUpSyst || systematic == muonDownSyst))
    {
      scale = 0.002;
    }
  else if (flavor == Electron &&
	   (systematic == electronUpSyst || systematic == electronDownSyst))
    {
      const Double_t InEBMax  = hScaleInEB ->GetXaxis()->GetBinCenter(hScaleInEB ->GetNbinsX());
      const Double_t OutEBMax = hScaleOutEB->GetXaxis()->GetBinCenter(hScaleOutEB->GetNbinsX());
      const Double_t EEMax    = hScaleEE   ->GetXaxis()->GetBinCenter(hScaleEE   ->GetNbinsX());
      
      const Double_t scaleInEB  = hScaleInEB ->GetBinContent(hScaleInEB ->FindBin(min(pt,InEBMax)));
      const Double_t scaleOutEB = hScaleOutEB->GetBinContent(hScaleOutEB->FindBin(min(pt,OutEBMax)));
      const Double_t scaleEE    = hScaleEE   ->GetBinContent(hScaleEE   ->FindBin(min(pt,EEMax)));
      
      if (fabs(eta) < 0.8)
      	{
      	  scale = scaleInEB;
      	}
      else if (fabs(eta) >= 0.8 && fabs(eta) < 1.479)
      	{
      	  scale = scaleOutEB;
      	}
      else
      	{
      	  scale = scaleEE;
      	}
    }
     
  if (systematic == muonDownSyst || systematic == electronDownSyst)
    {
      scale = -scale;
    }


  return (1. + scale) * pt;
}


//------------------------------------------------------------------------------
// GetHistogramFromGraph
//------------------------------------------------------------------------------
TH1F* AnalysisWZ::GetHistogramFromGraph(TString hname, TString gname)
{
  TString path = (runAtOviedo) ? "/nfs/fanae/user" : "/gpfs/csic_users";

  TFile* inputfile = TFile::Open(path + "/piedra/work/PAF/AuxiliaryFilesWZXS8TeV/gScaleSyst-8TeV.root");

  TGraphErrors* graph = (TGraphErrors*)inputfile->Get(gname);

  UInt_t nbins = graph->GetN();

  Double_t* xx = graph->GetX();
  Double_t* yy = graph->GetY();

  Double_t* range = new Double_t[nbins+1];

  for (UInt_t i=0; i<nbins; i++)
    {
      range[i+1] = (xx[i] + xx[i+1]) / 2.;
    }

  range[0]     = xx[0]       - (xx[1]       - xx[0]      ) / 2.;
  range[nbins] = xx[nbins-1] + (xx[nbins-1] - xx[nbins-2]) / 2.;

  TH1F* hist = new TH1F(hname, hname, nbins, range);
  
  hist->SetDirectory(0);

  for (UInt_t i=1; i<=nbins; i++)
    {
      hist->SetBinContent(i, fabs(yy[i-1]));
    }

  inputfile->Close();

  delete [] range;

  return hist;
}
