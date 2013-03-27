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
      hInvMass3Lep [i][j] = CreateH1D("hInvMass3Lep"  + suffix, "", 400,  0, 400);
      hPtLepton1   [i][j] = CreateH1D("hPtLepton1"    + suffix, "", 200,  0, 200);
      hPtLepton2   [i][j] = CreateH1D("hPtLepton2"    + suffix, "", 200,  0, 200);
      hPtLepton3   [i][j] = CreateH1D("hPtLepton3"    + suffix, "", 200,  0, 200);    
      hPtLeadingJet[i][j] = CreateH1D("hPtLeadingJet" + suffix, "", 200,  0, 200);    
      
      if (j < HasZCandidate) continue;
      
      hInvMass2Lep [i][j] = CreateH1D("hInvMass2Lep"  + suffix, "", 200,  0, 200);
      hDPhiZLeptons[i][j] = CreateH1D("hDPhiZLeptons" + suffix, "", 320,  0, 3.2);    
      hPtZLepton1  [i][j] = CreateH1D("hPtZLepton1"   + suffix, "", 200,  0, 200);
      hPtZLepton2  [i][j] = CreateH1D("hPtZLepton2"   + suffix, "", 200,  0, 200);
      hPtWLepton   [i][j] = CreateH1D("hPtWLepton"    + suffix, "", 200,  0, 200);    
      hDRWZLepton1 [i][j] = CreateH1D("hDRWZLepton1"  + suffix, "", 200,  0,   6);    
      hDRWZLepton2 [i][j] = CreateH1D("hDRWZLepton2"  + suffix, "", 200,  0,   6);    
      hMtW         [i][j] = CreateH1D("hMtW"          + suffix, "", 200,  0, 200);    
    }
  }


  // SF, FR, PR and trigger efficiencies histograms
  //----------------------------------------------------------------------------
  MuonSF = LoadHistogram("MuSF_2012",  "h2inverted", "MuonSF");
  ElecSF = LoadHistogram("EleSF_2012", "h2inverted", "ElecSF");
  MuonPR = LoadHistogram("MuPR_2012",  "h2inverted", "MuonPR");
  ElecPR = LoadHistogram("ElePR_2012", "h2inverted", "ElecPR");

  MuonFR[Jet15] = LoadHistogram("MuFR_Moriond13_jet15_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet15");
  MuonFR[Jet30] = LoadHistogram("MuFR_Moriond13_jet30_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet30");
  MuonFR[Jet50] = LoadHistogram("MuFR_Moriond13_jet50_EWKcorr", "FR_pT_eta_EWKcorr", "MuonFR_Jet50");

  ElecFR[Jet15] = LoadHistogram("EleFR_Moriond13_jet15_EWKcorr", "fakeElH2", "ElecFR_Jet15");
  ElecFR[Jet30] = LoadHistogram("EleFR_Moriond13_jet35_EWKcorr", "fakeElH2", "ElecFR_Jet30");
  ElecFR[Jet50] = LoadHistogram("EleFR_Moriond13_jet50_EWKcorr", "fakeElH2", "ElecFR_Jet50");

  DoubleElLead  = LoadHistogram("triggerEfficiencies", "DoubleElLead",  "DoubleElLead");
  DoubleMuLead  = LoadHistogram("triggerEfficiencies", "DoubleMuLead",  "DoubleMuLead");
  DoubleElTrail = LoadHistogram("triggerEfficiencies", "DoubleElTrail", "DoubleElTrail");
  DoubleMuTrail = LoadHistogram("triggerEfficiencies", "DoubleMuTrail", "DoubleMuTrail");
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

  efficiency_weight = pu_weight;
  mc_lepton_weight  = 1.;
  mc_trigger_weight = 1.;
  mc_total_weight   = 1.;

  AnalysisLeptons.clear();

  invMass2Lep    = 999.;
  invMass3Lep    = 999.;
  transverseMass = 999.;
  sumCharges     = 0.;

  nBJet     = 0;
  nElectron = 0;
  nTight    = 0;

  for (UInt_t j=0; j<nFakeRate; j++) dataDriven_weight[j] = 1.;

  EventMET = GetMET();


  // MC filters
  //----------------------------------------------------------------------------
  if (sample.Contains("WGstar") && WgammaFilter()) return;

  if (sample.Contains("ZJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;

  if (sample.Contains("WJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;


  // Loop over muons
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Muon_Px->size(); i++) {

    TLorentzVector MuonVector(T_Muon_Px->at(i),
			      T_Muon_Py->at(i),
			      T_Muon_Pz->at(i),
			      T_Muon_Energy->at(i));
    
    Double_t pt = MuonVector.Pt();

    Double_t eta = fabs(MuonVector.Eta());

    if (pt <= 10.) continue;

    if (eta >= 2.4) continue;

    Bool_t isnotglobal = ( T_Muon_IsAllStandAloneMuons->at(i)
			   && !T_Muon_IsGlobalMuon->at(i)
			   && !T_Muon_IsAllTrackerMuons->at(i) );

    if (isnotglobal) continue;

    if (fabs(T_Muon_IP2DBiasedPV->at(i)) > 0.2) continue;

    if (fabs(T_Muon_dzPVBiasedPV->at(i)) > 0.1) continue;

    if (T_Muon_MVARings->at(i) < -0.6) continue;

    if (!MuonID(i)) continue;

    UInt_t muon_type = (MuonCloseToPV(i) && MuonIsolation(i)) ? Tight : Fail;

    const Double_t sfMax = MuonSF->GetXaxis()->GetBinCenter(MuonSF->GetNbinsX());
    const Double_t prMax = MuonPR->GetXaxis()->GetBinCenter(MuonPR->GetNbinsX());
    const Double_t trMax = DoubleMuLead->GetXaxis()->GetBinCenter(DoubleMuLead->GetNbinsX());

    Lepton AnalysisMuon;

    AnalysisMuon.index  = i;
    AnalysisMuon.flavor = Muon;
    AnalysisMuon.type   = muon_type;
    AnalysisMuon.charge = T_Muon_Charge->at(i);
    AnalysisMuon.v      = MuonVector;
    AnalysisMuon.sf     = MuonSF->GetBinContent(MuonSF->FindBin(min(pt,sfMax),eta));
    AnalysisMuon.pr     = MuonPR->GetBinContent(MuonPR->FindBin(min(pt,prMax),eta));
    AnalysisMuon.lead   = DoubleMuLead ->GetBinContent(DoubleMuLead ->FindBin(min(pt,trMax),eta));
    AnalysisMuon.trail  = DoubleMuTrail->GetBinContent(DoubleMuTrail->FindBin(min(pt,trMax),eta));

    for (UInt_t j=0; j<nFakeRate; j++)
      AnalysisMuon.fr[j] = MuonFR[j]->GetBinContent(MuonFR[j]->FindBin(min(pt,35.),eta));

    AnalysisLeptons.push_back(AnalysisMuon);
  }


  // Loop over electrons
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Elec_Px->size(); i++) {

    TLorentzVector ElectronVector(T_Elec_Px->at(i),
				  T_Elec_Py->at(i),
				  T_Elec_Pz->at(i),
				  T_Elec_Energy->at(i));

    Double_t pt = ElectronVector.Pt();

    Double_t eta = fabs(ElectronVector.Eta());

    if (pt <= 10.) continue;

    if (eta >= 2.5) continue;

    if (!ElectronID(i)) continue;

    if (!ElectronCloseToPV(i)) continue;

    UInt_t electron_type = (ElectronBDT(i) && ElectronIsolation(i)) ? Tight : Fail;

    const Double_t sfMax = ElecSF->GetXaxis()->GetBinCenter(ElecSF->GetNbinsX());
    const Double_t prMax = ElecPR->GetXaxis()->GetBinCenter(ElecPR->GetNbinsX());
    const Double_t trMax = DoubleElLead->GetXaxis()->GetBinCenter(DoubleElLead->GetNbinsX());

    Lepton AnalysisElectron;
    
    AnalysisElectron.index  = i;
    AnalysisElectron.flavor = Electron;
    AnalysisElectron.type   = electron_type;
    AnalysisElectron.charge = T_Elec_Charge->at(i);
    AnalysisElectron.v      = ElectronVector;
    AnalysisElectron.sf     = ElecSF->GetBinContent(ElecSF->FindBin(min(pt,sfMax),eta));
    AnalysisElectron.pr     = ElecPR->GetBinContent(ElecPR->FindBin(min(pt,prMax),eta));
    AnalysisElectron.lead   = DoubleElLead ->GetBinContent(DoubleElLead ->FindBin(min(pt,trMax),eta));
    AnalysisElectron.trail  = DoubleElTrail->GetBinContent(DoubleElTrail->FindBin(min(pt,trMax),eta));

    for (UInt_t j=0; j<nFakeRate; j++)
      AnalysisElectron.fr[j] = ElecFR[j]->GetBinContent(ElecFR[j]->FindBin(min(pt,35.),eta));

    AnalysisLeptons.push_back(AnalysisElectron);
  }


  // Sort the leptons by pt
  //----------------------------------------------------------------------------
  if (AnalysisLeptons.size() != 3) return;

  std::sort(AnalysisLeptons.begin(), AnalysisLeptons.end());

  std::reverse(AnalysisLeptons.begin(), AnalysisLeptons.end());

  Bool_t pt3pass = (AnalysisLeptons[2].v.Pt() > 20.);
  
  if (closure_test) pt3pass = !pt3pass;
  
  if (!pt3pass) return;


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

  if (sample.Contains("DoubleMu")       && nElectron > 1) return;
  if (sample.Contains("DoubleElectron") && nElectron < 2) return;

  
  // HLT
  //----------------------------------------------------------------------------
  if (!PassTrigger()) return;


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
  if (mode == PPF)
    {
      for (UInt_t j=0; j<nFakeRate; j++) dataDriven_weight[j] = GetPPFWeight(j);
    }
  else if (mode == PPP)
    {
      for (UInt_t j=0; j<nFakeRate; j++) dataDriven_weight[j] = GetPPPWeight(j);
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

    if (Jet.Pt() < 40) continue;

    if (fabs(Jet.Eta()) > 2.4) continue;

    if (T_JetAKCHS_Tag_CombSVtx->at(i) < 0.244) continue;

    nBJet++;
  }


  // Fill histograms with exactly 3 leptons
  //----------------------------------------------------------------------------
  FillHistograms(theChannel, Exactly3Leptons, dataDriven_weight[Jet30]);


  // Make Z and W candidates
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<3; i++) {

    if (closure_test && i != 0) continue;

    for (UInt_t j=i+1; j<3; j++) {
      
      if (closure_test && j != 1) continue;

      if (AnalysisLeptons[i].flavor != AnalysisLeptons[j].flavor) continue;

      if (AnalysisLeptons[i].charge * AnalysisLeptons[j].charge > 0) continue;

      Double_t inv_mass = (AnalysisLeptons[i].v + AnalysisLeptons[j].v).M();

      if (fabs(inv_mass - Z_MASS) < fabs(invMass2Lep - Z_MASS)) {

	invMass2Lep = inv_mass;

	ZLepton1 = AnalysisLeptons[i].v;
	ZLepton2 = AnalysisLeptons[j].v;

	for (UInt_t k=0; k<3; k++) {
	
	  if (k == i) continue;
	  if (k == j) continue;

	  WLepton = AnalysisLeptons[k].v;
	}
      }
    }
  }

  if (invMass2Lep < 12.) return;


  // HasZCandidate
  //----------------------------------------------------------------------------
  if (fabs(invMass2Lep - Z_MASS) > 20.) return;

  if (closure_test && fabs(invMass2Lep - Z_MASS) > 15.) return;

  const Double_t met  = EventMET.Et();
  const Double_t lWEt = WLepton.Et();

  transverseMass = lWEt*lWEt + met*met - 2.*lWEt*met*cos(WLepton.Angle(EventMET.Vect()));
  
  transverseMass = sqrt(transverseMass);

  FillHistograms(theChannel, HasZCandidate, dataDriven_weight[Jet30]);


  // HasWCandidate
  //----------------------------------------------------------------------------
  if (!closure_test)
    {
      if (WLepton.DeltaR(ZLepton1) < 0.1) return;
      if (WLepton.DeltaR(ZLepton2) < 0.1) return;
    }

  FillHistograms(theChannel, HasWCandidate, dataDriven_weight[Jet30]);


  // ATLAS
  //----------------------------------------------------------------------------
  if (AnalysisLeptons[0].v.Pt() > 25  &&
      AnalysisLeptons[2].v.Pt() > 15  &&
      fabs(invMass2Lep - Z_MASS) < 10 &&
      EventMET.Et() > 25              &&
      transverseMass > 20)
    {
      FillHistograms(theChannel, ATLAS, dataDriven_weight[Jet50]);
    }


  // MET30
  //----------------------------------------------------------------------------
  Bool_t metPass = (EventMET.Et() > 30.);

  if (closure_test) metPass = !metPass;

  if (!metPass) return;

  if (closure_test) FillHistograms(theChannel, MET30, dataDriven_weight[Jet30]);
  else              FillHistograms(theChannel, MET30, dataDriven_weight[Jet50]);


  // MET40
  //----------------------------------------------------------------------------
  if (EventMET.Et() < 40.) return;

  FillHistograms(theChannel, MET40, dataDriven_weight[Jet50]);


  // MET40AntiBtag
  //----------------------------------------------------------------------------
  if (nBJet > 0) return;

  FillHistograms(theChannel, MET40AntiBtag, dataDriven_weight[Jet50]);
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

  GetInputParameters()->TheNamedDouble("luminosity",     luminosity);
  GetInputParameters()->TheNamedDouble("pu_luminosity",  pu_luminosity);
  GetInputParameters()->TheNamedDouble("xs_weight",      xs_weight);
  GetInputParameters()->TheNamedInt   ("mode",           mode);
  GetInputParameters()->TheNamedInt   ("systematic",     systematic);
  GetInputParameters()->TheNamedInt   ("closure_test",   closure_test);
  GetInputParameters()->TheNamedInt   ("runAtOviedo",    runAtOviedo);
}


