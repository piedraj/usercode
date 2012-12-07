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


  // Counters
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannels; i++) {
    for (UInt_t j=0; j<nCounters; j++) {

      TString cname = Form("counter%d%d", i, j);

      counter[i][j] = InitCounterUI(cname, cname, 0);
    }
  }


  // Histograms
  //----------------------------------------------------------------------------
  TH1::SetDefaultSumw2();

  for (UInt_t i=0; i<nChannels; i++) {
    for (UInt_t j=0; j<nCuts; j++) {

      TString suffix = "_" + sChannel[i] + "_" + sCut[j];

      hCounterEff[i][j] = CreateH1F(TString("hCounterEff" + suffix), "",   3, 0,   3);
      hCounter   [i][j] = CreateH1F(TString("hCounter"    + suffix), "",   3, 0,   3);
      hNPV       [i][j] = CreateH1F(TString("hNPV"        + suffix), "",  60, 0,  60);
      hMET       [i][j] = CreateH1F(TString("hMET"        + suffix), "", 200, 0, 200);

      if (j == PreSelection) continue;

      hPtZLepton1[i][j] = CreateH1F(TString("hPtZLepton1" + suffix), "", 200, 0, 200);
      hPtZLepton2[i][j] = CreateH1F(TString("hPtZLepton2" + suffix), "", 200, 0, 200);
      hPtWLepton [i][j] = CreateH1F(TString("hPtWLepton"  + suffix), "", 200, 0, 200);    
      hInvMassZ  [i][j] = CreateH1F(TString("hInvMassZ"   + suffix), "", 200, 0, 200);
    }
  }
}


//------------------------------------------------------------------------------
// InsideLoop
//------------------------------------------------------------------------------
void AnalysisWZ::InsideLoop()
{
  UInt_t cutCounter = 0;

  (*counter[MMM][cutCounter])++;
  (*counter[EEE][cutCounter])++;
  (*counter[MME][cutCounter])++;
  (*counter[EEM][cutCounter])++;

  cutCounter++;

  isData = (sample.Contains("DoubleElectron") || sample.Contains("DoubleMu")) ? 1 : 0;

  pu_weight = (isData) ? 1.0 : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

  efficiency_weight = pu_weight;


  // Reset data members
  //----------------------------------------------------------------------------
  Muons_Charge.clear();
  Electrons_Charge.clear();

  Muons.clear();
  Electrons.clear();

  dileptonInvMass =  0;
  nSelMuon        =  0;
  nSelElec        =  0;
  theChannel      = -1;


  // Preselection
  //----------------------------------------------------------------------------
  GetSelectedMuons    (10, 2.4);
  GetSelectedElectrons(10, 2.5);

  if ((nSelMuon + nSelElec) != 3) return;

  if      (nSelMuon == 3) theChannel = MMM;
  else if (nSelElec == 3) theChannel = EEE;
  else if (nSelMuon == 2) theChannel = MME;
  else if (nSelElec == 2) theChannel = EEM;

  (*counter[theChannel][cutCounter++])++;


  // Trigger
  //----------------------------------------------------------------------------
  if (isData) {
    if (nSelMuon >= 2 && !T_passTriggerDoubleMu) return;
    if (nSelElec >= 2 && !T_passTriggerDoubleEl) return;
  }

  (*counter[theChannel][cutCounter++])++;


  // Apply lepton scale factors
  //----------------------------------------------------------------------------
  //  if (!isData) {
  //    if      (theChannel == MMM) efficiency_weight *= (SF_Trigger_MM * SF_Global_MMM);
  //    else if (theChannel == EEE) efficiency_weight *= (SF_Trigger_EE * SF_Global_EEE);
  //    else if (theChannel == MME) efficiency_weight *= (SF_Trigger_MM * SF_Global_MME);
  //    else if (theChannel == EEM) efficiency_weight *= (SF_Trigger_EE * SF_Global_EEM);
  //  }


  Int_t numberOfHighPtLeptons = 0;

  for (UInt_t i=0; i<(nSelMuon + nSelElec); i++) {

    Double_t leptonPt = (i < nSelElec) ? Electrons[i].Pt() : Muons[i-nSelElec].Pt();

    if (leptonPt > 20) numberOfHighPtLeptons++;
  }

  if (numberOfHighPtLeptons < 2) return;

  (*counter[theChannel][cutCounter++])++;

  FillHistogramsAtCut(theChannel, PreSelection);


  // Z candidate
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

  (*counter[theChannel][cutCounter++])++;

  FillHistogramsAtCut(theChannel, ZCandidate);


  // W candidate
  //----------------------------------------------------------------------------
  if      (nSelElec == 2) WLepton = Muons[0];
  else if (nSelMuon == 2) WLepton = Electrons[0];

  if (WLepton.Pt() <= 20) return;

  (*counter[theChannel][cutCounter++])++;


  if (WLepton.DeltaR(ZLepton1) < 0.1) return;
  if (WLepton.DeltaR(ZLepton2) < 0.1) return;

  (*counter[theChannel][cutCounter++])++;


  if (T_METPFTypeI_ET <= 30) return;

  (*counter[theChannel][cutCounter++])++;

  FillHistogramsAtCut(theChannel, WCandidate);
}


