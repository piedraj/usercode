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
  pu_weight = (sample.Contains("Data")) ? 1.0 : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

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
  GetSelectedMuons    (10);
  GetSelectedElectrons(10);

  if ((nSelMuon + nSelElec) != 3) return;

  if      (nSelMuon == 3) theChannel = MMM;
  else if (nSelElec == 3) theChannel = EEE;
  else if (nSelMuon == 2) theChannel = MME;
  else if (nSelElec == 2) theChannel = EEM;


  // Trigger
  //----------------------------------------------------------------------------
  if (nSelMuon >= 2 && !T_passTriggerDoubleMu) return;
  if (nSelElec >= 2 && !T_passTriggerDoubleEl) return;


  // Apply lepton scale factors
  //----------------------------------------------------------------------------
  if (!sample.Contains("Data")) {
    if (nSelMuon >= 2) efficiency_weight *= SFmumu;
    if (nSelElec >= 2) efficiency_weight *= SFee;
  }


  Int_t numberOfHighPtLeptons = 0;

  for (UInt_t i=0; i<(nSelMuon + nSelElec); i++) {

    Double_t leptonPt = (i < nSelElec) ? Electrons[i].Pt() : Muons[i-nSelElec].Pt();

    if (leptonPt > 20) numberOfHighPtLeptons++;
  }

  if (numberOfHighPtLeptons < 2) return;

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

  FillHistogramsAtCut(theChannel, ZCandidate);


  // W candidate
  //----------------------------------------------------------------------------
  if      (nSelElec == 2) WLepton = Muons[0];
  else if (nSelMuon == 2) WLepton = Electrons[0];

  if (WLepton.Pt() <= 20) return;
  
  Double_t dR1 = WLepton.DeltaR(ZLepton1);
  Double_t dR2 = WLepton.DeltaR(ZLepton2);
      
  if ((nSelElec == 1 || nSelElec == 3) && (dR1 < 0.1 || dR2 < 0.1)) return;


  if (T_METPF_ET <= 30) return;

  FillHistogramsAtCut(theChannel, WCandidate);
}


//------------------------------------------------------------------------------
// GetParameters
//------------------------------------------------------------------------------
void AnalysisWZ::GetParameters()
{
  folder = GetInputParameters()->TheNamedString("folder");
  sample = GetInputParameters()->TheNamedString("sample");

  GetInputParameters()->TheNamedDouble("xs_weight",  xs_weight);
  GetInputParameters()->TheNamedDouble("luminosity", luminosity);
}


//------------------------------------------------------------------------------
// SelectedMuonPt
//------------------------------------------------------------------------------
Double_t AnalysisWZ::SelectedMuonPt(UInt_t iMuon)
{
  Double_t muonPt = -999;

  Bool_t pass = (fabs(T_Muon_vz->at(iMuon) - T_Vertex_z->at(0)) < 0.5);

  TLorentzVector Muon(T_Muon_Px    ->at(iMuon),
		      T_Muon_Py    ->at(iMuon),
		      T_Muon_Pz    ->at(iMuon),
		      T_Muon_Energy->at(iMuon));

  Double_t neutralIso = 0.0;

  neutralIso += T_Muon_neutralHadronIsoR04->at(iMuon);
  neutralIso += T_Muon_photonIsoR04->at(iMuon);
  neutralIso -= (0.5 * T_Muon_sumPUPtR04->at(iMuon));

  Double_t relIso = (T_Muon_chargedHadronIsoR04->at(iMuon) + neutralIso) / Muon.Pt();

  pass &= (relIso < 0.2);

  pass &= (T_Muon_IsAllTrackerMuons->at(iMuon));
  pass &= (T_Muon_IsGMPTMuons      ->at(iMuon)); 
  pass &= (T_Muon_IsGlobalMuon     ->at(iMuon));

  pass &= (T_Muon_NormChi2GTrk   ->at(iMuon) < 10);
  pass &= (T_Muon_NValidHitsInTrk->at(iMuon) > 10);
  pass &= (T_Muon_NValidHitsSATrk->at(iMuon) >  0);

  pass &= (fabs(T_Muon_IPAbsGTrack->at(iMuon)) < 0.02);

  pass &= (fabs(Muon.Eta()) < 2.4);

  if (pass) muonPt = Muon.Pt();

  return muonPt;
}