//------------------------------------------------------------------------------
// ElectronBDT
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::ElectronBDT(UInt_t iElec)
{
  Double_t eta = fabs(T_Elec_SC_Eta->at(iElec));
	
  Double_t mvaCut = 999;

  if (T_Elec_Pt->at(iElec) < 20)
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
  Double_t sigmaietaieta = T_Elec_sigmaIetaIeta->at(iElec);
  Double_t deltaPhiIn    = T_Elec_deltaPhiIn   ->at(iElec);
  Double_t deltaEtaIn    = T_Elec_deltaEtaIn   ->at(iElec);
  Double_t HtoE          = T_Elec_HtoE         ->at(iElec);
  Double_t pt            = T_Elec_Pt           ->at(iElec);
  Double_t trkIso03      = T_Elec_dr03TkSumPt  ->at(iElec);
  Double_t hadIso03      = T_Elec_dr03HcalSumEt->at(iElec);
  Double_t emIso03       = T_Elec_dr03EcalSumEt->at(iElec);
  Double_t max_emIso03   = std::max(emIso03-1., 0.);

  Bool_t pass = false;
	
  if (fabs(T_Elec_SC_Eta->at(iElec)) < 1.479)
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

  pass &= (T_Elec_nHits->at(iElec) <= 0);

  return pass;
}


