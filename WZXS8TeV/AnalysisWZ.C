#include "AnalysisWZ.h"


#if !defined(__CINT__)
ClassImp(AnalysisWZ);
#endif


AnalysisWZ::AnalysisWZ(TTree* tree) : CMSAnalysisSelectorMiniTrees(tree)
{
  h_n_tau_St3     = 0;
  h_n_muon_St3    = 0;
  h_n_muon_Gen    = 0;
  h_n_muon_Rec    = 0;
  h_n_muon_Global = 0;
  h_n_muon_PV     = 0;
  h_n_muon_Iso    = 0;
  h_n_muon_ID     = 0;
  h_n_elec_St3    = 0;
  h_n_elec_Gen    = 0;
  h_n_elec_Rec    = 0;
  h_n_elec_Global = 0;
  h_n_elec_PV     = 0;
  h_n_elec_Iso    = 0;
  h_n_elec_ID     = 0;

  h_pt_tau_St3     = 0;
  h_pt_muon_St3    = 0;
  h_pt_muon_Gen    = 0;
  h_pt_muon_Rec    = 0;
  h_pt_muon_Global = 0;
  h_pt_muon_PV     = 0;
  h_pt_muon_Iso    = 0;
  h_pt_muon_ID     = 0;
  h_pt_elec_St3    = 0;
  h_pt_elec_Gen    = 0;
  h_pt_elec_Rec    = 0;
  h_pt_elec_Global = 0;
  h_pt_elec_PV     = 0;
  h_pt_elec_Iso    = 0;
  h_pt_elec_ID     = 0;

  h_eta_tau_St3     = 0;
  h_eta_muon_St3    = 0;
  h_eta_muon_Gen    = 0;
  h_eta_muon_Rec    = 0;
  h_eta_muon_Global = 0;
  h_eta_muon_PV     = 0;
  h_eta_muon_Iso    = 0;
  h_eta_muon_ID     = 0;
  h_eta_elec_St3    = 0;
  h_eta_elec_Gen    = 0;
  h_eta_elec_Rec    = 0;
  h_eta_elec_Global = 0;
  h_eta_elec_PV     = 0;
  h_eta_elec_Iso    = 0;
  h_eta_elec_ID     = 0;

  h_mvaRings_muon_r1 = 0;
  h_mvaRings_muon_r2 = 0;
  h_mvaRings_muon_r3 = 0;
  h_mvaRings_muon_r4 = 0;
}


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

      if (j < AtLeast3Leptons) continue;

      hNPV        [i][j] = CreateH1D(TString("hNPV"         + suffix), "",  50,  0,  50);
      hMET        [i][j] = CreateH1D(TString("hMET"         + suffix), "", 200,  0, 200);
      hSumCharges [i][j] = CreateH1D(TString("hSumCharges"  + suffix), "",   9, -4,   5);
      hInvMass2Lep[i][j] = CreateH1D(TString("hInvMass2Lep" + suffix), "", 200,  0, 200);
      hInvMass3Lep[i][j] = CreateH1D(TString("hInvMass3Lep" + suffix), "", 400,  0, 400);
      hPtLepton1  [i][j] = CreateH1D(TString("hPtLepton1"   + suffix), "", 200,  0, 200);
      hPtLepton2  [i][j] = CreateH1D(TString("hPtLepton2"   + suffix), "", 200,  0, 200);
      hPtLepton3  [i][j] = CreateH1D(TString("hPtLepton3"   + suffix), "", 200,  0, 200);    
      hPtZLepton1 [i][j] = CreateH1D(TString("hPtZLepton1"  + suffix), "", 200,  0, 200);
      hPtZLepton2 [i][j] = CreateH1D(TString("hPtZLepton2"  + suffix), "", 200,  0, 200);
      hPtWLepton  [i][j] = CreateH1D(TString("hPtWLepton"   + suffix), "", 200,  0, 200);    
    }
  }


  // Gen study
  //----------------------------------------------------------------------------
  h_n_tau_St3     = CreateH1D("h_n_tau_St3",     "", 10, 0, 10);
  h_n_muon_St3    = CreateH1D("h_n_muon_St3",    "", 10, 0, 10);
  h_n_muon_Gen    = CreateH1D("h_n_muon_Gen",    "", 10, 0, 10);
  h_n_muon_Rec    = CreateH1D("h_n_muon_Rec",    "", 10, 0, 10);
  h_n_muon_Global = CreateH1D("h_n_muon_Global", "", 10, 0, 10);
  h_n_muon_PV     = CreateH1D("h_n_muon_PV",     "", 10, 0, 10);
  h_n_muon_Iso    = CreateH1D("h_n_muon_Iso",    "", 10, 0, 10);
  h_n_muon_ID     = CreateH1D("h_n_muon_ID",     "", 10, 0, 10);
  h_n_elec_St3    = CreateH1D("h_n_elec_St3",    "", 10, 0, 10);
  h_n_elec_Gen    = CreateH1D("h_n_elec_Gen",    "", 10, 0, 10);
  h_n_elec_Rec    = CreateH1D("h_n_elec_Rec",    "", 10, 0, 10);
  h_n_elec_Global = CreateH1D("h_n_elec_Global", "", 10, 0, 10);
  h_n_elec_PV     = CreateH1D("h_n_elec_PV",     "", 10, 0, 10);
  h_n_elec_Iso    = CreateH1D("h_n_elec_Iso",    "", 10, 0, 10);
  h_n_elec_ID     = CreateH1D("h_n_elec_ID",     "", 10, 0, 10);

  h_pt_tau_St3     = CreateH1D("h_pt_tau_St3",     "", 200, 0, 200);
  h_pt_muon_St3    = CreateH1D("h_pt_muon_St3",    "", 200, 0, 200);
  h_pt_muon_Gen    = CreateH1D("h_pt_muon_Gen",    "", 200, 0, 200);
  h_pt_muon_Rec    = CreateH1D("h_pt_muon_Rec",    "", 200, 0, 200);
  h_pt_muon_Global = CreateH1D("h_pt_muon_Global", "", 200, 0, 200);
  h_pt_muon_PV     = CreateH1D("h_pt_muon_PV",     "", 200, 0, 200);
  h_pt_muon_Iso    = CreateH1D("h_pt_muon_Iso",    "", 200, 0, 200);
  h_pt_muon_ID     = CreateH1D("h_pt_muon_ID",     "", 200, 0, 200);
  h_pt_elec_St3    = CreateH1D("h_pt_elec_St3",    "", 200, 0, 200);
  h_pt_elec_Gen    = CreateH1D("h_pt_elec_Gen",    "", 200, 0, 200);
  h_pt_elec_Rec    = CreateH1D("h_pt_elec_Rec",    "", 200, 0, 200);
  h_pt_elec_Global = CreateH1D("h_pt_elec_Global", "", 200, 0, 200);
  h_pt_elec_PV     = CreateH1D("h_pt_elec_PV",     "", 200, 0, 200);
  h_pt_elec_Iso    = CreateH1D("h_pt_elec_Iso",    "", 200, 0, 200);
  h_pt_elec_ID     = CreateH1D("h_pt_elec_ID",     "", 200, 0, 200);

  h_eta_tau_St3     = CreateH1D("h_eta_tau_St3",     "", 180, -3, 3);
  h_eta_muon_St3    = CreateH1D("h_eta_muon_St3",    "", 180, -3, 3);
  h_eta_muon_Gen    = CreateH1D("h_eta_muon_Gen",    "", 180, -3, 3);
  h_eta_muon_Rec    = CreateH1D("h_eta_muon_Rec",    "", 180, -3, 3);
  h_eta_muon_Global = CreateH1D("h_eta_muon_Global", "", 180, -3, 3);
  h_eta_muon_PV     = CreateH1D("h_eta_muon_PV",     "", 180, -3, 3);
  h_eta_muon_Iso    = CreateH1D("h_eta_muon_Iso",    "", 180, -3, 3);
  h_eta_muon_ID     = CreateH1D("h_eta_muon_ID",     "", 180, -3, 3);
  h_eta_elec_St3    = CreateH1D("h_eta_elec_St3",    "", 180, -3, 3);
  h_eta_elec_Gen    = CreateH1D("h_eta_elec_Gen",    "", 180, -3, 3);
  h_eta_elec_Rec    = CreateH1D("h_eta_elec_Rec",    "", 180, -3, 3);
  h_eta_elec_Global = CreateH1D("h_eta_elec_Global", "", 180, -3, 3);
  h_eta_elec_PV     = CreateH1D("h_eta_elec_PV",     "", 180, -3, 3);
  h_eta_elec_Iso    = CreateH1D("h_eta_elec_Iso",    "", 180, -3, 3);
  h_eta_elec_ID     = CreateH1D("h_eta_elec_ID",     "", 180, -3, 3);

  h_mvaRings_muon_r1 = CreateH1D("h_mvaRings_muon_r1", "", 110, -1.1, 1.1);
  h_mvaRings_muon_r2 = CreateH1D("h_mvaRings_muon_r2", "", 110, -1.1, 1.1);
  h_mvaRings_muon_r3 = CreateH1D("h_mvaRings_muon_r3", "", 110, -1.1, 1.1);
  h_mvaRings_muon_r4 = CreateH1D("h_mvaRings_muon_r4", "", 110, -1.1, 1.1);
}