//------------------------------------------------------------------------------
// SetDataMembersAtTermination
//------------------------------------------------------------------------------
void AnalysisWZ::SetDataMembersAtTermination()
{
  for (UInt_t i=0; i<nChannels; i++) {
    for (UInt_t j=0; j<nCounters; j++) {
      counter[i][j] = (TCounterUI*)FindOutput(Form("counter%d%d", i, j));
    }
  }
}


//------------------------------------------------------------------------------
// Summary
//------------------------------------------------------------------------------
void AnalysisWZ::Summary()
{
  GetParameters();

  TString cutName[nCounters] =
    {
      "all input events",
      "three tight leptons with pt > 10 GeV",
      "trigger",
      "pt1, pt2 > 20 GeV",
      "71 < mZ < 111 GeV",
      "W lepton pt > 20 GeV",
      "DeltaR(W lepton, Z leptons) > 0.1",
      "MET > 30 GeV"
    };

  ofstream outputfile;

  outputfile.open(Form("%s/%s.txt", directory.Data(), sample.Data()));

  outputfile << Form("\n %57s results with %7.1f pb\n", sample.Data(), luminosity);

  outputfile << Form("\n %36s  %10s %10s %10s %10s\n",
		     " ",
		     sChannel[0].Data(),
		     sChannel[1].Data(),
		     sChannel[2].Data(),
		     sChannel[3].Data());

  for (UInt_t i=0; i<nCounters; i++) {
    
    outputfile << Form(" %36s:", cutName[i].Data());

    for (UInt_t j=0; j<nChannels; j++)
      outputfile << Form(" %10d", counter[j][i]->GetValue());

    outputfile << "\n";
  }

  outputfile << "\n";


  // MC only
  //----------------------------------------------------------------------------
  if (!isData) {

    outputfile << Form(" %71s: %.6f\n", "xs * luminosity / nevents", xs_weight);

    outputfile << Form("\n %36s  %10s %10s %10s %10s\n",
		       " ",
		       sChannel[0].Data(),
		       sChannel[1].Data(),
		       sChannel[2].Data(),
		       sChannel[3].Data());

    for (UInt_t i=0; i<nCounters; i++) {
      
      outputfile << Form(" %36s:", cutName[i].Data());

      for (UInt_t j=0; j<nChannels; j++)
	outputfile << Form(" %10.0f", counter[j][i]->GetValue() * xs_weight);
      
      outputfile << "\n";
    }

    outputfile << "\n";
  }

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
  Double_t isoCut = (fabs(T_Muon_Eta->at(iMuon)) < 1.479) ? 0.82 : 0.86;
		
  return (T_Muon_MVARings->at(iMuon) >= isoCut);
}


