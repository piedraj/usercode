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

  fPUWeight = new PUWeight(luminosity, Summer12_53X, "2012"); 


  // Histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (UInt_t i=0; i<nChannels; i++) {
    for (UInt_t j=0; j<nCuts; j++) {

      TString suffix = "_" + sChannel[i] + "_" + sCut[j];

      hCounterRaw[i][j] = CreateH1D(TString("hCounterRaw" + suffix), "", 3, 0, 3);
      hCounterPU [i][j] = CreateH1D(TString("hCounterPU"  + suffix), "", 3, 0, 3);
      hCounterEff[i][j] = CreateH1D(TString("hCounterEff" + suffix), "", 3, 0, 3);
      hCounter   [i][j] = CreateH1D(TString("hCounter"    + suffix), "", 3, 0, 3);

      hCounterTTT[i][j] = CreateH1D(TString("hCounterTTT" + suffix), "", 3, 0, 3);
      hCounterTTF[i][j] = CreateH1D(TString("hCounterTTF" + suffix), "", 3, 0, 3);
      hCounterTFF[i][j] = CreateH1D(TString("hCounterTFF" + suffix), "", 3, 0, 3);
      hCounterFFF[i][j] = CreateH1D(TString("hCounterFFF" + suffix), "", 3, 0, 3);

      hNPV         [i][j] = CreateH1D(TString("hNPV"          + suffix), "",  50,  0,  50);
      hMET         [i][j] = CreateH1D(TString("hMET"          + suffix), "", 200,  0, 200);
      hSumCharges  [i][j] = CreateH1D(TString("hSumCharges"   + suffix), "",   9, -4,   5);
      hInvMass3Lep [i][j] = CreateH1D(TString("hInvMass3Lep"  + suffix), "", 400,  0, 400);
      hPtLepton1   [i][j] = CreateH1D(TString("hPtLepton1"    + suffix), "", 200,  0, 200);
      hPtLepton2   [i][j] = CreateH1D(TString("hPtLepton2"    + suffix), "", 200,  0, 200);
      hPtLepton3   [i][j] = CreateH1D(TString("hPtLepton3"    + suffix), "", 200,  0, 200);    
      hPtLeadingJet[i][j] = CreateH1D(TString("hPtLeadingJet" + suffix), "", 200,  0, 200);    

      if (j < HasZCandidate) continue;

      hInvMass2Lep [i][j] = CreateH1D(TString("hInvMass2Lep"  + suffix), "", 200,  0, 200);
      hDPhiZLeptons[i][j] = CreateH1D(TString("hDPhiZLeptons" + suffix), "", 320,  0, 3.2);    
      hPtZLepton1  [i][j] = CreateH1D(TString("hPtZLepton1"   + suffix), "", 200,  0, 200);
      hPtZLepton2  [i][j] = CreateH1D(TString("hPtZLepton2"   + suffix), "", 200,  0, 200);
      hPtWLepton   [i][j] = CreateH1D(TString("hPtWLepton"    + suffix), "", 200,  0, 200);    
      hDRWZLepton1 [i][j] = CreateH1D(TString("hDRWZLepton1"  + suffix), "", 200,  0,   6);    
      hDRWZLepton2 [i][j] = CreateH1D(TString("hDRWZLepton2"  + suffix), "", 200,  0,   6);    
      hMtW         [i][j] = CreateH1D(TString("hMtW"          + suffix), "", 200,  0, 200);    
    }
  }


  // SF, FR and PR histograms
  //----------------------------------------------------------------------------
  MuonSF = LoadHistogram("MuSF_2012",  "muonDATAMCratio_All_selec",      "MuonSF");
  ElecSF = LoadHistogram("EleSF_2012", "electronsDATAMCratio_All_selec", "ElecSF");
  
  MuonPR = LoadHistogram("prompt_rateMuons",     "effDATA_prompt_rate", "MuonPR");
  ElecPR = LoadHistogram("prompt_rateElectrons", "effDATA_All_selec",   "ElecPR");
  
  MuonFR_lo = LoadHistogram("MuFR_2012_jet30",  "h_Muon_signal_pT_eta", "MuonFR_lo");
  ElecFR_lo = LoadHistogram("EleFR_2012_jet35", "fakeElH2",             "ElecFR_lo");

  MuonFR_hi = LoadHistogram("MuFR_2012_jet50",  "h_Muon_signal_pT_eta", "MuonFR_hi");
  ElecFR_hi = LoadHistogram("EleFR_2012_jet50", "fakeElH2",             "ElecFR_hi");
}


