#include "AnalysisWW.h"


#if !defined(__CINT__)
ClassImp(AnalysisWW);
#endif


AnalysisWW::AnalysisWW(TTree* tree) : CMSAnalysisSelectorMiniTrees(tree) {}


//------------------------------------------------------------------------------
// Initialise
//------------------------------------------------------------------------------
void AnalysisWW::Initialise()
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
      hPtLepton1 [i][j] = CreateH1F(TString("hPtLepton1"  + suffix), "", 200, 0, 200);
      hPtLepton2 [i][j] = CreateH1F(TString("hPtLepton2"  + suffix), "", 200, 0, 200);
      hMll       [i][j] = CreateH1F(TString("hMll"        + suffix), "", 200, 0, 200);
    }
  }
}


//------------------------------------------------------------------------------
// InsideLoop
//------------------------------------------------------------------------------
void AnalysisWW::InsideLoop()
{
  isData = (sample.Contains("DoubleElectron") ||
	    sample.Contains("DoubleMu") ||
	    sample.Contains("MuEG")) ? 1 : 0;

  pu_weight = (isData) ? 1.0 : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

  efficiency_weight = pu_weight;


  // Reset data members
  //----------------------------------------------------------------------------
  Muons.clear();
  Electrons.clear();

  Muons_Charge.clear();
  Electrons_Charge.clear();

  nmuon   =  0;
  nelec   =  0;
  channel = -1;


  // Preselection
  //----------------------------------------------------------------------------
  GetSelectedMuons    (10);
  GetSelectedElectrons(10);

  if ((nmuon + nelec) < 2) return;

  Int_t numberOfHighPtLeptons = 0;

  for (UInt_t i=0; i<(nmuon + nelec); i++) {

    Double_t leptonPt = (i < nelec) ? Electrons[i].Pt() : Muons[i-nelec].Pt();

    if (leptonPt > 20) numberOfHighPtLeptons++;
  }

  if (numberOfHighPtLeptons != 2) return;


  // Move from Electrons and Muons to Lepton1 and Lepton2
  //----------------------------------------------------------------------------
  UInt_t firstLepton  = -1;
  UInt_t secondLepton = -1;

  if (nelec == 2) {

    channel = EE;

    if (Electrons[0].Pt() > Electrons[1].Pt()) {
      firstLepton  = 0;
      secondLepton = 1;
    }
    else {
      firstLepton  = 1;
      secondLepton = 0;
    }

    Lepton1 = Electrons[firstLepton];
    Lepton2 = Electrons[secondLepton];
    
    Lepton1_Charge = Electrons_Charge[firstLepton];
    Lepton2_Charge = Electrons_Charge[secondLepton];
  }
  else if (nmuon == 2) {

    channel = MuMu;

    if (Muons[0].Pt() > Muons[1].Pt()) {
      firstLepton  = 0;
      secondLepton = 1;
    }
    else {
      firstLepton  = 1;
      secondLepton = 0;
    }

    Lepton1 = Muons[firstLepton];
    Lepton2 = Muons[secondLepton];

    Lepton1_Charge = Muons_Charge[firstLepton];
    Lepton2_Charge = Muons_Charge[secondLepton];
  }
  else {

    if (Electrons[0].Pt() > Muons[0].Pt()) {

      channel = EMu;

      Lepton1 = Electrons[0];
      Lepton2 = Muons[0];

      Lepton1_Charge = Electrons_Charge[0];
      Lepton2_Charge = Muons_Charge[0];
    }
    else {

      channel = MuE;

      Lepton1 = Muons[0];
      Lepton2 = Electrons[0];

      Lepton1_Charge = Muons_Charge[0];
      Lepton2_Charge = Electrons_Charge[0];
    }
  }

  if (Lepton1_Charge * Lepton2_Charge > 0) return;


  // Trigger
  //----------------------------------------------------------------------------
  if      (channel == MuMu && !T_passTriggerDoubleMu) return;
  else if (channel == EE   && !T_passTriggerDoubleEl) return;
  else if (channel == EMu  && !T_passTriggerElMu)     return;
  else if (channel == MuE  && !T_passTriggerElMu)     return;


  // Apply lepton scale factors
  //----------------------------------------------------------------------------
  if (!isData) {
    if      (channel == MuMu) efficiency_weight *= SFmumu;
    else if (channel == EE)   efficiency_weight *= SFee;
    else if (channel == EMu)  efficiency_weight *= SFmue;
    else if (channel == MuE)  efficiency_weight *= SFmue;
  }

  FillHistogramsAtCut(channel, TwoLeptons);


  // Analysis
  //----------------------------------------------------------------------------
  UInt_t njet      = NumberOfJets   (30, 4.7, -999);
  UInt_t nbjettche = NumberOfJets   (10, 4.7,  2.1);
  Bool_t dphiv     = PassesDPhillJet(15, 4.7);

  Double_t metvar = T_METPF_ET;
  Double_t mll    = (Lepton1 + Lepton2).M();
  Double_t ptll   = (Lepton1 + Lepton2).Pt();

  Bool_t sameflav = (channel == MuMu || channel == EE);
  Bool_t zveto    = (fabs(mll - Z_MASS) > 15 || !sameflav);

  if (mll > 12) {

    if (zveto) {

      if (metvar > (20 + 25*sameflav)) {

	if (njet == 0) {

	  if (dphiv || !sameflav) {

	    if (ptll > 45) {

	      if (nbjettche == 0) {

		FillHistogramsAtCut(channel, WWLevel);
	      }
	    }
	  }
	}
      }
    }
  }
}