//------------------------------------------------------------------------------
// SelectedElectronPt
//------------------------------------------------------------------------------
Double_t AnalysisWZ::SelectedElectronPt(UInt_t iElec)
{
  Bool_t pass = true;

  pass &= ElectronBDT      (iElec);
  pass &= ElectronID       (iElec);
  pass &= ElectronCloseToPV(iElec);
  pass &= ElectronIsolation(iElec);

  Double_t electronPt = (pass) ? T_Elec_Pt->at(iElec) : -999;

  return electronPt;
}


//------------------------------------------------------------------------------
// SelectedMuonPt
//------------------------------------------------------------------------------
Double_t AnalysisWZ::SelectedMuonPt(UInt_t iMuon)
{
  Bool_t pass = true;

  pass &= MuonID       (iMuon);
  pass &= MuonCloseToPV(iMuon);
  pass &= MuonIsolation(iMuon);

  Double_t muonPt = (pass) ? T_Muon_Pt->at(iMuon) : -999;

  return muonPt;
}


//------------------------------------------------------------------------------
// GetSelectedElectrons
//------------------------------------------------------------------------------
void AnalysisWZ::GetSelectedElectrons(Double_t ptMin, Double_t etaMax)
{
  for (UInt_t i=0; i<T_Elec_Px->size(); i++) {

    if (SelectedElectronPt(i) <= ptMin)  continue;

    if (fabs(T_Elec_SC_Eta->at(i)) >= etaMax) continue;
	
    Electrons_Charge.push_back(T_Elec_Charge->at(i));
	
    TLorentzVector Elec(T_Elec_Px    ->at(i),
			T_Elec_Py    ->at(i),
			T_Elec_Pz    ->at(i),
			T_Elec_Energy->at(i));
	
    Electrons.push_back(Elec);
  }

  nSelElec = Electrons.size();
}


//------------------------------------------------------------------------------
// GetSelectedMuons
//------------------------------------------------------------------------------
void AnalysisWZ::GetSelectedMuons(Double_t ptMin, Double_t etaMax)
{
  for (UInt_t i=0; i<T_Muon_Px->size(); i++) {

    if (SelectedMuonPt(i) <= ptMin) continue;

    if (fabs(T_Muon_Eta->at(i)) >= etaMax) continue;

    Muons_Charge.push_back(T_Muon_Charge->at(i));

    TLorentzVector Muon(T_Muon_Px    ->at(i),
			T_Muon_Py    ->at(i),
			T_Muon_Pz    ->at(i),
			T_Muon_Energy->at(i));

    Muons.push_back(Muon);
  }

  nSelMuon = Muons.size();
}


//------------------------------------------------------------------------------
// FillHistogramsAtCut
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistogramsAtCut(UInt_t iChannel, UInt_t iCut)
{
  hCounterEff[iChannel][iCut]->Fill(1,                  efficiency_weight);
  hCounter   [iChannel][iCut]->Fill(1,                  efficiency_weight * xs_weight);
  hNPV       [iChannel][iCut]->Fill(T_Vertex_z->size(), efficiency_weight * xs_weight);
  hMET       [iChannel][iCut]->Fill(T_METPF_ET,         efficiency_weight * xs_weight);

  if (iCut == PreSelection) return;

  Double_t invMass = (ZLepton1 + ZLepton2).M();

  Double_t pt1, pt2;

  if (ZLepton1.Pt() > ZLepton2.Pt())
    {pt1 = ZLepton1.Pt(); pt2 = ZLepton2.Pt();}
  else
    {pt1 = ZLepton2.Pt(); pt2 = ZLepton1.Pt();}
  
  hPtZLepton1[iChannel][iCut]->Fill(pt1,          efficiency_weight * xs_weight);
  hPtZLepton2[iChannel][iCut]->Fill(pt2,          efficiency_weight * xs_weight);
  hPtWLepton [iChannel][iCut]->Fill(WLepton.Pt(), efficiency_weight * xs_weight);
  hInvMassZ  [iChannel][iCut]->Fill(invMass,      efficiency_weight * xs_weight);
}
