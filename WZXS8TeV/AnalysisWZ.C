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

      hNPV[i][j] = CreateH1D(TString("hNPV" + suffix), "",  60, 0,  60);
      hMET[i][j] = CreateH1D(TString("hMET" + suffix), "", 200, 0, 200);

      if (j < Exactly3Leptons) continue;

      hSumCharges  [i][j] = CreateH1D(TString("hSumCharges"   + suffix), "",   9, -4,    5);
      hInvMass2Lep1[i][j] = CreateH1D(TString("hInvMass2Lep1" + suffix), "", 200,  0,  200);
      hInvMass2Lep2[i][j] = CreateH1D(TString("hInvMass2Lep2" + suffix), "", 200,  0,  200);
      hInvMass3Lep [i][j] = CreateH1D(TString("hInvMass3Lep"  + suffix), "", 500,  0, 1000);
      hPtLepton1   [i][j] = CreateH1D(TString("hPtLepton1"    + suffix), "", 200,  0,  200);
      hPtLepton2   [i][j] = CreateH1D(TString("hPtLepton2"    + suffix), "", 200,  0,  200);
      hPtLepton3   [i][j] = CreateH1D(TString("hPtLepton3"    + suffix), "", 200,  0,  200);    

      if (j < HasWCandidate) continue;

      hPtZLepton1[i][j] = CreateH1D(TString("hPtZLepton1" + suffix), "", 200,  0, 200);
      hPtZLepton2[i][j] = CreateH1D(TString("hPtZLepton2" + suffix), "", 200,  0, 200);
      hPtWLepton [i][j] = CreateH1D(TString("hPtWLepton"  + suffix), "", 200,  0, 200);    
      hInvMassZ  [i][j] = CreateH1D(TString("hInvMassZ"   + suffix), "",  80, 71, 111);
    }
  }
}