//------------------------------------------------------------------------------
// GetParameters
//------------------------------------------------------------------------------
void AnalysisWW::GetParameters()
{
  sample = GetInputParameters()->TheNamedString("sample");

  GetInputParameters()->TheNamedDouble("xs_weight",  xs_weight);
  GetInputParameters()->TheNamedDouble("luminosity", luminosity);
}


//------------------------------------------------------------------------------
// SelectedMuonPt
//------------------------------------------------------------------------------
Double_t AnalysisWW::SelectedMuonPt(UInt_t iMuon)
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
Double_t AnalysisWW::SelectedElectronPt(UInt_t iElec)
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
void AnalysisWW::GetSelectedMuons(Double_t ptMin)
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

  nmuon = Muons.size();
}


//------------------------------------------------------------------------------
// GetSelectedElectrons
//------------------------------------------------------------------------------
void AnalysisWW::GetSelectedElectrons(Double_t ptMin)
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

  nelec = Electrons.size();
}


//------------------------------------------------------------------------------
// NumberOfJets
//------------------------------------------------------------------------------
UInt_t AnalysisWW::NumberOfJets(Double_t ptMin,
				Double_t etaMax,
				Double_t tagCut)
{
  UInt_t jetCounter = 0;

  UInt_t jetSize = T_JetAKCHS_Px->size();

  for (UInt_t i=0; i<jetSize; i++) {

    TVector3 vJet(T_JetAKCHS_Px->at(i),
		  T_JetAKCHS_Py->at(i),
		  T_JetAKCHS_Pz->at(i));

    if (vJet.Pt()        <= ptMin)  continue;
    if (fabs(vJet.Eta()) >= etaMax) continue;
    
    TVector3 vLepton1(Lepton1.Px(), Lepton1.Py(), Lepton1.Pz());
    TVector3 vLepton2(Lepton2.Px(), Lepton2.Py(), Lepton2.Pz());

    if (vJet.DeltaR(vLepton1) < 0.4) continue;
    if (vJet.DeltaR(vLepton2) < 0.4) continue;

    if (T_JetAKCHS_Tag_HighEffTC->at(i) < tagCut) continue;

    jetCounter++;
  }

  return jetCounter;
}


//------------------------------------------------------------------------------
// PassesDPhillJet
//------------------------------------------------------------------------------
Bool_t AnalysisWW::PassesDPhillJet(Double_t ptMin, Double_t etaMax)
{
  Double_t dphi  = 0;
  Double_t ptMax = 0;

  UInt_t jetSize = T_JetAKCHS_Px->size();
  
  for (UInt_t i=0; i<jetSize; i++) {

    TVector3 vJet(T_JetAKCHS_Px->at(i),
		  T_JetAKCHS_Py->at(i),
		  T_JetAKCHS_Pz->at(i));

    if (fabs(vJet.Eta()) >= etaMax) continue;

    TVector3 vLepton1(Lepton1.Px(), Lepton1.Py(), Lepton1.Pz());
    TVector3 vLepton2(Lepton2.Px(), Lepton2.Py(), Lepton2.Pz());
    
    if (vJet.DeltaR(vLepton1) < 0.4) continue;
    if (vJet.DeltaR(vLepton2) < 0.4) continue;

    if (vJet.Pt() > ptMax) {

      ptMax = vJet.Pt();

      dphi = fabs(vJet.DeltaPhi(vLepton1+vLepton2));
    }
  }

  return (ptMax <= ptMin || dphi / TMath::Pi() * 180. < 165.);
}


//------------------------------------------------------------------------------
// FillHistogramsAtCut
//------------------------------------------------------------------------------
void AnalysisWW::FillHistogramsAtCut(UInt_t iChannel, UInt_t iCut)
{
  hCounterEff[iChannel][iCut]->Fill(1,                       efficiency_weight);
  hCounter   [iChannel][iCut]->Fill(1,                       efficiency_weight * xs_weight);
  hNPV       [iChannel][iCut]->Fill(T_Vertex_z->size(),      efficiency_weight * xs_weight);
  hMET       [iChannel][iCut]->Fill(T_METPF_ET,              efficiency_weight * xs_weight);
  hPtLepton1 [iChannel][iCut]->Fill(Lepton1.Pt(),            efficiency_weight * xs_weight);
  hPtLepton2 [iChannel][iCut]->Fill(Lepton2.Pt(),            efficiency_weight * xs_weight);
  hMll       [iChannel][iCut]->Fill((Lepton1 + Lepton2).M(), efficiency_weight * xs_weight);
}


void AnalysisWW::SetDataMembersAtTermination() {}
void AnalysisWW::Summary() {}