//------------------------------------------------------------------------------
// InsideLoop
//------------------------------------------------------------------------------
void AnalysisWZ::InsideLoop()
{
  isData = (sample.Contains("DoubleElectron") || sample.Contains("DoubleMu")) ? 1 : 0;

  pu_weight = (isData) ? 1. : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

  efficiency_weight = pu_weight;

  AnalysisLeptons.clear();

  invMass2Lep     = 999.;
  invMass3Lep     = 999.;
  transverseMass  = 999.;
  sumCharges      = 0.;
  electronCounter = 0;
  tightCounter    = 0;


  // MC filters
  //----------------------------------------------------------------------------
  if (sample.Contains("WGstar") && WgammaFilter()) return;

  if (sample.Contains("ZJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;

  if (sample.Contains("WJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;


  // HLT
  //----------------------------------------------------------------------------
  if (sample.Contains("DoubleMu")       && !T_passTriggerDoubleMu) return;
  if (sample.Contains("DoubleElectron") && !T_passTriggerDoubleEl) return;


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

    const Double_t scale_factor = MuonSF->GetBinContent(MuonSF->FindBin(pt,eta));
    const Double_t prompt_rate  = MuonPR->GetBinContent(MuonPR->FindBin(eta,pt));  // Inverted axis

    if (pt > 35) pt = 34;

    const Double_t fake_rate_lo = MuonFR_lo->GetBinContent(MuonFR_lo->FindBin(pt,eta));
    const Double_t fake_rate_hi = MuonFR_hi->GetBinContent(MuonFR_hi->FindBin(pt,eta));

    Lepton AnalysisMuon;

    AnalysisMuon.index  = i;
    AnalysisMuon.flavor = Muon;
    AnalysisMuon.type   = muon_type;
    AnalysisMuon.charge = T_Muon_Charge->at(i);
    AnalysisMuon.SF     = scale_factor;
    AnalysisMuon.f_lo   = fake_rate_lo;
    AnalysisMuon.f_hi   = fake_rate_hi;
    AnalysisMuon.p      = prompt_rate;
    AnalysisMuon.v      = MuonVector;

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

    const Double_t scale_factor = ElecSF->GetBinContent(ElecSF->FindBin(pt,eta));
    const Double_t prompt_rate  = ElecPR->GetBinContent(ElecPR->FindBin(eta,pt));  // Inverted axis

    if (pt > 35) pt = 34;

    const Double_t fake_rate_lo = ElecFR_lo->GetBinContent(ElecFR_lo->FindBin(pt,eta));
    const Double_t fake_rate_hi = ElecFR_hi->GetBinContent(ElecFR_hi->FindBin(pt,eta));

    Lepton AnalysisElectron;
    
    AnalysisElectron.index  = i;
    AnalysisElectron.flavor = Electron;
    AnalysisElectron.type   = electron_type;
    AnalysisElectron.charge = T_Elec_Charge->at(i);
    AnalysisElectron.SF     = scale_factor;
    AnalysisElectron.f_lo   = fake_rate_lo;
    AnalysisElectron.f_hi   = fake_rate_hi;
    AnalysisElectron.p      = prompt_rate;
    AnalysisElectron.v      = ElectronVector;
    
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
      if (AnalysisLeptons[i].flavor == Electron) electronCounter++;
      if (AnalysisLeptons[i].type   == Tight)    tightCounter++;

      sumCharges += AnalysisLeptons[i].charge;
    }

  if (mode == RAW && tightCounter != 3) return;

  invMass3Lep = (AnalysisLeptons[0].v + AnalysisLeptons[1].v + AnalysisLeptons[2].v).M();

  if      (electronCounter == 0) theChannel = MMM;
  else if (electronCounter == 1) theChannel = MME;
  else if (electronCounter == 2) theChannel = EEM;
  else if (electronCounter == 3) theChannel = EEE;

  if (sample.Contains("DoubleMu")       && electronCounter > 1) return;
  if (sample.Contains("DoubleElectron") && electronCounter < 2) return;


  // Apply lepton SF
  //----------------------------------------------------------------------------
  if (!isData)
    for (UInt_t i=0; i<3; i++) efficiency_weight *= AnalysisLeptons[i].SF;


  // Data-driven estimates
  //----------------------------------------------------------------------------
  if (mode == PPF && tightCounter == 2)
    {
      dataDriven_weight_lo = GetPPFWeightApprx(LowPtJet);
      dataDriven_weight_hi = GetPPFWeightApprx(HighPtJet);
    }
  else if (mode == PPP)
    {
      dataDriven_weight_lo = GetPPPWeight(LowPtJet);
      dataDriven_weight_hi = GetPPPWeight(HighPtJet);
    }
  else
    {
      dataDriven_weight_lo = 1.;
      dataDriven_weight_hi = 1.;
    }

  efficiency_weight *= dataDriven_weight_lo;


  // Deal with jets
  //----------------------------------------------------------------------------
  nbjets       = 0;
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

    nbjets++;
  }


  // Fill histograms with exactly 3 leptons
  //----------------------------------------------------------------------------
  FillHistograms(theChannel, Exactly3Leptons);


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
  if (invMass2Lep < 71. || invMass2Lep > 111.) return;

  if (closure_test && fabs(invMass2Lep - Z_MASS) > 15.) return;


  // Compute the W transverse mass
  //----------------------------------------------------------------------------
  const Double_t met  = T_METPFTypeI_ET;
  const Double_t phi  = T_METPFTypeI_Phi;
  const Double_t lWEt = WLepton.Et();

  TLorentzVector metv(met*cos(phi), met*sin(phi), 0., 0.);

  transverseMass = lWEt*lWEt + met*met - 2.*lWEt*met*cos(WLepton.Angle(metv.Vect()));
  
  transverseMass = sqrt(transverseMass);

  FillHistograms(theChannel, HasZCandidate);


  // HasWCandidate
  //----------------------------------------------------------------------------
  if (!closure_test)
    {
      if (WLepton.Pt() <= 20) return;

      if (WLepton.DeltaR(ZLepton1) < 0.1) return;
      if (WLepton.DeltaR(ZLepton2) < 0.1) return;
    }

  FillHistograms(theChannel, HasWCandidate);


  // MET
  //----------------------------------------------------------------------------
  Bool_t metPass = (T_METPFTypeI_ET > 30);

  if (closure_test) metPass = !metPass;

  if (!metPass) return;

  if (!closure_test)
    {
      efficiency_weight *= (dataDriven_weight_hi / dataDriven_weight_lo);
    }

  FillHistograms(theChannel, MET);


  // SSLike
  //----------------------------------------------------------------------------
  if (fabs(invMass2Lep - Z_MASS) > 15.) return;

  if (T_METPFTypeI_ET < 40) return;

  FillHistograms(theChannel, SSLike);


  // SSLikeAntiBtag
  //----------------------------------------------------------------------------
  if (nbjets > 0) return;

  FillHistograms(theChannel, SSLikeAntiBtag);
}


//------------------------------------------------------------------------------
// SetDataMembersAtTermination
//------------------------------------------------------------------------------
void AnalysisWZ::SetDataMembersAtTermination()
{
  for (UInt_t i=0; i<nChannels; i++) {
    for (UInt_t j=0; j<nCuts; j++) {

      TString suffix = "_" + sChannel[i] + "_" + sCut[j];

      hCounterRaw[i][j] = (TH1D*)FindOutput(TString("hCounterRaw" + suffix));
      hCounterPU [i][j] = (TH1D*)FindOutput(TString("hCounterPU"  + suffix));
      hCounterEff[i][j] = (TH1D*)FindOutput(TString("hCounterEff" + suffix));
      hCounter   [i][j] = (TH1D*)FindOutput(TString("hCounter"    + suffix));

      hCounterTTT[i][j] = (TH1D*)FindOutput(TString("hCounterTTT" + suffix));
      hCounterTTF[i][j] = (TH1D*)FindOutput(TString("hCounterTTF" + suffix));
      hCounterTFF[i][j] = (TH1D*)FindOutput(TString("hCounterTFF" + suffix));
      hCounterFFF[i][j] = (TH1D*)FindOutput(TString("hCounterFFF" + suffix));
    }
  }
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary()
{
  GetParameters();

  ofstream outputfile;

  outputfile.open(Form("%s/%s.txt", directory.Data(), sample.Data()));


  // PU weights applied
  //----------------------------------------------------------------------------
  outputfile << Form("\n %39s results with %7.1f pb [PU weight applied]\n",
		     sample.Data(), luminosity);

  outputfile << Form("\n %18s  %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  for (UInt_t i=0; i<nCuts; i++) {
    
    outputfile << Form(" %18s:", sCut[i].Data());

    for (UInt_t j=0; j<nChannels; j++)
      outputfile << Form(" %10.0f", hCounterPU[j][i]->Integral());

    outputfile << "\n";
  }


  // All weights applied
  //----------------------------------------------------------------------------
  outputfile << Form("\n %39s results with %7.1f pb [all weights applied]\n",
		     sample.Data(), luminosity);

  outputfile << Form("\n %18s  %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  for (UInt_t i=0; i<nCuts; i++) {
      
    outputfile << Form(" %18s:", sCut[i].Data());

    for (UInt_t j=0; j<nChannels; j++)
      outputfile << Form(" %10.0f", hCounter[j][i]->Integral());
    
    outputfile << "\n";
  }


  // No weights applied
  //----------------------------------------------------------------------------
  UInt_t closure_test_cut = MET;

  outputfile << Form("\n %39s results with %7.1f pb [no weights applied]\n",
		     sample.Data(), luminosity);

  outputfile << Form("\n %18s  %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  outputfile << Form(" %18s:", "TTT");
  
  for (UInt_t j=0; j<nChannels; j++) outputfile << Form(" %10.0f", hCounterTTT[j][closure_test_cut]->Integral());

  outputfile << "\n";

  outputfile << Form(" %18s:", "TTF");
  
  for (UInt_t j=0; j<nChannels; j++) outputfile << Form(" %10.0f", hCounterTTF[j][closure_test_cut]->Integral());

  outputfile << "\n";

  outputfile << Form(" %18s:", "TFF");
  
  for (UInt_t j=0; j<nChannels; j++) outputfile << Form(" %10.0f", hCounterTFF[j][closure_test_cut]->Integral());

  outputfile << "\n";

  outputfile << Form(" %18s:", "FFF");
  
  for (UInt_t j=0; j<nChannels; j++) outputfile << Form(" %10.0f", hCounterFFF[j][closure_test_cut]->Integral());

  outputfile << "\n";


  // We are done
  //----------------------------------------------------------------------------
  outputfile << "\n";

  outputfile.close();
}


//------------------------------------------------------------------------------
// GetParameters
//------------------------------------------------------------------------------
void AnalysisWZ::GetParameters()
{
  directory = GetInputParameters()->TheNamedString("directory");
  sample    = GetInputParameters()->TheNamedString("sample");

  GetInputParameters()->TheNamedInt   ("mode",         mode);
  GetInputParameters()->TheNamedInt   ("closure_test", closure_test);
  GetInputParameters()->TheNamedDouble("xs_weight",    xs_weight);
  GetInputParameters()->TheNamedDouble("luminosity",   luminosity);
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
// FillChannelCounters
//------------------------------------------------------------------------------
void AnalysisWZ::FillChannelCounters(UInt_t iChannel, UInt_t iCut)
{
  hCounterRaw[iChannel][iCut]->Fill(1);
  hCounterPU [iChannel][iCut]->Fill(1, pu_weight);
  hCounterEff[iChannel][iCut]->Fill(1, efficiency_weight);
  hCounter   [iChannel][iCut]->Fill(1, efficiency_weight * xs_weight);
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(UInt_t iChannel, UInt_t iCut)
{
  FillChannelCounters(iChannel, iCut);

  if      (tightCounter == 3) hCounterTTT[iChannel][iCut]->Fill(1);
  else if (tightCounter == 2) hCounterTTF[iChannel][iCut]->Fill(1);
  else if (tightCounter == 1) hCounterTFF[iChannel][iCut]->Fill(1);
  else if (tightCounter == 0) hCounterFFF[iChannel][iCut]->Fill(1);

  Double_t hweight = efficiency_weight * xs_weight;
  
  hNPV         [iChannel][iCut]->Fill(T_Vertex_z->size(),        hweight);
  hMET         [iChannel][iCut]->Fill(T_METPFTypeI_ET,           hweight);
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

    Double_t f = (jetPt == LowPtJet) ? lep.f_lo : lep.f_hi;

    Double_t p = lep.p;

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

  if (tightCounter == 3) result *= -1.;

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

    Double_t f = (jetPt == LowPtJet) ? lep.f_lo : lep.f_hi;

    Double_t p = lep.p;

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

  if (tightCounter == 2) PPP *= -1.;

  return PPP;
}


//------------------------------------------------------------------------------
// GetPPFWeightApprx
//------------------------------------------------------------------------------
Double_t AnalysisWZ::GetPPFWeightApprx(UInt_t jetPt)
{
  Double_t weight = 1.;

  for (UInt_t i=0; i<3; i++) {

    Lepton lep = AnalysisLeptons[i];

    Double_t f = (jetPt == LowPtJet) ? lep.f_lo : lep.f_hi;

    if (lep.type == Fail) weight *= f / (1. - f);
  }

  return weight;
}


//------------------------------------------------------------------------------
// LoadHistogram
//------------------------------------------------------------------------------
TH2F* AnalysisWZ::LoadHistogram(TString filename,
				TString hname,
				TString cname)
{
  TFile* inputfile = TFile::Open("/nfs/fanae/user/piedra/work/PAF/LeptonScaleFactors/" + filename + ".root");

  //  TFile* inputfile = TFile::Open("/gpfs/csic_users/piedra/work/PAF/LeptonScaleFactors/" + filename + ".root");

  TH2F* hist = (TH2F*)inputfile->Get(hname)->Clone(cname);
  
  hist->SetDirectory(0);
  
  inputfile->Close();

  return hist;
}