//------------------------------------------------------------------------------
// InsideLoop
//------------------------------------------------------------------------------
void AnalysisWZ::InsideLoop()
{
  isData = (sample.Contains("DoubleElectron") || sample.Contains("DoubleMu")) ? 1 : 0;

  pu_weight = (isData) ? 1.0 : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

  efficiency_weight = pu_weight;

  Muons_Charge.clear();
  Electrons_Charge.clear();

  Muons.clear();
  Electrons.clear();

  dileptonInvMass =  0;
  nSelMuon        =  0;
  nSelElec        =  0;
  theChannel      = -1;


  // Accept only WZ events inside the Z window
  //----------------------------------------------------------------------
  if (sample.Contains("WZTo3LNu") && !isSignalMCInsideZmassRange(71,111)) return;


  // AllEvents
  //----------------------------------------------------------------------------
  if (!FillCounters(999, 999, AllEvents)) return;


  // HLT
  //----------------------------------------------------------------------------
  if (sample.Contains("DoubleMu")       && !T_passTriggerDoubleMu) return;
  if (sample.Contains("DoubleElectron") && !T_passTriggerDoubleEl) return;

  if (!FillCounters(999, 999, HLT)) return;


  // Loop over muons
  //----------------------------------------------------------------------------
  UInt_t countMuons    = 0;
  UInt_t countPVMuons  = 0;
  UInt_t countIsoMuons = 0;

  for (UInt_t i=0; i<T_Muon_Px->size(); i++) {

    TLorentzVector Muon(T_Muon_Px->at(i),
			T_Muon_Py->at(i),
			T_Muon_Pz->at(i),
			T_Muon_Energy->at(i));

    if (Muon.Pt() <= 10) continue;

    if (fabs(Muon.Eta()) >= 2.4) continue;

    Bool_t isnotglobal = ( T_Muon_IsAllStandAloneMuons->at(i)
			   && !T_Muon_IsGlobalMuon->at(i)
			   && !T_Muon_IsAllTrackerMuons->at(i) );

    if (isnotglobal) continue;

    countMuons++;

    if (!MuonCloseToPV(i)) continue;

    countPVMuons++;

    if (!MuonIsolation(i)) continue;

    countIsoMuons++;

    if (!MuonID(i)) continue;

    Muons_Charge.push_back(T_Muon_Charge->at(i));

    Muons.push_back(Muon);
  }

  nSelMuon = Muons.size();


  // Loop over electrons
  //----------------------------------------------------------------------------
  UInt_t countElectrons    = 0;
  UInt_t countPVElectrons  = 0;
  UInt_t countIsoElectrons = 0;

  for (UInt_t i=0; i<T_Elec_Px->size(); i++) {

    TLorentzVector Elec(T_Elec_Px->at(i),
			T_Elec_Py->at(i),
			T_Elec_Pz->at(i),
			T_Elec_Energy->at(i));

    if (Elec.Pt() <= 10) continue;

    if (fabs(Elec.Eta()) >= 2.5) continue;

    if (!ElectronID(i)) continue;

    countElectrons++;

    if (!ElectronCloseToPV(i)) continue;

    countPVElectrons++;

    if (!ElectronIsolation(i)) continue;

    if (!ElectronBDT(i)) continue;
    
    countIsoElectrons++;

    Electrons_Charge.push_back(T_Elec_Charge->at(i));
	
    Electrons.push_back(Elec);
  }

  nSelElec = Electrons.size();


  if (!FillCounters(countElectrons,    countMuons,    Has2Leptons))        return;
  if (!FillCounters(countPVElectrons,  countPVMuons,  Has2PVLeptons))      return;
  if (!FillCounters(countIsoElectrons, countIsoMuons, Has2IsoLeptons))     return;
  if (!FillCounters(nSelElec,          nSelMuon,      Has2IsoGoodLeptons)) return;


  // Debug histograms to understand MMM lack of MC
  //----------------------------------------------------------------------------
  if (nSelElec >= 2)
    {
      hNPV[EEE][Has2IsoGoodLeptons]->Fill(T_Vertex_z->size(), efficiency_weight * xs_weight);
      hMET[EEE][Has2IsoGoodLeptons]->Fill(T_METPFTypeI_ET,    efficiency_weight * xs_weight);
    }
  if (nSelMuon >= 2)
    {
      hNPV[MMM][Has2IsoGoodLeptons]->Fill(T_Vertex_z->size(), efficiency_weight * xs_weight);
      hMET[MMM][Has2IsoGoodLeptons]->Fill(T_METPFTypeI_ET,    efficiency_weight * xs_weight);
    }


  // Exactly3Leptons
  //----------------------------------------------------------------------------
  if ((nSelMuon + nSelElec) != 3) return;

  if (sample.Contains("DoubleMu")       && nSelMuon < 2) return;
  if (sample.Contains("DoubleElectron") && nSelElec < 2) return;

  if      (nSelMuon == 3) theChannel = MMM;
  else if (nSelElec == 3) theChannel = EEE;
  else if (nSelMuon == 2) theChannel = MME;
  else if (nSelElec == 2) theChannel = EEM;


  // Require at least two leptons with pt > 20 GeV
  //----------------------------------------------------------------------------
  UInt_t nHighPtElec = 0;
  UInt_t nHighPtMuon = 0;

  for (UInt_t i=0; i<nSelElec; i++) if (Electrons[i].Pt() > 20) nHighPtElec++;
  for (UInt_t i=0; i<nSelMuon; i++) if (Muons    [i].Pt() > 20) nHighPtMuon++;

  if ((nHighPtElec + nHighPtMuon) < 3) return;  // Replace 3 by 2

  if (nSelMuon >= 2 && nHighPtMuon < 1) return;
  if (nSelElec >= 2 && nHighPtElec < 1) return;

  FillHistograms(theChannel, Exactly3Leptons);


  // HasZCandidate
  //----------------------------------------------------------------------------
  if (nSelElec >= 2) {
    for (UInt_t i=0; i<nSelElec; i++) {
      for (UInt_t j=i+1; j<nSelElec; j++) {

	if (Electrons_Charge[i]*Electrons_Charge[j] > 0) continue;

	Double_t invMass = (Electrons[i] + Electrons[j]).M();

	if (invMass < 71 || invMass > 111) continue;

	if (fabs(invMass - Z_MASS) < fabs(dileptonInvMass - Z_MASS)) {

	  dileptonInvMass = invMass;

	  ZLepton1 = Electrons[i];
	  ZLepton2 = Electrons[j];

	  if (nSelElec == 3)
	    for (UInt_t k=0; k<nSelElec; k++)
	      if (k != i && k != j) WLepton = Electrons[k];
	}
      }
    }
  }
  else if (nSelMuon >= 2) {
    for (UInt_t i=0; i<nSelMuon; i++) {
      for (UInt_t j=i+1; j<nSelMuon; j++) {

	if (Muons_Charge[i]*Muons_Charge[j] > 0) continue;

	Double_t invMass = (Muons[i] + Muons[j]).M();

	if (invMass < 71 || invMass > 111) continue;
	  
	if (fabs(invMass - Z_MASS) < fabs(dileptonInvMass - Z_MASS)) {
	  dileptonInvMass = invMass;

	  ZLepton1 = Muons[i];
	  ZLepton2 = Muons[j];
	  
	  if (nSelMuon == 3)
	    for (UInt_t k=0; k<nSelMuon; k++)
	      if (k != i && k != j)
		WLepton = Muons[k];
	}
      }
    }
  }

  if (dileptonInvMass < 71) return;

  FillHistograms(theChannel, HasZCandidate);


  // HasWCandidate
  //----------------------------------------------------------------------------
  if      (nSelElec == 2) WLepton = Muons[0];
  else if (nSelMuon == 2) WLepton = Electrons[0];

  if (WLepton.Pt() <= 20) return;

  if (WLepton.DeltaR(ZLepton1) < 0.1) return;
  if (WLepton.DeltaR(ZLepton2) < 0.1) return;

  FillHistograms(theChannel, HasWCandidate);


  // MET
  //----------------------------------------------------------------------------
  if (T_METPFTypeI_ET <= 30) return;

  FillHistograms(theChannel, MET);
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

  outputfile << Form("\n %39s results with %7.1f pb\n", sample.Data(), luminosity);

  outputfile << Form("\n %18s  %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  for (UInt_t i=0; i<nCuts; i++) {
    
    outputfile << Form(" %18s:", sCut[i].Data());

    for (UInt_t j=0; j<nChannels; j++)
      outputfile << Form(" %11.1f", hCounterPU[j][i]->Integral());

    outputfile << "\n";
  }


  // MC only
  //----------------------------------------------------------------------------
  if (!sample.Contains("DoubleElectron") && !sample.Contains("DoubleMu")) {

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
  }

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

  GetInputParameters()->TheNamedDouble("xs_weight",  xs_weight);
  GetInputParameters()->TheNamedDouble("luminosity", luminosity);
  
  if (sample.Contains("WZTo3LNu"))
    {
      xs_weight = (xsWplusZ + xsWminusZ) * WZ23lnu * luminosity / ngenWZphase;
    }
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
      else if (eta > 1.479)               {mvaCut = 0.62;}
    }
  else
    {
      if      (eta <= 0.8)                 {mvaCut = 0.94;}
      else if (eta > 0.8 && eta <= 1.479 ) {mvaCut = 0.85;}
      else if (eta > 1.479 )               {mvaCut = 0.92;}
    }

  return (T_Elec_MVA->at(iElec) > mvaCut);
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
  Double_t max_emIso03   = std::max(emIso03-1.0, 0.0);

  Bool_t pass = false;
	
  if (fabs(T_Elec_SC_Eta->at(iElec)) < 1.479)
    {
      pass = sigmaietaieta < 0.01
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
      pass = sigmaietaieta < 0.03
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
  return (T_Elec_pfComb->at(iElec) < 0.15);
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
  Bool_t pass = (fabs(T_Muon_dzPVBiasedPV->at(iMuon)) <= 0.1);

  Double_t MaxMuIP = (T_Muon_Pt->at(iMuon) < 20) ? 0.01 : 0.02;

  pass &= (fabs(T_Muon_IP2DBiasedPV->at(iMuon)) <= MaxMuIP);

  return pass;
}