//------------------------------------------------------------------------------
// ElectronCloseToPV
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::ElectronCloseToPV(UInt_t iElec) 
{
  Bool_t pass = true;

  pass &= (fabs(T_Elec_dzPVBiasedPV->at(iElec)) <= 0.10);
  pass &= (fabs(T_Elec_IP2DBiasedPV->at(iElec)) <= 0.02);

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

  Double_t AEff04 = 0.;

  if (fabs(Elec.Eta()) < 1.0)
    {
      AEff04 = 0.21;  // ± 0.001
    }
  else if (fabs(Elec.Eta()) >= 1.0 && fabs(Elec.Eta()) < 1.479)
    {
      AEff04 = 0.21;  // ± 0.002
    }
  else if (fabs(Elec.Eta()) >= 1.479 && fabs(Elec.Eta()) < 2.0)
    {
      AEff04 = 0.11;  // ± 0.001
    }
  else if (fabs(Elec.Eta()) >= 2.0 && fabs(Elec.Eta()) < 2.2)
    {
      AEff04 = 0.14;  // ± 0.001
    }
  else if (fabs(Elec.Eta()) >= 2.2 && fabs(Elec.Eta()) < 2.3)
    {
      AEff04 = 0.18;  // ± 0.002
    }
  else if (fabs(Elec.Eta()) >= 2.3 && fabs(Elec.Eta()) < 2.4)
    {
      AEff04 = 0.19;  // ± 0.003
    }
  else if (fabs(Elec.Eta()) >= 2.4)
    {
      AEff04 = 0.26;  // ± 0.004
    }

  Double_t charged = T_Elec_chargedHadronIso->at(iElec);
  Double_t neutral = T_Elec_neutralHadronIso->at(iElec);
  Double_t photon  = T_Elec_photonIso->at(iElec);
  Double_t rho     = T_Event_RhoIso;

  Double_t relIso = (charged + max(0., neutral + photon - rho*AEff04)) / Elec.Pt();

  Bool_t pass = (relIso < 0.15);

  return pass;
}


