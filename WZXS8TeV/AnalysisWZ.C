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
  for (UInt_t i=0; i<nChannels; i++) {
    for (UInt_t j=0; j<nCuts; j++) {

      TString suffix = "_" + sChannel[i] + "_" + sCut[j];

      hCounter   [i][j] = CreateH1F(TString("hCounter"    + suffix), "",   3, 0,   3);
      hMET       [i][j] = CreateH1F(TString("hMET"        + suffix), "", 200, 0, 200);
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
  weight = fWeight;

  if (!sampleName.Contains("Data"))
    weight *= fPUWeight->GetWeight((Int_t)T_Event_nTruePU);


  // Reset data members
  //----------------------------------------------------------------------------
  Muons_Charge.clear();
  Electrons_Charge.clear();

  Muons.clear();
  Electrons.clear();

  leadingLeptonPt =  0;
  dileptonInvMass =  0;
  nSelMuon        =  0;
  nSelElec        =  0;
  theChannel      = -1;


  // Take pythia samples up to mll<50, then complete with madgraph
  //----------------------------------------------------------------------------
  if (!IsGenAccepted()) return;


  // Preselection
  //----------------------------------------------------------------------------
  GetSelectedMuon();
  GetSelectedElec();

  if ((nSelMuon + nSelElec) != 3) return;

  if      (nSelMuon == 3) theChannel = iMMM;
  else if (nSelElec == 3) theChannel = iEEE;
  else if (nSelMuon == 2) theChannel = iMME;
  else if (nSelElec == 2) theChannel = iEEM;

  if ((theChannel == iMMM || theChannel == iMME) && !T_passTriggerDoubleMu) return;
  if ((theChannel == iEEE || theChannel == iEEM) && !T_passTriggerDoubleEl) return;


  for (UInt_t i=0; i<(nSelMuon + nSelElec); i++) {

    if (i < nSelElec) {
      if (Electrons[i].Pt() > leadingLeptonPt)
	leadingLeptonPt = Electrons[i].Pt();
    }
    else {
      if (Muons[i-nSelElec].Pt() > leadingLeptonPt)
	leadingLeptonPt = Muons[i-nSelElec].Pt();
    }
  }

  if (leadingLeptonPt <= 20) return;


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

  FillHistogramsAtCut(theChannel, iZCandidate);


  // W candidate
  //----------------------------------------------------------------------------
  if      (nSelElec == 2) WLepton = Muons[0];
  else if (nSelMuon == 2) WLepton = Electrons[0];

  if (WLepton.Pt() <= 20) return;
  
  Double_t dR1 = WLepton.DeltaR(ZLepton1);
  Double_t dR2 = WLepton.DeltaR(ZLepton2);
      
  if ((nSelMuon == 2 || nSelElec == 3) && (dR1 < 0.1 || dR2 < 0.1)) return;

  if (T_METPF_ET <= 30) return;

  
  FillHistogramsAtCut(theChannel, iWCandidate);
}


//------------------------------------------------------------------------------
// GetParameters
//------------------------------------------------------------------------------
void AnalysisWZ::GetParameters()
{
  sampleName = GetInputParameters()->TheNamedString("sampleName");

  GetInputParameters()->TheNamedDouble("weight",     fWeight);
  GetInputParameters()->TheNamedDouble("luminosity", luminosity);
}


//------------------------------------------------------------------------------
// IsGenAccepted
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::IsGenAccepted()
{
  Bool_t pass = true;

  if (sampleName == "ZJets_Madgraph") {

    UInt_t nMuon = T_Gen_MuonSt3_PID->size();
    UInt_t nElec = T_Gen_ElecSt3_PID->size();

    if (nMuon != 2 && nElec != 2) pass = false;
  }
  
  return pass;
}


//------------------------------------------------------------------------------
// SelectedMuonPt
//------------------------------------------------------------------------------
Double_t AnalysisWZ::SelectedMuonPt(UInt_t iMuon, Int_t  iVertex)
{
  Double_t muonPt = -999;

  if (iVertex < 0) return muonPt;

  Bool_t pass = true;

  pass &= (fabs(T_Muon_vz->at(iMuon) - T_Vertex_z->at(iVertex)) < 1);

  TLorentzVector Muon(T_Muon_Px    ->at(iMuon),
		      T_Muon_Py    ->at(iMuon),
		      T_Muon_Pz    ->at(iMuon),
		      T_Muon_Energy->at(iMuon));

  Double_t relIso = 0.0;

  relIso += T_Muon_chargedHadronIsoR03->at(iMuon);
  relIso += T_Muon_neutralHadronIsoR03->at(iMuon);
  relIso += T_Muon_photonIsoR03->at(iMuon);
  relIso /= Muon.Pt();

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
// SelectedElecPt
//------------------------------------------------------------------------------
Double_t AnalysisWZ::SelectedElecPt(UInt_t iElec, Int_t  iVertex)
{
  Double_t electronPt = -999;

  if (iVertex < 0) return electronPt;

  Bool_t pass = true;

  pass &= (fabs(T_Elec_vz->at(iElec) - T_Vertex_z->at(iVertex)) < 1);
	   
  TLorentzVector Elec(T_Elec_Px    ->at(iElec),
		      T_Elec_Py    ->at(iElec),
		      T_Elec_Pz    ->at(iElec),
		      T_Elec_Energy->at(iElec));
  
  Double_t relIso = 0.0;

  relIso += T_Elec_chargedHadronIso->at(iElec);
  relIso += T_Elec_neutralHadronIso->at(iElec);
  relIso += T_Elec_photonIso->at(iElec);
  relIso /= Elec.Pt(); 

  pass &= (relIso < 0.17);

  pass &= (T_Elec_passConversionVeto->at(iElec));
  pass &= (T_Elec_isEcalDriven      ->at(iElec));

  pass &= (T_Elec_MVA  ->at(iElec) > -0.1);
  pass &= (T_Elec_SC_Et->at(iElec) >   15);

  pass &= (fabs(T_Elec_IPwrtPV->at(iElec)) < 0.04);

  pass &= (fabs(Elec.Eta()) < 2.5);


  // Remove electrons close to GlobalMuons
  //----------------------------------------------------------------------------
  Double_t minDeltaR = 999;	

  TVector3 vElec(T_Elec_Px->at(iElec),
		 T_Elec_Py->at(iElec),
		 T_Elec_Pz->at(iElec)); 
  
  for (UInt_t j=0; j<T_Muon_Energy->size(); j++) {
    
    if ((T_Muon_IsGlobalMuon->at(j) || T_Muon_IsAllTrackerMuons->at(j))) { 
	  
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
// FillHistogramsAtCut
//------------------------------------------------------------------------------
void AnalysisWZ::FillHistogramsAtCut(UInt_t iChannel, UInt_t iCut)
{
  Double_t invMass = (ZLepton1 + ZLepton2).M();
  
  hCounter   [iChannel][iCut]->Fill(1,             weight);
  hMET       [iChannel][iCut]->Fill(T_METPF_ET,    weight);
  hPtZLepton1[iChannel][iCut]->Fill(ZLepton1.Pt(), weight);
  hPtZLepton2[iChannel][iCut]->Fill(ZLepton2.Pt(), weight);
  hPtWLepton [iChannel][iCut]->Fill(WLepton.Pt(),  weight);
  hInvMassZ  [iChannel][iCut]->Fill(invMass,       weight);
}


//------------------------------------------------------------------------------
// GetSelectedMuon
//------------------------------------------------------------------------------
void AnalysisWZ::GetSelectedMuon()
{
  UInt_t muonSize = T_Muon_Energy->size();

  for (UInt_t i=0; i<muonSize; i++) {

    if (SelectedMuonPt(i, 0) > 10) {

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
// GetSelectedElec
//------------------------------------------------------------------------------
void AnalysisWZ::GetSelectedElec()
{
  UInt_t elecSize = T_Elec_Energy->size();
  
  for (UInt_t i=0; i<elecSize; i++) {

    if (SelectedElecPt(i, 0) > 10) {
	
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


void AnalysisWZ::SetDataMembersAtTermination() {}
void AnalysisWZ::Summary() {}