//------------------------------------------------------------------------------
// SelectedElectronPt
//------------------------------------------------------------------------------
Double_t AnalysisWZ::SelectedElectronPt(UInt_t iElec)
{
  Double_t electronPt = -999;

  Bool_t pass = (fabs(T_Elec_vz->at(iElec) - T_Vertex_z->at(0)) < 1);
	   
  TLorentzVector Elec(T_Elec_Px    ->at(iElec),
		      T_Elec_Py    ->at(iElec),
		      T_Elec_Pz    ->at(iElec),
		      T_Elec_Energy->at(iElec));

  Double_t neutralIso = 0.0;

  neutralIso += T_Elec_neutralHadronIso->at(iElec);
  neutralIso += T_Elec_photonIso->at(iElec);
  neutralIso -= (0.5 * T_Elec_puChargedHadronIso->at(iElec));
  neutralIso  = max(0.0, neutralIso);

  Double_t relIso = (T_Elec_chargedHadronIso->at(iElec) + neutralIso) / Elec.Pt();

  pass &= (relIso < 0.15);

  pass &= (T_Elec_passConversionVeto->at(iElec));

  pass &= (T_Elec_MVA->at(iElec) > -0.1);

  pass &= (fabs(T_Elec_IPwrtPV->at(iElec)) < 0.04);

  pass &= (fabs(Elec.Eta()) < 2.5);


  // Remove electrons close to GlobalMuons
  //----------------------------------------------------------------------------
  Double_t minDeltaR = 999;	

  TVector3 vElec(T_Elec_Px->at(iElec),
		 T_Elec_Py->at(iElec),
		 T_Elec_Pz->at(iElec)); 
  
  for (UInt_t j=0; j<T_Muon_Px->size(); j++) {
    
    if (T_Muon_IsGlobalMuon->at(j)) { 
	  
      TVector3 vMuon(T_Muon_Px->at(j),
		     T_Muon_Py->at(j),
		     T_Muon_Pz->at(j));
	  
      Double_t deltaR = vElec.DeltaR(vMuon);

      if (deltaR < minDeltaR) minDeltaR = deltaR;
    }
  }  
      
  pass &= (minDeltaR > 0.1);
	
  if (pass) electronPt = Elec.Pt();

  return electronPt;
}


//------------------------------------------------------------------------------
// GetSelectedMuons
//------------------------------------------------------------------------------
void AnalysisWZ::GetSelectedMuons(Double_t ptMin)
{
  UInt_t muonSize = T_Muon_Px->size();

  for (UInt_t i=0; i<muonSize; i++) {

    if (SelectedMuonPt(i) > ptMin) {

      Muons_Charge.push_back(T_Muon_Charge->at(i));

      TLorentzVector Muon(T_Muon_Px    ->at(i),
			  T_Muon_Py    ->at(i),
			  T_Muon_Pz    ->at(i),
			  T_Muon_Energy->at(i));

      Muons.push_back(Muon);
    }
  }

  nSelMuon = Muons.size();
}


//------------------------------------------------------------------------------
// GetSelectedElectrons
//------------------------------------------------------------------------------
void AnalysisWZ::GetSelectedElectrons(Double_t ptMin)
{
  UInt_t elecSize = T_Elec_Px->size();
  
  for (UInt_t i=0; i<elecSize; i++) {

    if (SelectedElectronPt(i) > ptMin) {
	
      Electrons_Charge.push_back(T_Elec_Charge->at(i));
	
      TLorentzVector Elec(T_Elec_Px    ->at(i),
			  T_Elec_Py    ->at(i),
			  T_Elec_Pz    ->at(i),
			  T_Elec_Energy->at(i));
	
      Electrons.push_back(Elec);
    }
  }

  nSelElec = Electrons.size();
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


void AnalysisWZ::SetDataMembersAtTermination() {}
void AnalysisWZ::Summary() {}