//------------------------------------------------------------------------------
// MuonID
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonID(UInt_t iMuon)
{
  Double_t ptResolution = T_Muon_deltaPt->at(iMuon) / T_Muon_Pt->at(iMuon);

  Bool_t passcutsforGlb = T_Muon_IsGlobalMuon->at(iMuon);

  passcutsforGlb &= (T_Muon_NormChi2GTrk->at(iMuon) < 10);
  passcutsforGlb &= (T_Muon_NumOfMatches->at(iMuon) >  1);
	
  Bool_t passcutsforSA = T_Muon_IsAllTrackerMuons->at(iMuon);

  passcutsforSA &= T_Muon_IsTMLastStationTight->at(iMuon);

  Bool_t pass = (passcutsforGlb || passcutsforSA);

  pass &= (fabs(ptResolution) < 0.1);
	
  pass &= T_Muon_isPFMuon->at(iMuon);

  pass &= (T_Muon_NLayers->at(iMuon) > 5);

  return pass;
}


//------------------------------------------------------------------------------
// MuonCloseToPV
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonCloseToPV(UInt_t iMuon)
{
  Double_t MaxMuIP = (T_Muon_Pt->at(iMuon) < 20) ? 0.01 : 0.02;

  Bool_t pass = (fabs(T_Muon_IP2DBiasedPV->at(iMuon)) <= MaxMuIP);

  return pass;
}