//------------------------------------------------------------------------------
// MuonIsolation
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonIsolation(UInt_t iMuon) 
{
  Double_t mueta = T_Muon_Eta->at(iMuon);
  Double_t mupt  = T_Muon_Pt ->at(iMuon);

  Double_t isoCut = false;

  if (mupt <= 20)
    {
      if  (fabs(mueta) < 1.479) isoCut = 0.82;
      else                      isoCut = 0.86;
    }
  else
    {
      if  (fabs(mueta) < 1.479) isoCut = 0.86;
      else                      isoCut = 0.82;
    }

  return (T_Muon_MVARings->at(iMuon) >= isoCut);
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
// FillCounters
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::FillCounters(UInt_t nElec, UInt_t nMuon, UInt_t iCut)
{
  if (nElec < 2 && nMuon < 2 && (nElec + nMuon) < 2) return false;

  if (nElec >= 2) FillChannelCounters(EEE, iCut);

  if (nMuon >= 2) FillChannelCounters(MMM, iCut);

  if (nElec + nMuon >= 2)
    {
      FillChannelCounters(EEM, iCut);
      FillChannelCounters(MME, iCut);
    }

  return true;
}


//------------------------------------------------------------------------------
// FillHistograms
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistograms(UInt_t iChannel, UInt_t iCut)
{
  FillChannelCounters(iChannel, iCut);

  Double_t hweight = efficiency_weight * xs_weight;

  hNPV[iChannel][iCut]->Fill(T_Vertex_z->size(), hweight);
  hMET[iChannel][iCut]->Fill(T_METPFTypeI_ET,    hweight);

  if (iCut < Exactly3Leptons) return;

  Double_t sumCharges   = -999;
  Double_t invMass3Lep  = -999;
  Double_t invMass2Lep1 =  999;
  Double_t invMass2Lep2 =  999;

  std::vector<Double_t> ptLeptons;

  ptLeptons.clear();


  // MMM
  //----------------------------------------------------------------------------
  if (theChannel == MMM)
    {
      sumCharges = Muons_Charge[0] + Muons_Charge[1] + Muons_Charge[2];

      invMass3Lep = (Muons[0] + Muons[1] + Muons[2]).M();

      if (Muons_Charge[0] * Muons_Charge[1] < 0) {

	invMass2Lep1 = (Muons[0] + Muons[1]).M();

	if (Muons_Charge[0] * Muons_Charge[2] < 0) invMass2Lep2 = (Muons[0] + Muons[2]).M();
	if (Muons_Charge[1] * Muons_Charge[2] < 0) invMass2Lep2 = (Muons[1] + Muons[2]).M();
      }
      else if (Muons_Charge[0] * Muons_Charge[2] < 0) {

	invMass2Lep1 = (Muons[0] + Muons[2]).M();

	if (Muons_Charge[1] * Muons_Charge[2] < 0) invMass2Lep2 = (Muons[1] + Muons[2]).M();
      }
      else
	if (Muons_Charge[1] * Muons_Charge[2] < 0) invMass2Lep1 = (Muons[1] + Muons[2]).M();

      ptLeptons.push_back(Muons[0].Pt());
      ptLeptons.push_back(Muons[1].Pt());
      ptLeptons.push_back(Muons[2].Pt());
    }
  // MME
  //----------------------------------------------------------------------------
  else if (theChannel == MME)
    {
      sumCharges = Muons_Charge[0] + Muons_Charge[1] + Electrons_Charge[0];

      invMass3Lep = (Muons[0] + Muons[1] + Electrons[0]).M();

      if (Muons_Charge[0] * Muons_Charge[1] < 0) invMass2Lep1 = (Muons[0] + Muons[1]).M();

      ptLeptons.push_back(Muons[0].Pt());
      ptLeptons.push_back(Muons[1].Pt());
      ptLeptons.push_back(Electrons[0].Pt());
    }
  // EEM
  //----------------------------------------------------------------------------
  else if (theChannel == EEM)
    {
      sumCharges = Muons_Charge[0] + Electrons_Charge[0] + Electrons_Charge[1];

      invMass3Lep = (Muons[0] + Electrons[1] + Electrons[2]).M();

      if (Electrons_Charge[0] * Electrons_Charge[1] < 0) invMass2Lep1 = (Electrons[0] + Electrons[1]).M();

      ptLeptons.push_back(Muons[0].Pt());
      ptLeptons.push_back(Electrons[0].Pt());
      ptLeptons.push_back(Electrons[1].Pt());
    }
  // EEE
  //----------------------------------------------------------------------------
  else if (theChannel == EEE)
    {
      sumCharges = Electrons_Charge[0] + Electrons_Charge[1] + Electrons_Charge[2];

      invMass3Lep = (Electrons[0] + Electrons[1] + Electrons[2]).M();

      if (Electrons_Charge[0] * Electrons_Charge[1] < 0) {

	invMass2Lep1 = (Electrons[0] + Electrons[1]).M();

	if (Electrons_Charge[0] * Electrons_Charge[2] < 0) invMass2Lep2 = (Electrons[0] + Electrons[2]).M();
	if (Electrons_Charge[1] * Electrons_Charge[2] < 0) invMass2Lep2 = (Electrons[1] + Electrons[2]).M();
      }
      else if (Electrons_Charge[0] * Electrons_Charge[2] < 0) {

	invMass2Lep1 = (Electrons[0] + Electrons[2]).M();

	if (Electrons_Charge[1] * Electrons_Charge[2] < 0) invMass2Lep2 = (Electrons[1] + Electrons[2]).M();
      }
      else
	if (Electrons_Charge[1] * Electrons_Charge[2] < 0) invMass2Lep1 = (Electrons[1] + Electrons[2]).M();

      ptLeptons.push_back(Electrons[0].Pt());
      ptLeptons.push_back(Electrons[1].Pt());
      ptLeptons.push_back(Electrons[2].Pt());
    }

  std::sort(ptLeptons.begin(), ptLeptons.end(), std::greater<Double_t>());

  hSumCharges  [iChannel][iCut]->Fill(sumCharges,  hweight);
  hInvMass3Lep [iChannel][iCut]->Fill(invMass3Lep, hweight);

  hPtLepton1   [iChannel][iCut]->Fill(ptLeptons[0], hweight);
  hPtLepton2   [iChannel][iCut]->Fill(ptLeptons[1], hweight);
  hPtLepton3   [iChannel][iCut]->Fill(ptLeptons[2], hweight);

  if (fabs(invMass2Lep1 - Z_MASS) < fabs(invMass2Lep2 - Z_MASS))
    {
      hInvMass2Lep1[iChannel][iCut]->Fill(invMass2Lep1, hweight);
      hInvMass2Lep2[iChannel][iCut]->Fill(invMass2Lep2, hweight);
    }
  else
    {
      hInvMass2Lep1[iChannel][iCut]->Fill(invMass2Lep2, hweight);
      hInvMass2Lep2[iChannel][iCut]->Fill(invMass2Lep1, hweight);
    }

  if (iCut < HasWCandidate) return;

  Double_t invMass = (ZLepton1 + ZLepton2).M();

  Double_t pt1, pt2;

  if (ZLepton1.Pt() > ZLepton2.Pt())
    {
      pt1 = ZLepton1.Pt(); pt2 = ZLepton2.Pt();
    }
  else
    {
      pt1 = ZLepton2.Pt(); pt2 = ZLepton1.Pt();
    }
  
  hPtZLepton1[iChannel][iCut]->Fill(pt1,          hweight);
  hPtZLepton2[iChannel][iCut]->Fill(pt2,          hweight);
  hPtWLepton [iChannel][iCut]->Fill(WLepton.Pt(), hweight);
  hInvMassZ  [iChannel][iCut]->Fill(invMass,      hweight);
}


//------------------------------------------------------------------------------
// isSignalMCInsideZmassRange
//------------------------------------------------------------------------------
const bool AnalysisWZ::isSignalMCInsideZmassRange(const float & masslow,
						  const float & masshigh) const
{
  float masszcand = 0.0;

  const unsigned int ntauSt3  = T_Gen_TauSt3_PID ->size();
  const unsigned int nmuonSt3 = T_Gen_MuonSt3_PID->size();
  const unsigned int nelecSt3 = T_Gen_ElecSt3_PID->size();

  if (ntauSt3 >= 2)
    {
      std::vector<std::pair<int,TLorentzVector> > precand;

      for (unsigned int i=0; i<ntauSt3; ++i)
	{
	  TLorentzVector Tau(T_Gen_TauSt3_Px->at(i),
			     T_Gen_TauSt3_Py->at(i),
			     T_Gen_TauSt3_Pz->at(i),
			     T_Gen_TauSt3_Energy->at(i));
	  
	  precand.push_back(std::pair<int, TLorentzVector>(T_Gen_TauSt3_PID->at(i), Tau));
	}
      

      // Construct the Z candidate
      //------------------------------------------------------------------------
      for (unsigned int j=0; j<precand.size(); ++j)
	{
	  for (unsigned int i=j+1; i<precand.size(); ++i)
	    {
	      if (precand[j].first/precand[i].first == 1) continue;

	      float thismass = (precand[j].second + precand[i].second).M();
	      
	      if (fabs(masszcand - Z_MASS) > (thismass - Z_MASS)) masszcand = thismass;
	    }
	}
      
    }
  else if (nmuonSt3 >= 2)
    {
      std::vector<TLorentzVector> zcand;

      for (unsigned int i=0; i<nmuonSt3; ++i)
	{
	  if (abs(T_Gen_Muon_MPID->at(i)) == 23) // PID Z == 23
	    {
	      zcand.push_back(TLorentzVector(T_Gen_Muon_Px->at(i),
					     T_Gen_Muon_Py->at(i),
					     T_Gen_Muon_Pz->at(i),
					     T_Gen_Muon_Energy->at(i)));
	    }
	}

      if (zcand.size() == 2)
	{
	  TLorentzVector prov(zcand[0] + zcand[1]);

	  masszcand = prov.M();
	}
    }
  else if (nelecSt3 >= 2)
    {
      std::vector<TLorentzVector> zcand;

      for(unsigned int i=0; i<nelecSt3; ++i)
	{
	  if(abs(T_Gen_Elec_MPID->at(i)) == 23)
	    {
	      zcand.push_back(TLorentzVector(T_Gen_Elec_Px->at(i),
					     T_Gen_Elec_Py->at(i),
					     T_Gen_Elec_Pz->at(i),
					     T_Gen_Elec_Energy->at(i)));
	    }
	}
      
      if (zcand.size() == 2)
	{
	  TLorentzVector prov(zcand[0] + zcand[1]);

	  masszcand = prov.M();
	}
    }
  

  // Accepting only generated events inside the range
  //----------------------------------------------------------------------------
  bool ispass = true;

  if (masszcand < masslow || masszcand > masshigh)
    {
      ispass = false;
    }
  
  return ispass;
}