//------------------------------------------------------------------------------
// InsideLoop
//------------------------------------------------------------------------------
void AnalysisWZ::InsideLoop()
{
  isData = (sample.Contains("DoubleElectron") || sample.Contains("DoubleMu")) ? 1 : 0;

  pu_weight = (isData) ? 1.0 : fPUWeight->GetWeight((Int_t)T_Event_nTruePU);

  efficiency_weight = pu_weight;

  AnalysisLeptons.clear();

  invMass2Lep =  0;
  invMass3Lep =  0;
  sumCharges  =  0;
  theChannel  = -1;


  // Gen study
  //----------------------------------------------------------------------------
  GenStudy();


  // MC filters
  //----------------------------------------------------------------------------
  if (sample.Contains("WGstar") && WgammaFilter()) return;

  if (sample.Contains("WZTo3LNu") && !isSignalMCInsideZmassRange(71,111)) return;

  if (sample.Contains("ZJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;

  if (sample.Contains("WJets_Madgraph") && T_Gen_bSt3_Px->size() > 0) return;


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

    TLorentzVector MuonVector(T_Muon_Px->at(i),
			      T_Muon_Py->at(i),
			      T_Muon_Pz->at(i),
			      T_Muon_Energy->at(i));

    if (MuonVector.Pt() <= 10) continue;

    if (fabs(MuonVector.Eta()) >= 2.4) continue;

    Bool_t isnotglobal = ( T_Muon_IsAllStandAloneMuons->at(i)
			   && !T_Muon_IsGlobalMuon->at(i)
			   && !T_Muon_IsAllTrackerMuons->at(i) );

    if (isnotglobal) continue;

    countMuons++;


    // Tight muons
    //--------------------------------------------------------------------------
    if (MuonCloseToPV(i, false)) {

      countPVMuons++;

      if (MuonIsolation(i, false)) {

	countIsoMuons++;

	if (MuonID(i)) {

	  Lepton AnalysisMuon;

	  AnalysisMuon.index  = i;
	  AnalysisMuon.flavor = Muon;
	  AnalysisMuon.type   = Tight;
	  AnalysisMuon.charge = T_Muon_Charge->at(i);
	  AnalysisMuon.v      = MuonVector;

	  AnalysisLeptons.push_back(AnalysisMuon);
	}
      }
    }


    // Loose muons
    //--------------------------------------------------------------------------
    if (!useFakes) continue;

    if (MuonCloseToPV(i, true)) {

      if (MuonIsolation(i, true)) {

	if (MuonID(i)) {
	  
	  Lepton AnalysisMuon;
	  
	  AnalysisMuon.index  = i;
	  AnalysisMuon.flavor = Muon;
	  AnalysisMuon.type   = Loose;
	  AnalysisMuon.charge = T_Muon_Charge->at(i);
	  AnalysisMuon.v      = MuonVector;

	  AnalysisLeptons.push_back(AnalysisMuon);
	}
      }
    }
  }


  // Loop over electrons
  //----------------------------------------------------------------------------
  UInt_t countElectrons    = 0;
  UInt_t countPVElectrons  = 0;
  UInt_t countIsoElectrons = 0;

  for (UInt_t i=0; i<T_Elec_Px->size(); i++) {

    TLorentzVector ElectronVector(T_Elec_Px->at(i),
				  T_Elec_Py->at(i),
				  T_Elec_Pz->at(i),
				  T_Elec_Energy->at(i));

    if (ElectronVector.Pt() <= 10) continue;

    if (fabs(ElectronVector.Eta()) >= 2.5) continue;

    if (!ElectronID(i)) continue;

    countElectrons++;

    if (!ElectronCloseToPV(i)) continue;

    countPVElectrons++;


    // Tight electrons
    //--------------------------------------------------------------------------
    if (ElectronIsolation(i, false)) {

      countIsoElectrons++;

      if (ElectronBDT(i, false)) {
	
	Lepton AnalysisElectron;
	
	AnalysisElectron.index  = i;
	AnalysisElectron.flavor = Electron;
	AnalysisElectron.type   = Tight;
	AnalysisElectron.charge = T_Elec_Charge->at(i);
	AnalysisElectron.v      = ElectronVector;
	
	AnalysisLeptons.push_back(AnalysisElectron);
      }
    }


    // Loose electrons
    //--------------------------------------------------------------------------
    if (!useFakes) continue;

    if (ElectronIsolation(i, true)) {

      if (ElectronBDT(i, true)) {
	
	Lepton AnalysisElectron;
	
	AnalysisElectron.index  = i;
	AnalysisElectron.flavor = Electron;
	AnalysisElectron.type   = Loose;
	AnalysisElectron.charge = T_Elec_Charge->at(i);
	AnalysisElectron.v      = ElectronVector;
	
	AnalysisLeptons.push_back(AnalysisElectron);
      }
    }
  }


  // Require at least 2 tight leptons
  //----------------------------------------------------------------------------
  if (!FillCounters(countElectrons,    countMuons,    Has2Leptons))    return;
  if (!FillCounters(countPVElectrons,  countPVMuons,  Has2PVLeptons))  return;
  if (!FillCounters(countIsoElectrons, countIsoMuons, Has2IsoLeptons)) return;


  // Sort the leptons by pt
  //----------------------------------------------------------------------------
  if (AnalysisLeptons.size() < 3) return;

  std::sort(AnalysisLeptons.begin(), AnalysisLeptons.end());

  std::reverse(AnalysisLeptons.begin(), AnalysisLeptons.end());


  // Require exactly 3 leptons with pt > 20 GeV
  //----------------------------------------------------------------------------
  UInt_t numberOfHighPtLeptons = 0;

  for (std::vector<Lepton>::iterator lep_iterator=AnalysisLeptons.begin();
       lep_iterator!=AnalysisLeptons.end(); lep_iterator++) {
      
    if ((*lep_iterator).v.Pt() > 20) numberOfHighPtLeptons++;
  }

  if (numberOfHighPtLeptons != 3) return;


  // Classify the channels
  //----------------------------------------------------------------------------
  UInt_t electronCounter = 0;
  UInt_t leptonCounter   = 0;

  for (std::vector<Lepton>::iterator lep_iterator=AnalysisLeptons.begin();
       lep_iterator!=AnalysisLeptons.end(); lep_iterator++) {
  
    if (++leptonCounter > 3) break;

    if ((*lep_iterator).flavor == Electron) electronCounter++;
  }

  if      (electronCounter == 0) theChannel = MMM;
  else if (electronCounter == 1) theChannel = MME;
  else if (electronCounter == 2) theChannel = EEM;
  else if (electronCounter == 3) theChannel = EEE;

  if (sample.Contains("DoubleMu")       && electronCounter > 1) return;
  if (sample.Contains("DoubleElectron") && electronCounter < 2) return;


  // Make Z and W candidates
  //----------------------------------------------------------------------------
  for (std::vector<Lepton>::iterator lep1_iterator=AnalysisLeptons.begin();
       lep1_iterator!=AnalysisLeptons.end(); lep1_iterator++) {

    Lepton lep1 = (*lep1_iterator);

    if (lep1.v.Pt() <= 20) break;

    sumCharges += lep1.charge;

    for (std::vector<Lepton>::iterator lep2_iterator=AnalysisLeptons.begin();
	 lep2_iterator!=AnalysisLeptons.end(); lep2_iterator++) {
      
      if (lep2_iterator == lep1_iterator) continue;

      Lepton lep2 = (*lep2_iterator);

      if (lep2.v.Pt() <= 20) break;

      if (lep1.flavor != lep2.flavor) continue;

      if (lep1.charge * lep2.charge > 0) continue;

      Double_t inv_mass = (lep1.v + lep2.v).M();

      if (fabs(inv_mass - Z_MASS) < fabs(invMass2Lep - Z_MASS)) {

	invMass2Lep = inv_mass;

	ZLepton1 = lep1.v;
	ZLepton2 = lep2.v;

	for (std::vector<Lepton>::iterator lep3_iterator=AnalysisLeptons.begin();
	     lep3_iterator!=AnalysisLeptons.end(); lep3_iterator++) {
	
	  if (lep3_iterator == lep1_iterator) continue;
	  if (lep3_iterator == lep2_iterator) continue;

	  Lepton lep3 = (*lep3_iterator);

	  if (lep3.v.Pt() <= 20) break;
	  
	  WLepton = lep3.v;
	}
      }
    }
  }

  invMass3Lep = (ZLepton1 + ZLepton2 + WLepton).M();

  FillHistograms(theChannel, AtLeast3Leptons);


  // HasZCandidate
  //----------------------------------------------------------------------------
  if (invMass2Lep < 71 || invMass2Lep > 111) return;

  FillHistograms(theChannel, HasZCandidate);


  // HasWCandidate
  //----------------------------------------------------------------------------
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

  GetInputParameters()->TheNamedBool  ("useFakes",   useFakes);
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
Bool_t AnalysisWZ::ElectronBDT(UInt_t iElec, Bool_t isLoose)
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

  Bool_t pass = (T_Elec_MVA->at(iElec) > mvaCut);

  if (isLoose) pass = !pass;

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
Bool_t AnalysisWZ::ElectronIsolation(UInt_t iElec, Bool_t isLoose) 
{
  Bool_t pass = (T_Elec_pfComb->at(iElec) < 0.15);

  if (isLoose) pass = !pass;

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
Bool_t AnalysisWZ::MuonCloseToPV(UInt_t iMuon, Bool_t isLoose)
{
  Bool_t pass = (fabs(T_Muon_dzPVBiasedPV->at(iMuon)) <= 0.1);

  Double_t MaxMuIP = (T_Muon_Pt->at(iMuon) < 20) ? 0.01 : 0.02;

  if (isLoose)
    {
      pass &= (fabs(T_Muon_IP2DBiasedPV->at(iMuon)) > MaxMuIP);
      pass &= (fabs(T_Muon_IP2DBiasedPV->at(iMuon)) <= 0.2);
    }
  else
    {
      pass &= (fabs(T_Muon_IP2DBiasedPV->at(iMuon)) <= MaxMuIP);
    }

  return pass;
}


//------------------------------------------------------------------------------
// MuonIsolation
//------------------------------------------------------------------------------
Bool_t AnalysisWZ::MuonIsolation(UInt_t iMuon, Bool_t isLoose) 
{
  Bool_t pass = true;

  Double_t mueta = T_Muon_Eta->at(iMuon);
  Double_t mupt  = T_Muon_Pt ->at(iMuon);

  Double_t isoCut = -1;

  if (mupt <= 20) isoCut = (fabs(mueta) < 1.479) ? 0.82 : 0.86;
  else            isoCut = (fabs(mueta) < 1.479) ? 0.86 : 0.82;

  if (isLoose)
    {
      pass &= (T_Muon_MVARings->at(iMuon) < isoCut);
      pass &= (T_Muon_MVARings->at(iMuon) >= -0.6);
    }
  else
    {
      pass &= (T_Muon_MVARings->at(iMuon) >= isoCut);
    }
  
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
  if (iCut < AtLeast3Leptons) return;

  FillChannelCounters(iChannel, iCut);

  Double_t hweight = efficiency_weight * xs_weight;

  Double_t pt1, pt2;
  
  if (ZLepton1.Pt() > ZLepton2.Pt())
    {
      pt1 = ZLepton1.Pt(); pt2 = ZLepton2.Pt();
    }
  else
    {
      pt1 = ZLepton2.Pt(); pt2 = ZLepton1.Pt();
    }

  hNPV        [iChannel][iCut]->Fill(T_Vertex_z->size(),        hweight);
  hMET        [iChannel][iCut]->Fill(T_METPFTypeI_ET,           hweight);
  hSumCharges [iChannel][iCut]->Fill(sumCharges,                hweight);
  hInvMass2Lep[iChannel][iCut]->Fill(invMass2Lep,               hweight);
  hInvMass3Lep[iChannel][iCut]->Fill(invMass3Lep,               hweight);
  hPtLepton1  [iChannel][iCut]->Fill(AnalysisLeptons[0].v.Pt(), hweight);
  hPtLepton2  [iChannel][iCut]->Fill(AnalysisLeptons[1].v.Pt(), hweight);
  hPtLepton3  [iChannel][iCut]->Fill(AnalysisLeptons[2].v.Pt(), hweight);
  hPtZLepton1 [iChannel][iCut]->Fill(pt1,                       hweight);
  hPtZLepton2 [iChannel][iCut]->Fill(pt2,                       hweight);
  hPtWLepton  [iChannel][iCut]->Fill(WLepton.Pt(),              hweight);
}


//------------------------------------------------------------------------------
// isSignalMCInsideZmassRange
//------------------------------------------------------------------------------
const bool AnalysisWZ::isSignalMCInsideZmassRange(const float & masslow,
						  const float & masshigh) const
{
  float masszcand = 0.0;

  const UInt_t ntauSt3  = T_Gen_TauSt3_PID->size();
  const UInt_t nmuonSt3 = T_Gen_MuonSt3_PID->size();
  const UInt_t nelecSt3 = T_Gen_ElecSt3_PID->size();

  if (ntauSt3 >= 2)
    {
      std::vector<std::pair<int,TLorentzVector> > precand;

      for (UInt_t i=0; i<ntauSt3; ++i)
	{
	  TLorentzVector Tau(T_Gen_TauSt3_Px->at(i),
			     T_Gen_TauSt3_Py->at(i),
			     T_Gen_TauSt3_Pz->at(i),
			     T_Gen_TauSt3_Energy->at(i));
	  
	  precand.push_back(std::pair<int, TLorentzVector>(T_Gen_TauSt3_PID->at(i), Tau));
	}
      

      // Construct the Z candidate
      //------------------------------------------------------------------------
      for (UInt_t j=0; j<precand.size(); ++j)
	{
	  for (UInt_t i=j+1; i<precand.size(); ++i)
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

      for (UInt_t i=0; i<nmuonSt3; ++i)
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

      for (UInt_t i=0; i<nelecSt3; ++i)
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
// GenStudy
//------------------------------------------------------------------------------
void AnalysisWZ::GenStudy()
{
  UInt_t n_tau_St3     = 0;
  UInt_t n_muon_St3    = 0;
  UInt_t n_muon_Gen    = 0;
  UInt_t n_muon_Rec    = 0;
  UInt_t n_muon_Global = 0;
  UInt_t n_muon_PV     = 0;
  UInt_t n_muon_Iso    = 0;
  UInt_t n_muon_ID     = 0;
  UInt_t n_elec_St3    = 0;
  UInt_t n_elec_Gen    = 0;
  UInt_t n_elec_Rec    = 0;
  UInt_t n_elec_Global = 0;
  UInt_t n_elec_PV     = 0;
  UInt_t n_elec_Iso    = 0;
  UInt_t n_elec_ID     = 0;


  // Gen St3
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Gen_TauSt3_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Gen_TauSt3_Px->at(i), T_Gen_TauSt3_Py->at(i), T_Gen_TauSt3_Pz->at(i), T_Gen_TauSt3_Energy->at(i));
      
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.5) continue;

      h_pt_tau_St3 ->Fill(tlv.Pt());
      h_eta_tau_St3->Fill(tlv.Eta());
      n_tau_St3++;
    }

  for (UInt_t i=0; i<T_Gen_MuonSt3_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Gen_MuonSt3_Px->at(i), T_Gen_MuonSt3_Py->at(i), T_Gen_MuonSt3_Pz->at(i), T_Gen_MuonSt3_Energy->at(i));
    
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.4) continue;

      h_pt_muon_St3 ->Fill(tlv.Pt());
      h_eta_muon_St3->Fill(tlv.Eta());
      n_muon_St3++;
    }

  for (UInt_t i=0; i<T_Gen_ElecSt3_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Gen_ElecSt3_Px->at(i), T_Gen_ElecSt3_Py->at(i), T_Gen_ElecSt3_Pz->at(i), T_Gen_ElecSt3_Energy->at(i));
    
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.5) continue;

      h_pt_elec_St3 ->Fill(tlv.Pt());
      h_eta_elec_St3->Fill(tlv.Eta());
      n_elec_St3++;
    }

  if (n_tau_St3  > 0) h_n_tau_St3 ->Fill(n_tau_St3);
  if (n_muon_St3 > 0) h_n_muon_St3->Fill(n_muon_St3);
  if (n_elec_St3 > 0) h_n_elec_St3->Fill(n_elec_St3);


  // Gen
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Gen_Muon_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Gen_Muon_Px->at(i), T_Gen_Muon_Py->at(i), T_Gen_Muon_Pz->at(i), T_Gen_Muon_Energy->at(i));
    
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.4) continue;

      h_pt_muon_Gen ->Fill(tlv.Pt());
      h_eta_muon_Gen->Fill(tlv.Eta());
      n_muon_Gen++;
    }

  for (UInt_t i=0; i<T_Gen_Elec_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Gen_Elec_Px->at(i), T_Gen_Elec_Py->at(i), T_Gen_Elec_Pz->at(i), T_Gen_Elec_Energy->at(i));
    
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.5) continue;

      h_pt_elec_Gen ->Fill(tlv.Pt());
      h_eta_elec_Gen->Fill(tlv.Eta());
      n_elec_Gen++;
    }

  if (n_muon_Gen > 0) h_n_muon_Gen->Fill(n_muon_Gen);
  if (n_elec_Gen > 0) h_n_elec_Gen->Fill(n_elec_Gen);


  // Rec
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<T_Muon_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Muon_Px->at(i), T_Muon_Py->at(i), T_Muon_Pz->at(i), T_Muon_Energy->at(i));
    
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.4) continue;

      h_pt_muon_Rec ->Fill(tlv.Pt());
      h_eta_muon_Rec->Fill(tlv.Eta());
      n_muon_Rec++;

      Bool_t isnotglobal = ( T_Muon_IsAllStandAloneMuons->at(i)
			     && !T_Muon_IsGlobalMuon->at(i)
			     && !T_Muon_IsAllTrackerMuons->at(i) );

      if (isnotglobal) continue;

      h_pt_muon_Global ->Fill(tlv.Pt());
      h_eta_muon_Global->Fill(tlv.Eta());
      n_muon_Global++;
      
      if (!MuonCloseToPV(i)) continue;

      h_pt_muon_PV ->Fill(tlv.Pt());
      h_eta_muon_PV->Fill(tlv.Eta());
      n_muon_PV++;

      if (tlv.Pt() <= 20)
	{
	  if  (fabs(tlv.Eta()) < 1.479) h_mvaRings_muon_r1->Fill(T_Muon_MVARings->at(i));
	  else                          h_mvaRings_muon_r2->Fill(T_Muon_MVARings->at(i));
	}
      else
	{
	  if  (fabs(tlv.Eta()) < 1.479) h_mvaRings_muon_r3->Fill(T_Muon_MVARings->at(i));
	  else                          h_mvaRings_muon_r4->Fill(T_Muon_MVARings->at(i));
	}

      if (!MuonIsolation(i)) continue;

      h_pt_muon_Iso ->Fill(tlv.Pt());
      h_eta_muon_Iso->Fill(tlv.Eta());
      n_muon_Iso++;

      if (!MuonID(i)) continue;

      h_pt_muon_ID ->Fill(tlv.Pt());
      h_eta_muon_ID->Fill(tlv.Eta());
      n_muon_ID++;
    }

  for (UInt_t i=0; i<T_Elec_Px->size(); ++i)
    {
      TLorentzVector tlv(T_Elec_Px->at(i), T_Elec_Py->at(i), T_Elec_Pz->at(i), T_Elec_Energy->at(i));
    
      if (tlv.Pt()        <=  10) continue;
      if (fabs(tlv.Eta()) >= 2.5) continue;

      h_pt_elec_Rec ->Fill(tlv.Pt());
      h_eta_elec_Rec->Fill(tlv.Eta());
      n_elec_Rec++;

      if (!ElectronID(i)) continue;

      h_pt_elec_Global ->Fill(tlv.Pt());
      h_eta_elec_Global->Fill(tlv.Eta());
      n_elec_Global++;

      if (!ElectronCloseToPV(i)) continue;

      h_pt_elec_PV ->Fill(tlv.Pt());
      h_eta_elec_PV->Fill(tlv.Eta());
      n_elec_PV++;

      if (!ElectronIsolation(i)) continue;

      h_pt_elec_Iso ->Fill(tlv.Pt());
      h_eta_elec_Iso->Fill(tlv.Eta());
      n_elec_Iso++;

      if (!ElectronBDT(i)) continue;
    
      h_pt_elec_ID ->Fill(tlv.Pt());
      h_eta_elec_ID->Fill(tlv.Eta());
      n_elec_ID++;
    }

  if (n_muon_Rec    > 0) h_n_muon_Rec   ->Fill(n_muon_Rec);
  if (n_muon_Global > 0) h_n_muon_Global->Fill(n_muon_Global);
  if (n_muon_PV     > 0) h_n_muon_PV    ->Fill(n_muon_PV);
  if (n_muon_Iso    > 0) h_n_muon_Iso   ->Fill(n_muon_Iso);
  if (n_muon_ID     > 0) h_n_muon_ID    ->Fill(n_muon_ID);

  if (n_elec_Rec    > 0) h_n_elec_Rec   ->Fill(n_elec_Rec);
  if (n_elec_Global > 0) h_n_elec_Global->Fill(n_elec_Global);
  if (n_elec_PV     > 0) h_n_elec_PV    ->Fill(n_elec_PV);
  if (n_elec_Iso    > 0) h_n_elec_Iso   ->Fill(n_elec_Iso);
  if (n_elec_ID     > 0) h_n_elec_ID    ->Fill(n_elec_ID);
}