//------------------------------------------------------------------------------
// MuonIsolation
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonIsolation(UInt_t iMuon) 
{
  Double_t eta = T_Muon_Eta->at(iMuon);
  Double_t pt  = T_Muon_Pt ->at(iMuon);

  Double_t isoCut = -1;

  if (pt <= 20) isoCut = (fabs(eta) < 1.479) ? 0.82 : 0.86;
  else          isoCut = (fabs(eta) < 1.479) ? 0.86 : 0.82;

  Bool_t pass = (T_Muon_MVARings->at(iMuon) >= isoCut);

  return pass;
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(UInt_t   iChannel,
				UInt_t   iCut,
				Double_t dd_weight)
{
  Double_t hweight = efficiency_weight * xs_weight * dd_weight;


  // Counters
  //----------------------------------------------------------------------------  
  hCounterRaw[iChannel][iCut][nTight]->Fill(1);
  hCounterPU [iChannel][iCut][nTight]->Fill(1, pu_weight);
  hCounterEff[iChannel][iCut][nTight]->Fill(1, efficiency_weight * dd_weight);
  hCounter   [iChannel][iCut][nTight]->Fill(1, hweight);

  hCounterRaw[iChannel][iCut][LLL]->Fill(1);
  hCounterPU [iChannel][iCut][LLL]->Fill(1, pu_weight);
  hCounterEff[iChannel][iCut][LLL]->Fill(1, efficiency_weight * dd_weight);
  hCounter   [iChannel][iCut][LLL]->Fill(1, hweight);


  // MC weight histograms
  //----------------------------------------------------------------------------
  hLeptonWeight [theChannel][iCut]->Fill(mc_lepton_weight,  pu_weight);
  hTriggerWeight[theChannel][iCut]->Fill(mc_trigger_weight, pu_weight);
  hTotalWeight  [theChannel][iCut]->Fill(mc_total_weight,   pu_weight);


  // Analysis histograms
  //----------------------------------------------------------------------------  
  hNPV         [iChannel][iCut]->Fill(T_Vertex_z->size(),        hweight);
  hMET         [iChannel][iCut]->Fill(EventMET.Et(),             hweight);
  hSumCharges  [iChannel][iCut]->Fill(sumCharges,                hweight);
  hInvMass3Lep [iChannel][iCut]->Fill(invMass3Lep,               hweight);
  hPtLepton1   [iChannel][iCut]->Fill(AnalysisLeptons[0].v.Pt(), hweight);
  hPtLepton2   [iChannel][iCut]->Fill(AnalysisLeptons[1].v.Pt(), hweight);
  hPtLepton3   [iChannel][iCut]->Fill(AnalysisLeptons[2].v.Pt(), hweight);
  hPtLeadingJet[iChannel][iCut]->Fill(ptLeadingJet,              hweight);

  if (iCut < HasZCandidate) return;

  Double_t deltaPhi = ZLepton1.DeltaPhi(ZLepton2);
  Double_t deltaR1  = WLepton.DeltaR(ZLepton1);
  Double_t deltaR2  = WLepton.DeltaR(ZLepton2);

  hInvMass2Lep [iChannel][iCut]->Fill(invMass2Lep,    hweight);
  hDPhiZLeptons[iChannel][iCut]->Fill(fabs(deltaPhi), hweight);
  hPtZLepton1  [iChannel][iCut]->Fill(ZLepton1.Pt(),  hweight);
  hPtZLepton2  [iChannel][iCut]->Fill(ZLepton2.Pt(),  hweight);
  hPtWLepton   [iChannel][iCut]->Fill(WLepton.Pt(),   hweight);
  hDRWZLepton1 [iChannel][iCut]->Fill(deltaR1,        hweight);
  hDRWZLepton2 [iChannel][iCut]->Fill(deltaR2,        hweight);
  hMtW         [iChannel][iCut]->Fill(transverseMass, hweight);
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
// GetPPFWeight
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
Double_t AnalysisWZ::GetPPFWeight(UInt_t jetPt)
{
  Double_t promptProbability[3];
  Double_t fakeProbability[3];

  for (UInt_t i=0; i<3; i++) {
    
    Lepton lep = AnalysisLeptons[i];

    Double_t f = lep.fr[jetPt];
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

  if (nTight == 1) result *= -1.;
  if (nTight == 3) result *= -1.;

  return result;
}


//------------------------------------------------------------------------------
// GetPPPWeight
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetPPPWeight(UInt_t jetPt)
{
  Double_t promptProbability[3];

  for (UInt_t i=0; i<3; i++) {
    
    Lepton lep = AnalysisLeptons[i];

    Double_t f = lep.fr[jetPt];
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

  if (nTight == 0) PPP *= -1.;
  if (nTight == 2) PPP *= -1.;

  return PPP;
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

  TLorentzVector metv(px, py, 0.0, met);

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

  if (sample.Contains("DoubleMu"))
    {
      pass = T_passTriggerDoubleMu;
    }
  else if (sample.Contains("DoubleElectron"))
    {
      pass = T_passTriggerDoubleEl;
    }
  else if (sample.Contains("MuEG"))
    {
      pass = T_passTriggerElMu;

      if (nElectron > 1)
	{
	  pass &= (!T_passTriggerDoubleEl);
	}
      else
	{
	  pass &= (!T_passTriggerDoubleMu);
	}
    }

  return pass;
}
