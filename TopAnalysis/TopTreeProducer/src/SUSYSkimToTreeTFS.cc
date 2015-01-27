// 
// Package:  SUSYSkimToTreeTFS
// Class:    SUSYSkimToTreeTFS
// 
/**\class SUSYSkimToTreeTFS SUSYSkimToTreeTFS.cc TopAnalysis/TopTreeProducer/src/SUSYSkimToTreeTFS.cc

Description: IFCA-Uniovi tree producer

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Javier Fernandez
//         Created:  Fri Jan 23 12:30:30 CET 2015
// $Id$
//
// 

// System include files
#include <memory>

// User include files
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimatorCSA14.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTfilters/interface/HLTHighLevel.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TFile.h"
#include "TTree.h"
#include <math.h> 
#include <stdlib.h>
#include <string.h>

#define NumberOfJetCollections 1

using namespace edm;
using namespace std;
using namespace reco;


//
// Class declaration
//
class SUSYSkimToTreeTFS : public edm::EDAnalyzer {
public:
  explicit SUSYSkimToTreeTFS(const edm::ParameterSet&);
  ~SUSYSkimToTreeTFS();
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  bool IsRealData;  
 
  TTree* Tree = new TTree();
  
  void SetJetInfo(int idx, edm::View<pat::Jet> JET, const reco::VertexCollection& vtxs, bool calojet);

  void SetJetBranchAddress(int idx, TString namecol, bool calojet);
 
  void LeptonicTauDecay(const reco::Candidate& tau, bool& elecdec, bool& muondec,
			int& pdgId, float& px, float& py, float& pz, float& energy);
  
  // Input tags
  edm::InputTag muoLabel_;
  edm::InputTag jetPFLabel_;
  edm::InputTag metLabel_;
  edm::InputTag PVLabel_;
  edm::InputTag trigLabel_;
  edm::InputTag elecLabel_;     
  edm::InputTag tauLabel_;
  edm::InputTag pfLabel_;
 
  edm::ParameterSet jetIdLoose_; 
  
  EGammaMvaEleEstimatorCSA14* myMVATrig;

  // Event variables
  bool T_EventF_HBHENoiseFilter,T_EventF_CSCTightHaloFilter;
  bool T_EventF_hcalLaserEventFilter,T_EventF_EcalDeadCellTriggerPrimitiveFilter;
  bool T_EventF_goodVertices,T_EventF_trackingFailureFilter,T_EventF_eeBadScFilter,T_EventF_ecalLaserCorrFilter;
  bool T_EventF_trkPOGFilters,T_EventF_trkPOG_manystripclus53X;
  bool T_EventF_trkPOG_toomanystripclus53X,T_EventF_trkPOG_logErrorTooManyClusters,T_EventF_METFilters;

  int T_Event_RunNumber;
  int T_Event_EventNumber;
  int T_Event_LuminosityBlock;
  int T_Event_processID;
  int T_Event_nPU;
  float T_Event_nTruePU;
  int T_Event_nPUm;
  int T_Event_nPUp;
  float T_Event_AveNTruePU;
  float T_Event_Rho;

  //Gen

  std::vector<float> *T_Gen_StopMass;
  std::vector<float> *T_Gen_Chi0Mass;
  std::vector<float> *T_Gen_CharginoMass;


  std::vector<int> *T_Gen_PromptMuon_pdgId;
  std::vector<float> *T_Gen_PromptMuon_Px;
  std::vector<float> *T_Gen_PromptMuon_Py;
  std::vector<float> *T_Gen_PromptMuon_Pz;
  std::vector<float> *T_Gen_PromptMuon_Energy;
  
  std::vector<int> *T_Gen_PromptElec_pdgId;
  std::vector<float> *T_Gen_PromptElec_Px;
  std::vector<float> *T_Gen_PromptElec_Py;
  std::vector<float> *T_Gen_PromptElec_Pz;
  std::vector<float> *T_Gen_PromptElec_Energy;
  
  std::vector<int> *T_Gen_Promptb_pdgId;
  std::vector<float> *T_Gen_Promptb_Px;
  std::vector<float> *T_Gen_Promptb_Py;
  std::vector<float> *T_Gen_Promptb_Pz;
  std::vector<float> *T_Gen_Promptb_Energy;

  std::vector<int> *T_Gen_PromptMuon_MpdgId;
  std::vector<float> *T_Gen_PromptMuon_MPx;
  std::vector<float> *T_Gen_PromptMuon_MPy;
  std::vector<float> *T_Gen_PromptMuon_MPz;
  std::vector<float> *T_Gen_PromptMuon_MEnergy;
  std::vector<int> *T_Gen_PromptMuon_MSt;

  std::vector<int> *T_Gen_PromptElec_MpdgId;
  std::vector<float> *T_Gen_PromptElec_MPx;
  std::vector<float> *T_Gen_PromptElec_MPy;
  std::vector<float> *T_Gen_PromptElec_MPz;
  std::vector<float> *T_Gen_PromptElec_MEnergy;
  std::vector<int> *T_Gen_PromptElec_MSt;

  std::vector<int> *T_Gen_Promptb_MpdgId;
  std::vector<float> *T_Gen_Promptb_MPx;
  std::vector<float> *T_Gen_Promptb_MPy;
  std::vector<float> *T_Gen_Promptb_MPz;
  std::vector<float> *T_Gen_Promptb_MEnergy;
  std::vector<int> *T_Gen_Promptb_MSt;
  
  std::vector<int> *T_Gen_Muon_pdgId;
  std::vector<float> *T_Gen_Muon_Px;
  std::vector<float> *T_Gen_Muon_Py;
  std::vector<float> *T_Gen_Muon_Pz;
  std::vector<float> *T_Gen_Muon_Energy;
  
  std::vector<int> *T_Gen_Elec_pdgId;
  std::vector<float> *T_Gen_Elec_Px;
  std::vector<float> *T_Gen_Elec_Py;
  std::vector<float> *T_Gen_Elec_Pz;
  std::vector<float> *T_Gen_Elec_Energy;
 
  std::vector<int> *T_Gen_FinalMuon_pdgId;
  std::vector<float> *T_Gen_FinalMuon_Px;
  std::vector<float> *T_Gen_FinalMuon_Py;
  std::vector<float> *T_Gen_FinalMuon_Pz;
  std::vector<float> *T_Gen_FinalMuon_Energy;

  std::vector<int> *T_Gen_FinalElec_pdgId;
  std::vector<float> *T_Gen_FinalElec_Px;
  std::vector<float> *T_Gen_FinalElec_Py;
  std::vector<float> *T_Gen_FinalElec_Pz;
  std::vector<float> *T_Gen_FinalElec_Energy;
 
  std::vector<int> *T_Gen_b_pdgId;
  std::vector<float> *T_Gen_b_Px;
  std::vector<float> *T_Gen_b_Py;
  std::vector<float> *T_Gen_b_Pz;
  std::vector<float> *T_Gen_b_Energy;

  std::vector<int> *T_Gen_Muon_MpdgId;
  std::vector<float> *T_Gen_Muon_MPx;
  std::vector<float> *T_Gen_Muon_MPy;
  std::vector<float> *T_Gen_Muon_MPz;
  std::vector<float> *T_Gen_Muon_MEnergy;
  std::vector<int> *T_Gen_Muon_MSt;

  std::vector<int> *T_Gen_Elec_MpdgId;
  std::vector<float> *T_Gen_Elec_MPx;
  std::vector<float> *T_Gen_Elec_MPy;
  std::vector<float> *T_Gen_Elec_MPz;
  std::vector<float> *T_Gen_Elec_MEnergy;
  std::vector<int> *T_Gen_Elec_MSt;

  std::vector<int> *T_Gen_b_MpdgId;
  std::vector<float> *T_Gen_b_MPx;
  std::vector<float> *T_Gen_b_MPy;
  std::vector<float> *T_Gen_b_MPz;
  std::vector<float> *T_Gen_b_MEnergy;
  std::vector<int> *T_Gen_b_MSt;

  std::vector<int>   *T_Gen_Stop_pdgId;	
  std::vector<int>   *T_Gen_Stop_MpdgId;
  std::vector<float> *T_Gen_Stop_energy;	
  std::vector<float> *T_Gen_Stop_pt;		
  std::vector<float> *T_Gen_Stop_eta;	
  std::vector<float> *T_Gen_Stop_phi;        
  
  std::vector<int>   *T_Gen_Chi0_pdgId;
  std::vector<int>   *T_Gen_Chi0_MpdgId;
  std::vector<float> *T_Gen_Chi0_energy;
  std::vector<float> *T_Gen_Chi0_pt;
  std::vector<float> *T_Gen_Chi0_eta;
  std::vector<float> *T_Gen_Chi0_phi;
  
  std::vector<int>   *T_Gen_t_pdgId;
  std::vector<int>   *T_Gen_t_MpdgId;
  std::vector<float> *T_Gen_t_energy;
  std::vector<float> *T_Gen_t_pt;
  std::vector<float> *T_Gen_t_eta;
  std::vector<float> *T_Gen_t_phi;

  std::vector<int>   *T_Gen_ChiPM_pdgId;
  std::vector<int>   *T_Gen_ChiPM_MpdgId;
  std::vector<float> *T_Gen_ChiPM_energy;
  std::vector<float> *T_Gen_ChiPM_pt;
  std::vector<float> *T_Gen_ChiPM_eta;
  std::vector<float> *T_Gen_ChiPM_phi;

  std::vector<int>   *T_Gen_Nu_pdgId;
  std::vector<int>   *T_Gen_Nu_MpdgId;
  std::vector<float> *T_Gen_Nu_energy;
  std::vector<float> *T_Gen_Nu_pt;
  std::vector<float> *T_Gen_Nu_eta;
  std::vector<float> *T_Gen_Nu_phi;

  std::vector<int>   *T_Gen_Z_pdgId;
  std::vector<int>   *T_Gen_Z_MpdgId;
  std::vector<float> *T_Gen_Z_energy;
  std::vector<float> *T_Gen_Z_pt;
  std::vector<float> *T_Gen_Z_eta;
  std::vector<float> *T_Gen_Z_phi;

  std::vector<int>   *T_Gen_W_pdgId;
  std::vector<int>   *T_Gen_W_MpdgId;
  std::vector<float> *T_Gen_W_energy;
  std::vector<float> *T_Gen_W_pt;
  std::vector<float> *T_Gen_W_eta;
  std::vector<float> *T_Gen_W_phi;

  std::vector<int>   *T_Gen_PromptTau_pdgId;
  std::vector<float> *T_Gen_PromptTau_Energy;
  std::vector<float> *T_Gen_PromptTau_Px;
  std::vector<float> *T_Gen_PromptTau_Py;
  std::vector<float> *T_Gen_PromptTau_Pz;
  std::vector<int>   *T_Gen_PromptTau_MpdgId;
  std::vector<float> *T_Gen_PromptTau_MEnergy;
  std::vector<float> *T_Gen_PromptTau_MPx;
  std::vector<float> *T_Gen_PromptTau_MPy;
  std::vector<float> *T_Gen_PromptTau_MPz;
  std::vector<int>   *T_Gen_PromptTau_MSt;
  
  std::vector<bool> *T_Gen_PromptTau_IsLepDec;
  std::vector<int> *T_Gen_PromptTau_LepDec_pdgId;
  std::vector<float> *T_Gen_PromptTau_LepDec_Px;
  std::vector<float> *T_Gen_PromptTau_LepDec_Py;
  std::vector<float> *T_Gen_PromptTau_LepDec_Pz;
  std::vector<float> *T_Gen_PromptTau_LepDec_Energy;
 
  std::vector<int>   *T_Gen_Tau_pdgId;
  std::vector<float> *T_Gen_Tau_Energy;
  std::vector<float> *T_Gen_Tau_Px;
  std::vector<float> *T_Gen_Tau_Py;
  std::vector<float> *T_Gen_Tau_Pz;
  std::vector<int>   *T_Gen_Tau_MpdgId;
  std::vector<float> *T_Gen_Tau_MEnergy;
  std::vector<float> *T_Gen_Tau_MPx;
  std::vector<float> *T_Gen_Tau_MPy;
  std::vector<float> *T_Gen_Tau_MPz;
  std::vector<int>   *T_Gen_Tau_MSt;
  
  std::vector<bool> *T_Gen_Tau_IsLepDec;
  std::vector<int> *T_Gen_Tau_LepDec_pdgId;
  std::vector<float> *T_Gen_Tau_LepDec_Px;
  std::vector<float> *T_Gen_Tau_LepDec_Py;
  std::vector<float> *T_Gen_Tau_LepDec_Pz;
  std::vector<float> *T_Gen_Tau_LepDec_Energy;
 
  //Muons
  std::vector<float>*T_Muon_Eta;
  std::vector<bool> *T_Muon_IsGlobalMuon;
  std::vector<bool> *T_Muon_IsGMPTMuons;
  std::vector<bool> *T_Muon_IsAllStandAloneMuons;
  std::vector<bool> *T_Muon_IsTMLastStationTight;       // penetration depth Tight selector
  std::vector<bool> *T_Muon_IsAllTrackerMuons;          // checks isTrackerMuon flag
  std::vector<bool> *T_Muon_IsTrackerMuonArbitrated;    // resolve ambiguity of sharing segments
  std::vector<bool> *T_Muon_IsAllArbitrated;            // all muons with the tracker muon arbitrated
  std::vector<float> *T_Muon_SegmentCompatibility; 
  std::vector<float> *T_Muon_trkKink;
  std::vector<float> *T_Muon_Px;
  std::vector<float> *T_Muon_Py;
  std::vector<float> *T_Muon_Pz;
  std::vector<float> *T_Muon_Pt;
  std::vector<float> *T_Muon_deltaPt;
  std::vector<float> *T_Muon_Energy;
  std::vector<int> *T_Muon_Charge;
  std::vector<float> *T_Muon_NormChi2GTrk;
  std::vector<int> *T_Muon_NValidHitsInTrk;
  std::vector<int> *T_Muon_NValidPixelHitsInTrk;
  std::vector<int> *T_Muon_NValidHitsSATrk;
  std::vector<int> *T_Muon_NValidHitsGTrk;
  std::vector<int> *T_Muon_NumOfMatchedStations;
  std::vector<int> *T_Muon_InnerTrackFound;
  std::vector<float> *T_Muon_Chi2InTrk;
  std::vector<float> *T_Muon_dofInTrk;
  std::vector<float> *T_Muon_dxyGTrack;
  std::vector<float> *T_Muon_dxyInTrack;
  std::vector<float> *T_Muon_dzGTrack;
  std::vector<float> *T_Muon_dzInTrack;
  std::vector<float> *T_Muon_IPwrtAveBSInTrack;
  std::vector<float> *T_Muon_chargedHadronIsoR04;
  std::vector<float> *T_Muon_neutralHadronIsoR04;
  std::vector<float> *T_Muon_neutralIsoPFweightR04;
  std::vector<float> *T_Muon_photonIsoR04;
  std::vector<float> *T_Muon_sumPUPtR04;
  std::vector<float> *T_Muon_chargedParticleIsoR03;
  std::vector<float> *T_Muon_chargedHadronIsoR03;
  std::vector<float> *T_Muon_neutralHadronIsoR03;
  std::vector<float> *T_Muon_photonIsoR03;
  std::vector<float> *T_Muon_sumPUPtR03;
  std::vector<float> *T_Muon_vz;
  std::vector<float> *T_Muon_vy;
  std::vector<float> *T_Muon_vx;
  std::vector<bool> *T_Muon_isPFMuon;
  std::vector<float> *T_Muon_PFMuonPt; 
  std::vector<float> *T_Muon_PFMuonPx;
  std::vector<float> *T_Muon_PFMuonPy;
  std::vector<float> *T_Muon_PFMuonPz;
  std::vector<float> *T_Muon_PFMuonE;
  std::vector<int> *T_Muon_NLayers;
  std::vector<float> *T_Muon_BestTrack_dxy;
  std::vector<float> *T_Muon_BestTrack_dz;
  std::vector<float> *T_Muon_BestTrack_vx;
  std::vector<float> *T_Muon_BestTrack_vy;
  std::vector<float> *T_Muon_BestTrack_vz;
  std::vector<float> *T_Muon_BestTrack_Px;
  std::vector<float> *T_Muon_BestTrack_Py;
  std::vector<float> *T_Muon_BestTrack_Pz;
  std::vector<float> *T_Muon_BestTrack_Pt;
  std::vector<float> *T_Muon_BestTrack_Phi;

  // Tau
  /*  std::vector<float> *T_Tau_Px;
      std::vector<float> *T_Tau_Py;
      std::vector<float> *T_Tau_Pz;
      std::vector<float> *T_Tau_Energy;
      std::vector<int> *T_Tau_Charge;
  */

  // Vertex
  std::vector<float> *T_Vertex_x;
  std::vector<float> *T_Vertex_y;
  std::vector<float> *T_Vertex_z;
  std::vector<float> *T_Vertex_Chi2Prob;
  std::vector<float> *T_Vertex_ndof;
  std::vector<float> *T_Vertex_rho;
  std::vector<bool>  *T_Vertex_isFake;
  std::vector<int>   *T_Vertex_tracksSize;
  std::vector<int>   *T_Vertex_nTracks;
 
  
  //Electrons
  std::vector<float> *T_Elec_Eta;
  std::vector<float> *T_Elec_IPwrtAveBS;
  std::vector<float> *T_Elec_IPwrtPV;
  std::vector<float> *T_Elec_dzwrtPV;
  std::vector<float> *T_Elec_Px;
  std::vector<float> *T_Elec_Py;
  std::vector<float> *T_Elec_Pz;
  std::vector<float> *T_Elec_Pt;
  std::vector<float> *T_Elec_Energy;
  std::vector<int> *T_Elec_Charge;
  std::vector<float> *T_Elec_puChargedHadronIso;
  std::vector<float> *T_Elec_chargedHadronIso;
  std::vector<float> *T_Elec_neutralHadronIso;
  std::vector<float> *T_Elec_neutralIsoPFweight;
  std::vector<float> *T_Elec_photonIso;
  std::vector<float> *T_Elec_pfIsoEA03;

  std::vector<bool> *T_Elec_passConversionVeto;
  std::vector<float> *T_Elec_vz;
  std::vector<float> *T_Elec_vy;
  std::vector<float> *T_Elec_vx;
  std::vector<int> *T_Elec_nLost; 
  std::vector<int> *T_Elec_nHits;
  std::vector<float> *T_Elec_SC_Et;
  std::vector<float> *T_Elec_SC_Eta;
  std::vector<int> *T_Elec_nBrems;
  std::vector<float> *T_Elec_fBrem;
  std::vector<float> *T_Elec_eSuperClusterOverP;
  std::vector<float> *T_Elec_ecalEnergy;
  std::vector<float> *T_Elec_dr03TkSumPt;
  std::vector<float> *T_Elec_dr03EcalSumEt;
  std::vector<float> *T_Elec_dr03HcalSumEt;
  std::vector<bool> *T_Elec_isEB;
  std::vector<bool> *T_Elec_isEE;
  std::vector<bool> *T_Elec_isPF;
  std::vector<float> *T_Elec_PFElecPt;
  std::vector<float> *T_Elec_PFElecPx;
  std::vector<float> *T_Elec_PFElecPy;
  std::vector<float> *T_Elec_PFElecPz;
  std::vector<float> *T_Elec_PFElecE;
  std::vector<float> *T_Elec_deltaPhiIn;
  std::vector<float> *T_Elec_deltaEtaIn;
  std::vector<float> *T_Elec_sigmaIetaIeta;
  std::vector<bool>   *T_Elec_isEcalDriven; 
  std::vector<float> *T_Elec_HtoE;
  std::vector<float> *T_Elec_MVAoutput;

  //Jets vector
  std::vector<float> *T_Jet_Px[NumberOfJetCollections];
  std::vector<float> *T_Jet_Py[NumberOfJetCollections];
  std::vector<float> *T_Jet_Pz[NumberOfJetCollections];
  std::vector<float> *T_Jet_Et[NumberOfJetCollections];
  std::vector<float> *T_Jet_Eta[NumberOfJetCollections];
  std::vector<float> *T_Jet_Corr[NumberOfJetCollections];
  std::vector<float> *T_Jet_Energy[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_HighEffTC[NumberOfJetCollections];  
  std::vector<float> *T_Jet_Tag_CombInclusiveSVtxV2[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_CombMVA[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_CombSVtx[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_pfCombinedSVtx[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_JetBProb[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_JetProb[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_HighPurSimpSVtx[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_HighEffSimpSVtx[NumberOfJetCollections];
  std::vector<float> *T_Jet_Tag_HighPurTC[NumberOfJetCollections];
  std::vector<float> *T_Jet_Parton_Px[NumberOfJetCollections];
  std::vector<float> *T_Jet_Parton_Py[NumberOfJetCollections];
  std::vector<float> *T_Jet_Parton_Pz[NumberOfJetCollections];
  std::vector<float> *T_Jet_Parton_Energy[NumberOfJetCollections];
  std::vector<int> *T_Jet_Parton_Flavour[NumberOfJetCollections];  
  std::vector<float> *T_Jet_Uncertainty[NumberOfJetCollections];
  
  
  std::vector<float> *T_Jet_CharHadEnergyFrac[NumberOfJetCollections];
  std::vector<float> *T_Jet_NeutHadEnergyFrac[NumberOfJetCollections];
  std::vector<float> *T_Jet_CharEmEnergyFrac[NumberOfJetCollections]; 
  std::vector<float> *T_Jet_NeutEmEnergyFrac[NumberOfJetCollections];
  std::vector<float> *T_Jet_CharHadEnergy[NumberOfJetCollections];
  std::vector<float> *T_Jet_NeutHadEnergy[NumberOfJetCollections];
  std::vector<float> *T_Jet_CharEmEnergy[NumberOfJetCollections]; 
  std::vector<float> *T_Jet_NeutEmEnergy[NumberOfJetCollections];
  std::vector<int> *T_Jet_MuonMultiplicity[NumberOfJetCollections];
  std::vector<int> *T_Jet_NeutralMultiplicity[NumberOfJetCollections];
  std::vector<int> *T_Jet_ChargedMultiplicity[NumberOfJetCollections];
  std::vector<bool> *T_Jet_IDLoose[NumberOfJetCollections];
  std::vector<int> *T_Jet_nDaughters[NumberOfJetCollections];
 
  std::vector<float> *T_Jet_GenJet_InvisibleE[NumberOfJetCollections];
  std::vector<float> *T_Jet_GenJet_Px[NumberOfJetCollections];
  std::vector<float> *T_Jet_GenJet_Py[NumberOfJetCollections];
  std::vector<float> *T_Jet_GenJet_Pz[NumberOfJetCollections];
  std::vector<float> *T_Jet_GenJet_Eta[NumberOfJetCollections];
  std::vector<float> *T_Jet_GenJet_Et[NumberOfJetCollections];
  std::vector<float> *T_Jet_GenJet_Energy[NumberOfJetCollections];
  std::vector<bool> *T_Jet_IsGenJet[NumberOfJetCollections];      

  
  //MET 
  float T_METPF_ET;
  float T_METPF_Phi;
  float T_METgen_ET;
  float T_METgen_Phi;
  
  //HLT
   bool T_HLT_Mu_vx;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

SUSYSkimToTreeTFS::SUSYSkimToTreeTFS(const edm::ParameterSet& iConfig):
//  theHistosFileName(iConfig.getUntrackedParameter<string>("histosFileName")),
  muoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag")),
  jetPFLabel_(iConfig.getUntrackedParameter<edm::InputTag>("jetPFTag")),
  metLabel_(iConfig.getUntrackedParameter<edm::InputTag>("metTag")),
  PVLabel_(iConfig.getUntrackedParameter<edm::InputTag>("PVTag")),
  trigLabel_(iConfig.getUntrackedParameter<edm::InputTag>("trigTag")),
  elecLabel_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag")),
  tauLabel_(iConfig.getUntrackedParameter<edm::InputTag>("tauTag")),
  pfLabel_(iConfig.getUntrackedParameter<edm::InputTag>("pfTag"))
{
 //CSA14 EleMVAID
   std::vector<std::string> myManualCatWeigths;
     myManualCatWeigths.push_back("EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_50ns_EB_BDT.weights.xml");
       myManualCatWeigths.push_back("EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_50ns_EE_BDT.weights.xml");
 
         vector<string> myManualCatWeigthsTrig;
           string the_path;
              for (unsigned i = 0 ; i < myManualCatWeigths.size() ; i++){
                      the_path = edm::FileInPath ( myManualCatWeigths[i] ).fullPath();
                              myManualCatWeigthsTrig.push_back(the_path);
                                      }
                                        myMVATrig = new EGammaMvaEleEstimatorCSA14();
                                          myMVATrig->initialize("BDT",
                                            EGammaMvaEleEstimatorCSA14::kTrig,
                                                    true,
                                                            myManualCatWeigthsTrig);
 

}


SUSYSkimToTreeTFS::~SUSYSkimToTreeTFS()
{}



void
SUSYSkimToTreeTFS::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  // first: get all objects from the event.
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}

  //for the moment decide if is real data if collection of gen particles is found
  IsRealData = false;
  edm::Handle <reco::GenParticleCollection> genParticles;
  try {
    iEvent.getByLabel("prunedGenParticles", genParticles);
    //I need to call genParticles size to forze the exception
    int aux = genParticles->size();
    //to avoid warnings I add the line aux = 0
    aux = 0+aux;
  }
  catch(...) {IsRealData = true;} 

  edm::Handle<edm::View<pat::Muon> > muonHandle;
  iEvent.getByLabel(muoLabel_,muonHandle);

  edm::Handle<edm::View<pat::Electron> > electronHandle;
  iEvent.getByLabel(elecLabel_, electronHandle);

  /*  edm::Handle<edm::View<pat::Tau> > tauHandle;
      iEvent.getByLabel(tauLabel_, tauHandle);
  */ 

  edm::Handle<pat::PackedCandidateCollection> pfHandle;
  iEvent.getByLabel(pfLabel_, pfHandle);

  edm::Handle<edm::View<pat::Jet> > jetPFHandle;
  iEvent.getByLabel(jetPFLabel_,jetPFHandle);
  edm::View<pat::Jet> jetsPF = *jetPFHandle;

   edm::Handle<pat::METCollection> patMET;
  iEvent.getByLabel(metLabel_,patMET);

  edm::Handle<VertexCollection> vertex;
  iEvent.getByLabel(PVLabel_,vertex);
  const reco::VertexCollection& vtxs = *(vertex.product());

  
  //Events
  EventID eventID = iEvent.id();
  T_Event_EventNumber = eventID.event();
  //cout<<"T_Event_EventNumber: "<<T_Event_EventNumber<<endl;
  T_Event_RunNumber = eventID.run();
  T_Event_LuminosityBlock = iEvent.luminosityBlock();  

  T_Event_nPU =-1;
  T_Event_nPUp=-1;
  T_Event_nPUm=-1;
  T_Event_AveNTruePU=-1.;
  float truePu=0.;
  if(!IsRealData){
    Handle<std::vector< PileupSummaryInfo > > puInfo;
    try {
      iEvent.getByLabel("addPileupInfo",puInfo);
      std::vector<PileupSummaryInfo>::const_iterator PVI;
      //The in-time crossing is getBunchCrossing = 0; negative ones are early, positive ones are late.
      for(PVI = puInfo->begin(); PVI != puInfo->end(); ++PVI) {

	//    std::cout << " Pileup Information: bunchXing, nvtx: " << PVI->getBunchCrossing() << " " << PVI->getPU_NumInteractions() << std::endl;
	if(PVI->getBunchCrossing()==0){
	  T_Event_nPU =PVI->getPU_NumInteractions();
	  T_Event_nTruePU=PVI->getTrueNumInteractions();

	}

	else if(PVI->getBunchCrossing()==-1){
	  T_Event_nPUm=PVI->getPU_NumInteractions();
	}
	else if(PVI->getBunchCrossing()==1){
	  T_Event_nPUp=PVI->getPU_NumInteractions();
	}
	truePu += PVI->getTrueNumInteractions();
      }
    } catch (...) {}
	
  }
  T_Event_AveNTruePU=truePu/3.;


     
  //Rho 
  edm::Handle<double> rhoH;
  iEvent.getByLabel(edm::InputTag("fixedGridRhoAll"),rhoH);
  T_Event_Rho=*rhoH; 
  

  //MET filters result
 T_EventF_HBHENoiseFilter=false;
 T_EventF_CSCTightHaloFilter=false;
 T_EventF_hcalLaserEventFilter=false;
 T_EventF_EcalDeadCellTriggerPrimitiveFilter=false;
 T_EventF_goodVertices=false;
 T_EventF_trackingFailureFilter=false;
 T_EventF_eeBadScFilter=false;
 T_EventF_ecalLaserCorrFilter=false;
 T_EventF_trkPOGFilters=false;
 T_EventF_trkPOG_manystripclus53X=false;
 T_EventF_trkPOG_toomanystripclus53X=false;
 T_EventF_trkPOG_logErrorTooManyClusters=false;
 T_EventF_METFilters=false;

  
  try {
    edm::Handle<edm::TriggerResults> metFilters;

    iEvent.getByLabel("TriggerResults::PAT", metFilters);

    const edm::TriggerNames &names = iEvent.triggerNames(*metFilters);
//    std::cout << "\n === MET Filters === " << std::endl;
    for (unsigned int i = 0, n = metFilters->size(); i < n; ++i) {
//        std::cout << "Trigger " << names.triggerName(i) << 
//                ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
//                ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)") 
//                << std::endl;
   if (names.triggerName(i) == "HBHENoiseFilter") {
      if (metFilters.product()->accept(i)) T_EventF_HBHENoiseFilter = true;}

   if (names.triggerName(i) == "CSCTightHaloFilter") {
      if (metFilters.product()->accept(i))  T_EventF_CSCTightHaloFilter= true;}

   if (names.triggerName(i) == "hcalLaserEventFilter") {
      if (metFilters.product()->accept(i))  T_EventF_hcalLaserEventFilter= true;}

   if (names.triggerName(i) == "EcalDeadCellTriggerPrimitiveFilter") {
      if (metFilters.product()->accept(i))  T_EventF_EcalDeadCellTriggerPrimitiveFilter= true;}

   if (names.triggerName(i) == "goodVertices") {
      if (metFilters.product()->accept(i))  T_EventF_goodVertices= true;}

   if (names.triggerName(i) == "trackingFailureFilter") {
      if (metFilters.product()->accept(i))  T_EventF_trackingFailureFilter= true;}

   if (names.triggerName(i) == "eeBadScFilter") {
      if (metFilters.product()->accept(i))  T_EventF_eeBadScFilter= true;}

   if (names.triggerName(i) == "ecalLaserCorrFilter") {
      if (metFilters.product()->accept(i))   T_EventF_ecalLaserCorrFilter= true;}

   if (names.triggerName(i) == "trkPOGFilters") {
      if (metFilters.product()->accept(i))   T_EventF_trkPOGFilters= true;}

//and the sub-filters
   if (names.triggerName(i) == "manystripclus53X") {
      if (metFilters.product()->accept(i))   T_EventF_trkPOG_manystripclus53X= true;}

   if (names.triggerName(i) == "toomanystripclus53X") {
      if (metFilters.product()->accept(i))   T_EventF_trkPOG_toomanystripclus53X= true;}

   if (names.triggerName(i) == "logErrorTooManyClusters") {
      if (metFilters.product()->accept(i))   T_EventF_trkPOG_logErrorTooManyClusters = true;}

// summary
   if (names.triggerName(i) == "METFilters") {
      if (metFilters.product()->accept(i)) T_EventF_METFilters= true;}
  }
  } catch(...) {;} 
  //HLT

  Handle<TriggerResults> trh;
  try {iEvent.getByLabel(trigLabel_,trh);
    unsigned int aux = trh.product()->size();
    aux = 0 + aux;
  } catch(...) {;}
//  const edm::TriggerNames &triggerNames_=iEvent.triggerNames(*trh);

//    T_HLT_Mu_vx=false;

//  for (unsigned int i= 0; i < trh.product()->size(); ++i) { 
    /*
      if (triggerNames_.triggerName(i) == "HLT_Mu_vx") {
      if (trh.product()->accept(i)) T_HLT_Mu_vx = true;
      }*/

//  } // loop in i

  //  if(!IsRealData){
  //Gen
  T_Gen_StopMass = new std::vector<float>;
  T_Gen_Chi0Mass = new std::vector<float>;
  T_Gen_CharginoMass = new std::vector<float>;


  T_Gen_PromptMuon_pdgId = new std::vector<int>;
  T_Gen_PromptMuon_Px = new std::vector<float>;
  T_Gen_PromptMuon_Py = new std::vector<float>;
  T_Gen_PromptMuon_Pz = new std::vector<float>;
  T_Gen_PromptMuon_Energy = new std::vector<float>;
  
  T_Gen_PromptMuon_MpdgId = new std::vector<int>;
  T_Gen_PromptMuon_MPx = new std::vector<float>;
  T_Gen_PromptMuon_MPy = new std::vector<float>;
  T_Gen_PromptMuon_MPz = new std::vector<float>;
  T_Gen_PromptMuon_MEnergy = new std::vector<float>;
  T_Gen_PromptMuon_MSt = new std::vector<int>;

  T_Gen_PromptElec_pdgId = new std::vector<int>;
  T_Gen_PromptElec_Px = new std::vector<float>;
  T_Gen_PromptElec_Py = new std::vector<float>;
  T_Gen_PromptElec_Pz = new std::vector<float>;
  T_Gen_PromptElec_Energy = new std::vector<float>;

  T_Gen_PromptElec_MpdgId = new std::vector<int>;
  T_Gen_PromptElec_MPx = new std::vector<float>;
  T_Gen_PromptElec_MPy = new std::vector<float>;
  T_Gen_PromptElec_MPz = new std::vector<float>;
  T_Gen_PromptElec_MEnergy = new std::vector<float>;
  T_Gen_PromptElec_MSt = new std::vector<int>;

  T_Gen_Promptb_pdgId = new std::vector<int>;
  T_Gen_Promptb_Px = new std::vector<float>;
  T_Gen_Promptb_Py = new std::vector<float>;
  T_Gen_Promptb_Pz = new std::vector<float>;
  T_Gen_Promptb_Energy = new std::vector<float>;

  T_Gen_Promptb_MpdgId = new std::vector<int>;
  T_Gen_Promptb_MPx = new std::vector<float>;
  T_Gen_Promptb_MPy = new std::vector<float>;
  T_Gen_Promptb_MPz = new std::vector<float>;
  T_Gen_Promptb_MEnergy = new std::vector<float>;
  T_Gen_Promptb_MSt = new std::vector<int>;

  T_Gen_FinalMuon_pdgId = new std::vector<int>;
  T_Gen_FinalMuon_Px = new std::vector<float>;
  T_Gen_FinalMuon_Py = new std::vector<float>;
  T_Gen_FinalMuon_Pz = new std::vector<float>;
  T_Gen_FinalMuon_Energy = new std::vector<float>;

  T_Gen_Muon_pdgId = new std::vector<int>;
  T_Gen_Muon_Px = new std::vector<float>;
  T_Gen_Muon_Py = new std::vector<float>;
  T_Gen_Muon_Pz = new std::vector<float>;
  T_Gen_Muon_Energy = new std::vector<float>;
  
  T_Gen_Muon_MpdgId = new std::vector<int>;
  T_Gen_Muon_MPx = new std::vector<float>;
  T_Gen_Muon_MPy = new std::vector<float>;
  T_Gen_Muon_MPz = new std::vector<float>;
  T_Gen_Muon_MEnergy = new std::vector<float>;
  T_Gen_Muon_MSt = new std::vector<int>;

  T_Gen_FinalElec_pdgId = new std::vector<int>;
  T_Gen_FinalElec_Px = new std::vector<float>;
  T_Gen_FinalElec_Py = new std::vector<float>;
  T_Gen_FinalElec_Pz = new std::vector<float>;
  T_Gen_FinalElec_Energy = new std::vector<float>;

  T_Gen_Elec_pdgId = new std::vector<int>;
  T_Gen_Elec_Px = new std::vector<float>;
  T_Gen_Elec_Py = new std::vector<float>;
  T_Gen_Elec_Pz = new std::vector<float>;
  T_Gen_Elec_Energy = new std::vector<float>;

  T_Gen_Elec_MpdgId = new std::vector<int>;
  T_Gen_Elec_MPx = new std::vector<float>;
  T_Gen_Elec_MPy = new std::vector<float>;
  T_Gen_Elec_MPz = new std::vector<float>;
  T_Gen_Elec_MEnergy = new std::vector<float>;
  T_Gen_Elec_MSt = new std::vector<int>;

  T_Gen_b_pdgId = new std::vector<int>;
  T_Gen_b_Px = new std::vector<float>;
  T_Gen_b_Py = new std::vector<float>;
  T_Gen_b_Pz = new std::vector<float>;
  T_Gen_b_Energy = new std::vector<float>;

  T_Gen_b_MpdgId = new std::vector<int>;
  T_Gen_b_MPx = new std::vector<float>;
  T_Gen_b_MPy = new std::vector<float>;
  T_Gen_b_MPz = new std::vector<float>;
  T_Gen_b_MEnergy = new std::vector<float>;
  T_Gen_b_MSt = new std::vector<int>;

 
  T_Gen_Stop_pdgId = new std::vector<int>;	   
  T_Gen_Stop_MpdgId = new std::vector<int>;
  T_Gen_Stop_energy = new std::vector<float>;   
  T_Gen_Stop_pt = new std::vector<float>;	   
  T_Gen_Stop_eta = new std::vector<float>;	   
  T_Gen_Stop_phi = new std::vector<float>;      
  
  T_Gen_Chi0_pdgId = new std::vector<int>;	   
  T_Gen_Chi0_MpdgId = new std::vector<int>;
  T_Gen_Chi0_energy = new std::vector<float>;   
  T_Gen_Chi0_pt = new std::vector<float>;	   
  T_Gen_Chi0_eta = new std::vector<float>;	   
  T_Gen_Chi0_phi = new std::vector<float>;      
  
  T_Gen_t_pdgId = new std::vector<int>;	   
  T_Gen_t_MpdgId = new std::vector<int>;
  T_Gen_t_energy = new std::vector<float>;   
  T_Gen_t_pt = new std::vector<float>;	   
  T_Gen_t_eta = new std::vector<float>;	   
  T_Gen_t_phi = new std::vector<float>;      

  T_Gen_ChiPM_pdgId = new std::vector<int>;	   
  T_Gen_ChiPM_MpdgId = new std::vector<int>;
  T_Gen_ChiPM_energy = new std::vector<float>;   
  T_Gen_ChiPM_pt = new std::vector<float>;	   
  T_Gen_ChiPM_eta = new std::vector<float>;	   
  T_Gen_ChiPM_phi = new std::vector<float>;      

  T_Gen_Nu_pdgId = new std::vector<int>;	
  T_Gen_Nu_MpdgId = new std::vector<int>;
  T_Gen_Nu_energy = new std::vector<float>;   
  T_Gen_Nu_pt = new std::vector<float>;	
  T_Gen_Nu_eta = new std::vector<float>;	
  T_Gen_Nu_phi = new std::vector<float>;      

  T_Gen_Z_pdgId = new std::vector<int>;	
  T_Gen_Z_MpdgId = new std::vector<int>;
  T_Gen_Z_energy = new std::vector<float>;   
  T_Gen_Z_pt = new std::vector<float>;	
  T_Gen_Z_eta = new std::vector<float>;	
  T_Gen_Z_phi = new std::vector<float>;      

  T_Gen_W_pdgId = new std::vector<int>;	
  T_Gen_W_MpdgId = new std::vector<int>;
  T_Gen_W_energy = new std::vector<float>;   
  T_Gen_W_pt = new std::vector<float>;	
  T_Gen_W_eta = new std::vector<float>;	
  T_Gen_W_phi = new std::vector<float>;      

  T_Gen_PromptTau_pdgId = new std::vector<int>;	
  T_Gen_PromptTau_Energy = new std::vector<float>;   
  T_Gen_PromptTau_Px = new std::vector<float>;	
  T_Gen_PromptTau_Py = new std::vector<float>;	
  T_Gen_PromptTau_Pz = new std::vector<float>;      
  T_Gen_PromptTau_MpdgId = new std::vector<int>;
  T_Gen_PromptTau_MEnergy = new std::vector<float>;   
  T_Gen_PromptTau_MPx = new std::vector<float>;	
  T_Gen_PromptTau_MPy = new std::vector<float>;	
  T_Gen_PromptTau_MPz = new std::vector<float>;      
  T_Gen_PromptTau_MSt = new std::vector<int>;

  T_Gen_PromptTau_IsLepDec = new std::vector<bool>;
  T_Gen_PromptTau_LepDec_pdgId = new std::vector<int>;
  T_Gen_PromptTau_LepDec_Px = new std::vector<float>;
  T_Gen_PromptTau_LepDec_Py = new std::vector<float>;
  T_Gen_PromptTau_LepDec_Pz = new std::vector<float>;
  T_Gen_PromptTau_LepDec_Energy = new std::vector<float>;  
  
  T_Gen_Tau_pdgId = new std::vector<int>;	
  T_Gen_Tau_Energy = new std::vector<float>;   
  T_Gen_Tau_Px = new std::vector<float>;	
  T_Gen_Tau_Py = new std::vector<float>;	
  T_Gen_Tau_Pz = new std::vector<float>;      
  T_Gen_Tau_MpdgId = new std::vector<int>;
  T_Gen_Tau_MEnergy = new std::vector<float>;   
  T_Gen_Tau_MPx = new std::vector<float>;	
  T_Gen_Tau_MPy = new std::vector<float>;	
  T_Gen_Tau_MPz = new std::vector<float>;      
  T_Gen_Tau_MSt = new std::vector<int>;

  T_Gen_Tau_IsLepDec = new std::vector<bool>;
  T_Gen_Tau_LepDec_pdgId = new std::vector<int>;
  T_Gen_Tau_LepDec_Px = new std::vector<float>;
  T_Gen_Tau_LepDec_Py = new std::vector<float>;
  T_Gen_Tau_LepDec_Pz = new std::vector<float>;
  T_Gen_Tau_LepDec_Energy = new std::vector<float>;  
  
  if(!IsRealData){
    edm::Handle<GenEventInfoProduct> genEvtInfo;
    iEvent.getByLabel("generator", genEvtInfo);
    //T_Event_PtHat =  genEvtInfo->hasBinningValues() ? (genEvtInfo->binningValues())[0] : 0.0;
    T_Event_processID= genEvtInfo->signalProcessID();

//    std::vector<SUSYGenParticle> genParVec;
//    genParVec.clear();

    for (size_t i = 0; i < genParticles->size(); ++i){//avoid pp
      const Candidate & p = (*genParticles)[i];
      int id = p.pdgId();
      int st = p.status();
//      int mom = (p.mother())->pdgId();
//        cout<<"pdgId: "<<id<<" Mom: "<<mom<<endl;

     if (!(abs(id) == 11 || abs(id) == 13 || abs(id) == 15 || abs(id)== 12 || abs(id)==14 || abs(id)== 16 || abs(id)==23 || abs(id)==24 || abs(id)==25 || abs(id)==5 || abs(id)==6 || abs(id) == 1000006 || abs(id) == 1000022 || abs(id) == 1000024)) continue;

      if (abs(id) == 1000006) T_Gen_StopMass->push_back(p.mass());      
      if (abs(id) == 1000022) T_Gen_Chi0Mass->push_back(p.mass());
      if (abs(id) == 1000024) T_Gen_CharginoMass->push_back(p.mass());
      
	// get mother 
	const GenParticle* gen_mom = static_cast<const GenParticle*> (p.mother());
	int m_id=id;
	if(gen_mom!=0) m_id = gen_mom -> pdgId();
	else m_id=0;

//	if(m_id==0) cout<<"Particle "<<id <<" with status "<<st<<" has no mother id"<<endl;
	
      if (abs(id) == 11 || abs(id) == 13 || abs(id) == 5){ // muons,electrons and b quarks	
	if(!(abs(m_id)== 6 || abs(m_id)== 22 || abs(m_id)== 24 || abs(m_id)== 23 || m_id==id)){  //Non-prompt,we do NOT want those coming from W/Z/top
      
		if (abs(id) == 11) {
		  T_Gen_Elec_pdgId->push_back(id);
		  T_Gen_Elec_Px->push_back(p.px());
		  T_Gen_Elec_Py->push_back(p.py());
		  T_Gen_Elec_Pz->push_back(p.pz());
		  T_Gen_Elec_Energy->push_back(p.energy());
		  if(gen_mom!=0){
		    T_Gen_Elec_MpdgId->push_back(m_id);
		    T_Gen_Elec_MPx->push_back(gen_mom->px());
		    T_Gen_Elec_MPy->push_back(gen_mom->py());
		    T_Gen_Elec_MPz->push_back(gen_mom->pz());
		    T_Gen_Elec_MEnergy->push_back(gen_mom->energy());
		    T_Gen_Elec_MSt->push_back(gen_mom->status());
		  }
		  else{
//		cout<<"Particle "<<id <<" with status "<<st<<"and no mother"<<endl;
		    T_Gen_Elec_MpdgId->push_back(111); //asign a pi0
		    T_Gen_Elec_MPx->push_back(0);
		    T_Gen_Elec_MPy->push_back(0);
		    T_Gen_Elec_MPz->push_back(0);
		    T_Gen_Elec_MEnergy->push_back(0);
		    T_Gen_Elec_MSt->push_back(0);
		  }
		}//ele
      
		else if (abs(id) == 13) {
		  T_Gen_Muon_pdgId->push_back(id);
		  T_Gen_Muon_Px->push_back(p.px());
		  T_Gen_Muon_Py->push_back(p.py());
		  T_Gen_Muon_Pz->push_back(p.pz());
		  T_Gen_Muon_Energy->push_back(p.energy());
	
		  T_Gen_Muon_MpdgId->push_back(m_id);
		  if(gen_mom!=0){
		    T_Gen_Muon_MPx->push_back(gen_mom->px());
		    T_Gen_Muon_MPy->push_back(gen_mom->py());
		    T_Gen_Muon_MPz->push_back(gen_mom->pz());
		    T_Gen_Muon_MEnergy->push_back(gen_mom->energy());
		    T_Gen_Muon_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_Muon_MPx->push_back(0);
		    T_Gen_Muon_MPy->push_back(0);
		    T_Gen_Muon_MPz->push_back(0);
		    T_Gen_Muon_MEnergy->push_back(0);
		    T_Gen_Muon_MSt->push_back(0);
		  }
		}//mu
      
		else if (abs(id) == 5) {
		  T_Gen_b_pdgId->push_back(id);
		  T_Gen_b_Px->push_back(p.px());
		  T_Gen_b_Py->push_back(p.py());
		  T_Gen_b_Pz->push_back(p.pz());
		  T_Gen_b_Energy->push_back(p.energy());
	
		  if(gen_mom!=0){
		    T_Gen_b_MpdgId->push_back(m_id);
		    T_Gen_b_MPx->push_back(gen_mom->px());
		    T_Gen_b_MPy->push_back(gen_mom->py());
		    T_Gen_b_MPz->push_back(gen_mom->pz());
		    T_Gen_b_MEnergy->push_back(gen_mom->energy());
		    T_Gen_b_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_b_MpdgId->push_back(21);//assign gluon
		    T_Gen_b_MPx->push_back(0);
		    T_Gen_b_MPy->push_back(0);
		    T_Gen_b_MPz->push_back(0);
		    T_Gen_b_MEnergy->push_back(0);
		    T_Gen_b_MSt->push_back(0);
		  }
		}//bs      
      	} //Non Prompt
      
        else if(m_id!=id){
	
		if (abs(id) == 11) {
		  T_Gen_PromptElec_pdgId->push_back(id);
		  T_Gen_PromptElec_Px->push_back(p.px());
		  T_Gen_PromptElec_Py->push_back(p.py());
		  T_Gen_PromptElec_Pz->push_back(p.pz());
		  T_Gen_PromptElec_Energy->push_back(p.energy());
	
		  T_Gen_PromptElec_MpdgId->push_back(m_id);
		  if(gen_mom!=0){
		    T_Gen_PromptElec_MPx->push_back(gen_mom->px());
		    T_Gen_PromptElec_MPy->push_back(gen_mom->py());
		    T_Gen_PromptElec_MPz->push_back(gen_mom->pz());
		    T_Gen_PromptElec_MEnergy->push_back(gen_mom->energy());
		    T_Gen_PromptElec_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_PromptElec_MPx->push_back(0);
		    T_Gen_PromptElec_MPy->push_back(0);
		    T_Gen_PromptElec_MPz->push_back(0);
		    T_Gen_PromptElec_MEnergy->push_back(0);
		    T_Gen_PromptElec_MSt->push_back(0);
		  }
		}//ele
      
		else if (abs(id) == 13) {
		  T_Gen_PromptMuon_pdgId->push_back(id);
		  T_Gen_PromptMuon_Px->push_back(p.px());
		  T_Gen_PromptMuon_Py->push_back(p.py());
		  T_Gen_PromptMuon_Pz->push_back(p.pz());
		  T_Gen_PromptMuon_Energy->push_back(p.energy());
	
		  T_Gen_PromptMuon_MpdgId->push_back(m_id);
		  if(gen_mom!=0){
		    T_Gen_PromptMuon_MPx->push_back(gen_mom->px());
		    T_Gen_PromptMuon_MPy->push_back(gen_mom->py());
		    T_Gen_PromptMuon_MPz->push_back(gen_mom->pz());
		    T_Gen_PromptMuon_MEnergy->push_back(gen_mom->energy());
		    T_Gen_PromptMuon_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_PromptMuon_MPx->push_back(0);
		    T_Gen_PromptMuon_MPy->push_back(0);
		    T_Gen_PromptMuon_MPz->push_back(0);
		    T_Gen_PromptMuon_MEnergy->push_back(0);
		    T_Gen_PromptMuon_MSt->push_back(0);
		  }
		}//mu
      
		else if (abs(id) == 5) {
		  T_Gen_Promptb_pdgId->push_back(id);
		  T_Gen_Promptb_Px->push_back(p.px());
		  T_Gen_Promptb_Py->push_back(p.py());
		  T_Gen_Promptb_Pz->push_back(p.pz());
		  T_Gen_Promptb_Energy->push_back(p.energy());
	
		  T_Gen_Promptb_MpdgId->push_back(m_id);
		  if(gen_mom!=0){
		    T_Gen_Promptb_MPx->push_back(gen_mom->px());
		    T_Gen_Promptb_MPy->push_back(gen_mom->py());
		    T_Gen_Promptb_MPz->push_back(gen_mom->pz());
		    T_Gen_Promptb_MEnergy->push_back(gen_mom->energy());
		    T_Gen_Promptb_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_Promptb_MPx->push_back(0);
		    T_Gen_Promptb_MPy->push_back(0);
		    T_Gen_Promptb_MPz->push_back(0);
		    T_Gen_Promptb_MEnergy->push_back(0);
		    T_Gen_Promptb_MSt->push_back(0);
		  }
		}//bs      
      	} //Prompt

	else if (st==1 && m_id==id){ //after radiation, if any... m_id==id

		if(abs(id) == 11){
		T_Gen_FinalElec_pdgId->push_back(id);
                T_Gen_FinalElec_Px->push_back(p.px());
                T_Gen_FinalElec_Py->push_back(p.py());
                T_Gen_FinalElec_Pz->push_back(p.pz());
                T_Gen_FinalElec_Energy->push_back(p.energy());
		}
		else if(abs(id) == 13){
                T_Gen_FinalMuon_pdgId->push_back(id);
                T_Gen_FinalMuon_Px->push_back(p.px());
                T_Gen_FinalMuon_Py->push_back(p.py());
                T_Gen_FinalMuon_Pz->push_back(p.pz());
                T_Gen_FinalMuon_Energy->push_back(p.energy());
                }
	}//after radiation, m_id==id
	
     } // e + mu + b


     else if (abs(id) == 15){ //taus
     
      if(abs(m_id)==23 || abs(m_id)==24) {//Prompt taus
	bool elecdec = false, muondec = false;
	int pdgId = 0;
	float px = 0, py = 0, pz = 0, energy = 0;
	LeptonicTauDecay(p, elecdec, muondec, pdgId, px, py, pz, energy);
	T_Gen_PromptTau_IsLepDec->push_back(elecdec || muondec);
	T_Gen_PromptTau_LepDec_pdgId->push_back(pdgId);
	T_Gen_PromptTau_LepDec_Px->push_back(px);
	T_Gen_PromptTau_LepDec_Py->push_back(py);
	T_Gen_PromptTau_LepDec_Pz->push_back(pz);
	T_Gen_PromptTau_LepDec_Energy->push_back(energy);    	
	T_Gen_PromptTau_pdgId->push_back(id);
	T_Gen_PromptTau_Px->push_back(p.px());
	T_Gen_PromptTau_Py->push_back(p.py());
	T_Gen_PromptTau_Pz->push_back(p.pz());
	T_Gen_PromptTau_Energy->push_back(p.energy());  
	
		  T_Gen_PromptTau_MpdgId->push_back(m_id);
		  if(gen_mom!=0){
		    T_Gen_PromptTau_MPx->push_back(gen_mom->px());
		    T_Gen_PromptTau_MPy->push_back(gen_mom->py());
		    T_Gen_PromptTau_MPz->push_back(gen_mom->pz());
		    T_Gen_PromptTau_MEnergy->push_back(gen_mom->energy());
		    T_Gen_PromptTau_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_PromptTau_MPx->push_back(0);
		    T_Gen_PromptTau_MPy->push_back(0);
		    T_Gen_PromptTau_MPz->push_back(0);
		    T_Gen_PromptTau_MEnergy->push_back(0);
		    T_Gen_PromptTau_MSt->push_back(0);
		  }
	}
	
      else if(m_id!=id){ //Non prompt taus
  	bool elecdec = false, muondec = false;
	int pdgId = 0;
	float px = 0, py = 0, pz = 0, energy = 0;
	LeptonicTauDecay(p, elecdec, muondec, pdgId, px, py, pz, energy);
	T_Gen_Tau_IsLepDec->push_back(elecdec || muondec);
	T_Gen_Tau_LepDec_pdgId->push_back(pdgId);
	T_Gen_Tau_LepDec_Px->push_back(px);
	T_Gen_Tau_LepDec_Py->push_back(py);
	T_Gen_Tau_LepDec_Pz->push_back(pz);
	T_Gen_Tau_LepDec_Energy->push_back(energy);  
	T_Gen_Tau_pdgId->push_back(id);
	T_Gen_Tau_Px->push_back(p.px());
	T_Gen_Tau_Py->push_back(p.py());
	T_Gen_Tau_Pz->push_back(p.pz());
	T_Gen_Tau_Energy->push_back(p.energy());  
		  T_Gen_Tau_MpdgId->push_back(m_id);
		  if(gen_mom!=0){
		    T_Gen_Tau_MPx->push_back(gen_mom->px());
		    T_Gen_Tau_MPy->push_back(gen_mom->py());
		    T_Gen_Tau_MPz->push_back(gen_mom->pz());
		    T_Gen_Tau_MEnergy->push_back(gen_mom->energy());
		    T_Gen_Tau_MSt->push_back(gen_mom->status());
		  }
		  else{
		    T_Gen_Tau_MPx->push_back(0);
		    T_Gen_Tau_MPy->push_back(0);
		    T_Gen_Tau_MPz->push_back(0);
		    T_Gen_Tau_MEnergy->push_back(0);
		    T_Gen_Tau_MSt->push_back(0);
		  }
    
	}//Non prompt taus      
     }//taus


     //VIP particles, first version
      else if( abs(id ) == 1000006 && m_id!=id){				   
									   
	T_Gen_Stop_pdgId       ->push_back( id );	   
	T_Gen_Stop_MpdgId      ->push_back( m_id);
	T_Gen_Stop_energy      ->push_back( p.energy() );	   
	T_Gen_Stop_pt          ->push_back( p.pt() );	   
	T_Gen_Stop_eta         ->push_back( p.eta() );	   
	T_Gen_Stop_phi         ->push_back( p.phi() );	   
      }                                                                    
      
      else if( abs(id ) == 1000022 && m_id!=id){			      	   
									      
	T_Gen_Chi0_pdgId       ->push_back( id );      	   
	T_Gen_Chi0_MpdgId      ->push_back( m_id );
	T_Gen_Chi0_energy      ->push_back( p.energy() );     	   
	T_Gen_Chi0_pt          ->push_back( p.pt() );	      
	T_Gen_Chi0_eta         ->push_back( p.eta() );	      
	T_Gen_Chi0_phi         ->push_back( p.phi() );	      
      }                                                                       
      
       else if( abs(id) == 1000024 && m_id!=id ){			      
									      
	T_Gen_ChiPM_pdgId       ->push_back( id );      
	T_Gen_ChiPM_MpdgId      ->push_back( m_id);
	T_Gen_ChiPM_energy      ->push_back( p.energy() );     
	T_Gen_ChiPM_pt          ->push_back( p.pt() );	      
	T_Gen_ChiPM_eta         ->push_back( p.eta() );	      
	T_Gen_ChiPM_phi         ->push_back( p.phi() );	      
      }                                                                       

      else if( abs(id) == 6 && m_id!=id ){				   
									   
	T_Gen_t_pdgId       ->push_back( id );	   
	T_Gen_t_MpdgId      ->push_back( m_id );
	T_Gen_t_energy      ->push_back( p.energy() );	   
	T_Gen_t_pt          ->push_back( p.pt() );	   
	T_Gen_t_eta         ->push_back( p.eta() );	   
	T_Gen_t_phi         ->push_back( p.phi() );	   
      }                                                                    


      else if( abs(id) == 24 && m_id!=id ){				   
									   
	T_Gen_W_pdgId       ->push_back( id );	   
	T_Gen_W_MpdgId      ->push_back( m_id );
	T_Gen_W_energy      ->push_back( p.energy() );	   
	T_Gen_W_pt          ->push_back( p.pt() );	   
	T_Gen_W_eta         ->push_back( p.eta() );	   
	T_Gen_W_phi         ->push_back( p.phi() );	   
      }                                                                    

      else if( abs(id) == 23 && m_id!=id ){				   
									   
	T_Gen_Z_pdgId       ->push_back( id );	   
	T_Gen_Z_MpdgId      ->push_back( m_id );
	T_Gen_Z_energy      ->push_back( p.energy() );	   
	T_Gen_Z_pt          ->push_back( p.pt() );	   
	T_Gen_Z_eta         ->push_back( p.eta() );	   
	T_Gen_Z_phi         ->push_back( p.phi() );	   
      }    
      
      else if( (abs(id) == 12 || abs(id)==14 || abs(id)==16) && m_id!=id){ //neutrinos
									   
	T_Gen_Nu_pdgId       ->push_back( id );	   
	T_Gen_Nu_MpdgId      ->push_back( m_id );
	T_Gen_Nu_energy      ->push_back( p.energy() );	   
	T_Gen_Nu_pt          ->push_back( p.pt() );	   
	T_Gen_Nu_eta         ->push_back( p.eta() );	   
	T_Gen_Nu_phi         ->push_back( p.phi() );	   
      }    
                                                                     
    
    } // loop over GenParticles    

  }// !IsRealData
    
  //**********************VERTEX****************************
  T_Vertex_x          = new std::vector<float>; 
  T_Vertex_y          = new std::vector<float>;
  T_Vertex_z          = new std::vector<float>;   
  T_Vertex_Chi2Prob   = new std::vector<float>;
  T_Vertex_ndof       = new std::vector<float>;
  T_Vertex_rho        = new std::vector<float>;
  T_Vertex_isFake     = new std::vector<bool>;
  T_Vertex_tracksSize = new std::vector<int>; 
  T_Vertex_nTracks    = new std::vector<int>; 
  
  //Vertex
  
  if (vtxs.size() != 0) {
    for (size_t i=0; i<vtxs.size(); i++) {
      T_Vertex_x         ->push_back(vtxs[i].x());
      T_Vertex_y         ->push_back(vtxs[i].y());
      T_Vertex_z         ->push_back(vtxs[i].z());
      T_Vertex_Chi2Prob  ->push_back(ChiSquaredProbability(vtxs[i].chi2(),vtxs[i].ndof()));
      T_Vertex_ndof      ->push_back(vtxs[i].ndof());
      T_Vertex_rho       ->push_back(vtxs[i].position().Rho());
      T_Vertex_isFake    ->push_back(vtxs[i].isFake());
      T_Vertex_tracksSize->push_back(vtxs[i].tracksSize());      
      T_Vertex_nTracks   ->push_back(vtxs[i].nTracks());      
    }
  } 
  
  
  
  //********************MUONS*******************
  T_Muon_Eta = new std::vector<float>;
  T_Muon_IsGlobalMuon = new std::vector<bool>;
  T_Muon_IsGMPTMuons = new std::vector<bool>;
  T_Muon_IsAllStandAloneMuons = new std::vector<bool>;
  T_Muon_IsTMLastStationTight = new std::vector<bool>; 
  T_Muon_IsAllTrackerMuons = new std::vector<bool>;
  T_Muon_IsTrackerMuonArbitrated = new std::vector<bool>;
  T_Muon_IsAllArbitrated = new std::vector<bool>;
  T_Muon_SegmentCompatibility= new std::vector<float>;
  T_Muon_trkKink  = new std::vector<float>;
  T_Muon_Px = new std::vector<float>;
  T_Muon_Py = new std::vector<float>;
  T_Muon_Pz = new std::vector<float>;
  T_Muon_Pt = new std::vector<float>;
  T_Muon_deltaPt = new std::vector<float>;
  T_Muon_Energy = new std::vector<float>;
  T_Muon_Charge = new std::vector<int>;
  T_Muon_NormChi2GTrk = new std::vector<float>;
  T_Muon_NValidHitsInTrk = new std::vector<int>;
  T_Muon_NValidPixelHitsInTrk = new std::vector<int>;
  T_Muon_NValidHitsSATrk = new std::vector<int>;
  T_Muon_NValidHitsGTrk = new std::vector<int>;
  T_Muon_NumOfMatchedStations = new std::vector<int>;
  T_Muon_Chi2InTrk = new std::vector<float>;
  T_Muon_dofInTrk = new std::vector<float>;
  T_Muon_dxyGTrack = new std::vector<float>;
  T_Muon_dxyInTrack = new std::vector<float>;
  T_Muon_dzGTrack = new std::vector<float>;
  T_Muon_dzInTrack = new std::vector<float>;
  T_Muon_IPwrtAveBSInTrack =  new std::vector<float>;
  T_Muon_InnerTrackFound=new std::vector<int>;
  T_Muon_chargedHadronIsoR04 = new std::vector<float>;
  T_Muon_neutralHadronIsoR04 = new std::vector<float>;
  T_Muon_neutralIsoPFweightR04 = new std::vector<float>;
  T_Muon_photonIsoR04 = new std::vector<float>;
  T_Muon_sumPUPtR04 = new std::vector<float>;
  T_Muon_chargedParticleIsoR03 = new std::vector<float>;
  T_Muon_chargedHadronIsoR03 = new std::vector<float>;
  T_Muon_neutralHadronIsoR03 = new std::vector<float>;
  T_Muon_photonIsoR03 = new std::vector<float>;
  T_Muon_sumPUPtR03 = new std::vector<float>;
  T_Muon_vz = new std::vector<float>;
  T_Muon_vy = new std::vector<float>;  
  T_Muon_vx = new std::vector<float>;
  T_Muon_PFMuonPt = new std::vector<float>;
  T_Muon_PFMuonPx = new std::vector<float>;
  T_Muon_PFMuonPy = new std::vector<float>;
  T_Muon_PFMuonPz = new std::vector<float>;
  T_Muon_PFMuonE = new std::vector<float>;
  T_Muon_isPFMuon = new std::vector<bool>;
  T_Muon_NLayers =  new std::vector<int>;
  T_Muon_BestTrack_dxy = new std::vector<float>;
  T_Muon_BestTrack_dz  = new std::vector<float>;
  T_Muon_BestTrack_vx  = new std::vector<float>;
  T_Muon_BestTrack_vy  = new std::vector<float>;
  T_Muon_BestTrack_vz  = new std::vector<float>;
  T_Muon_BestTrack_Px  = new std::vector<float>;
  T_Muon_BestTrack_Py  = new std::vector<float>;
  T_Muon_BestTrack_Pz  = new std::vector<float>;
  T_Muon_BestTrack_Pt  = new std::vector<float>;
  T_Muon_BestTrack_Phi = new std::vector<float>;

  //Muons
  //no estoy seguro de que haga falta ordenar (se puede usar para filtrar en el futuro)

  std::map<float,pat::Muon> muonMap;
  for (size_t i = 0; i< muonHandle->size(); ++i) {
    muonMap[(*muonHandle)[i].pt()]=(*muonHandle)[i];  
  }
  std::vector<pat::Muon> selected_muons;
  for( std::map<float,pat::Muon>::reverse_iterator rit=muonMap.rbegin(); rit!=muonMap.rend(); ++rit){
    selected_muons.push_back( (*rit).second );
  }

  
  for (size_t k = 0; k < selected_muons.size(); ++k) {
    float IP      = 9999.;
    float dZ      =9999.;
    float normchi2 = 9999;
    //quantities wrt global track    
    reco::TrackRef tr_globaltrack = selected_muons[k].globalTrack();       
    if (!tr_globaltrack.isNull() && selected_muons[k].isGlobalMuon()) {
    
      normchi2 = selected_muons[k].globalTrack()->normalizedChi2();

      if (vtxs.size() > 0) {

	// to calculate the ImpactParameter, its Error and the Significance use
	IP = selected_muons[k].globalTrack()->dxy(vtxs[0].position());
	dZ = selected_muons[k].globalTrack()->dz(vtxs[0].position());
      }
      
    
    }


    //cuantities wrt inner track
    reco::TrackRef tr_innertrack = selected_muons[k].innerTrack(); 
    int nhitsinnertracker = -1,pixelHits=-1, found=-1, nLayers=-1;
    float chi2innertracker=9999;
    float dofinnertracker=9999;
    float IPIn      = 9999., dZIn=9999.;
    float deltaPt =9999.;
    if (!tr_innertrack.isNull()) {
      if (vtxs.size() > 0) {

	// to calculate the ImpactParameter, its Error and the Significance use
	IPIn             =   selected_muons[k].innerTrack()->dxy(vtxs[0].position());
	dZIn		 =   selected_muons[k].innerTrack()->dz(vtxs[0].position());
      }
      nhitsinnertracker = tr_innertrack->hitPattern().numberOfValidTrackerHits();
      pixelHits = tr_innertrack->hitPattern().numberOfValidPixelHits(); 
      nLayers = tr_innertrack->hitPattern().trackerLayersWithMeasurement();
      chi2innertracker=tr_innertrack->chi2();
      dofinnertracker=tr_innertrack->ndof();
      deltaPt=tr_innertrack->ptError();
      found=tr_innertrack->found();

    }

    T_Muon_BestTrack_dxy->push_back(selected_muons[k].muonBestTrack()->dxy(vtxs[0].position()));
    T_Muon_BestTrack_dz->push_back(selected_muons[k].muonBestTrack()->dz(vtxs[0].position()));
    T_Muon_BestTrack_vx->push_back(selected_muons[k].muonBestTrack()->vx());
    T_Muon_BestTrack_vy->push_back(selected_muons[k].muonBestTrack()->vy());
    T_Muon_BestTrack_vz->push_back(selected_muons[k].muonBestTrack()->vz()); 
    T_Muon_BestTrack_Px->push_back(selected_muons[k].muonBestTrack()->px());
    T_Muon_BestTrack_Py->push_back(selected_muons[k].muonBestTrack()->py());
    T_Muon_BestTrack_Pz->push_back(selected_muons[k].muonBestTrack()->pz());
    T_Muon_BestTrack_Pt->push_back(selected_muons[k].muonBestTrack()->pt());
    T_Muon_BestTrack_Phi->push_back(selected_muons[k].muonBestTrack()->phi());

    reco::TrackRef tr_outtrack = selected_muons[k].standAloneMuon(); 

    float nhitsouttrack=9999;
    
    if (!tr_outtrack.isNull()) {
    
      nhitsouttrack=selected_muons[k].standAloneMuon()->hitPattern().numberOfValidMuonHits();
    
    }

    int numOfValidHitsGTrk=0;

    if (selected_muons[k].isGlobalMuon()){

      numOfValidHitsGTrk=selected_muons[k].globalTrack()-> hitPattern().numberOfValidMuonHits();
    }

    T_Muon_Eta->push_back(selected_muons[k].eta()); 
    
    T_Muon_IsGlobalMuon->push_back(selected_muons[k].isGlobalMuon());
    T_Muon_IsGMPTMuons->push_back(selected_muons[k].muonID("GlobalMuonPromptTight"));
    T_Muon_IsAllTrackerMuons->push_back(selected_muons[k].muonID("AllTrackerMuons"));
    T_Muon_IsTrackerMuonArbitrated->push_back(selected_muons[k].muonID("TrackerMuonArbitrated"));
    T_Muon_IsAllArbitrated->push_back(selected_muons[k].muonID("AllArbitrated"));
    T_Muon_IsAllStandAloneMuons->push_back(selected_muons[k].muonID("AllStandAloneMuons"));
    T_Muon_IsTMLastStationTight->push_back(selected_muons[k].muonID("TMLastStationTight"));
    T_Muon_Px->push_back(selected_muons[k].px());
    T_Muon_Py->push_back(selected_muons[k].py());
    T_Muon_Pz->push_back(selected_muons[k].pz());
    T_Muon_Pt->push_back(selected_muons[k].pt());
    T_Muon_InnerTrackFound->push_back(found);
    T_Muon_deltaPt->push_back(deltaPt);
    T_Muon_Energy->push_back(selected_muons[k].energy());
    T_Muon_Charge->push_back(selected_muons[k].charge());
    T_Muon_NormChi2GTrk->push_back(normchi2);
    T_Muon_NValidHitsInTrk->push_back(nhitsinnertracker);
    T_Muon_NValidPixelHitsInTrk->push_back(pixelHits);
    T_Muon_Chi2InTrk->push_back(chi2innertracker);
    T_Muon_dofInTrk->push_back(dofinnertracker);
    T_Muon_dxyGTrack->push_back(IP);
    T_Muon_dxyInTrack->push_back(IPIn);
    T_Muon_dzGTrack->push_back(dZ);
    T_Muon_dzInTrack->push_back(dZIn);

    T_Muon_IPwrtAveBSInTrack->push_back(selected_muons[k].dB());
    T_Muon_chargedHadronIsoR04->push_back(selected_muons[k].pfIsolationR04().sumChargedHadronPt);
    T_Muon_neutralHadronIsoR04->push_back(selected_muons[k].pfIsolationR04().sumNeutralHadronEt);
    T_Muon_photonIsoR04->push_back(selected_muons[k].pfIsolationR04().sumPhotonEt);
    T_Muon_chargedParticleIsoR03->push_back(selected_muons[k].pfIsolationR03().sumChargedParticlePt);
    T_Muon_chargedHadronIsoR03->push_back(selected_muons[k].pfIsolationR03().sumChargedHadronPt);
    T_Muon_neutralHadronIsoR03->push_back(selected_muons[k].pfIsolationR03().sumNeutralHadronEt);
    T_Muon_photonIsoR03->push_back(selected_muons[k].pfIsolationR03().sumPhotonEt);
    T_Muon_sumPUPtR03->push_back(selected_muons[k].pfIsolationR03().sumPUPt);
    T_Muon_sumPUPtR04->push_back(selected_muons[k].pfIsolationR04().sumPUPt);
    T_Muon_vz->push_back(selected_muons[k].vz());
    T_Muon_vy->push_back(selected_muons[k].vy());
    T_Muon_vx->push_back(selected_muons[k].vx());
    T_Muon_NValidHitsGTrk->push_back(numOfValidHitsGTrk);
    T_Muon_SegmentCompatibility->push_back(muon::segmentCompatibility(selected_muons[k]));
    T_Muon_trkKink ->push_back(selected_muons[k].combinedQuality().trkKink );
    T_Muon_NValidHitsSATrk->push_back(nhitsouttrack);
    T_Muon_NumOfMatchedStations->push_back(selected_muons[k].numberOfMatchedStations());
    T_Muon_isPFMuon->push_back(selected_muons[k].isPFMuon ());
    if(selected_muons[k].isPFMuon ())   {
      T_Muon_PFMuonPt->push_back(selected_muons[k].pfP4 ().pt());
      T_Muon_PFMuonPx->push_back(selected_muons[k].pfP4 ().px());
      T_Muon_PFMuonPy->push_back(selected_muons[k].pfP4 ().py());
      T_Muon_PFMuonPz->push_back(selected_muons[k].pfP4 ().pz());
      T_Muon_PFMuonE->push_back(selected_muons[k].pfP4 ().E());
    }
	
    else {
      T_Muon_PFMuonPt->push_back(-99999.);
      T_Muon_PFMuonPx->push_back(-99999.);
      T_Muon_PFMuonPy->push_back(-99999.);
      T_Muon_PFMuonPz->push_back(-99999.);
      T_Muon_PFMuonE->push_back(-99999.);
    }

    T_Muon_NLayers->push_back(nLayers);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PF-Reweight ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    double muon_neutralIsoPFweightR04 = 0;

    // Get a list of the PF candidates used to build the muon, to exclude them
    std::vector<reco::CandidatePtr> footprint;

    for (unsigned int i=0, n=selected_muons[k].numberOfSourceCandidatePtrs(); i<n; ++i) {
      footprint.push_back(selected_muons[k].sourceCandidatePtr(i));
    }

    // Loop on PF candidates
    for (unsigned int i=0, n=pfHandle->size(); i<n; ++i) {

      const pat::PackedCandidate &pf = (*pfHandle)[i];

      if (deltaR(pf,selected_muons[k]) < 0.4) {

	// PF candidate-based footprint removal
	if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfHandle,i)) != footprint.end()) {
	  continue;
	}

	if (pf.charge() == 0) {

	  double sumPU = 1.0;
	  double sumPV = 1.0;

	  for (unsigned int j=0, n=pfHandle->size(); j<n; ++j) {

	    const pat::PackedCandidate &pfj = (*pfHandle)[j];

	    if (pfj.charge() == 0) continue;

	    double sum = (pfj.pt() * pfj.pt()) / deltaR2(pf.eta(),pf.phi(),pfj.eta(),pfj.phi());

	    if (pfj.fromPV() >= 2) {
	      if (sum > 1.0) sumPV *= sum;
	    } else {
	      if (sum > 1.0) sumPU *= sum;
	    }
	  }

	  sumPU = 0.5 * log(sumPU);
	  sumPV = 0.5 * log(sumPV);

	  double pfw = sumPV / (sumPV + sumPU);

	  muon_neutralIsoPFweightR04 += (pfw * pf.pt());
	}
      }
    }

    T_Muon_neutralIsoPFweightR04->push_back(muon_neutralIsoPFweightR04);
  }

  
  //************ TAUS ************************

  /*
    T_Tau_Px = new std::vector<float>;
    T_Tau_Py = new std::vector<float>;
    T_Tau_Pz = new std::vector<float>;
    T_Tau_Energy = new std::vector<float>;
    T_Tau_Charge = new std::vector<int>;
    //no estoy seguro de que haga falta ordenar (se puede usar para filtrar en el futuro)
    std::map<float,pat::Tau> tauMap;
    for (size_t i = 0; i< tauHandle->size(); ++i) {
    tauMap[(*tauHandle)[i].pt()]=(*tauHandle)[i];
    }
    std::vector<pat::Tau> selected_Taus;
    for( std::map<float,pat::Tau>::reverse_iterator rit=tauMap.rbegin(); rit!=tauMap.rend(); ++rit){
    selected_Taus.push_back( (*rit).second );
    }
    for (size_t k = 0; k < selected_Taus.size(); ++k) {
    T_Tau_Px->push_back(selected_Taus[k].px());
    T_Tau_Py->push_back(selected_Taus[k].py());
    T_Tau_Pz->push_back(selected_Taus[k].pz());
    T_Tau_Energy->push_back(selected_Taus[k].energy());
    T_Tau_Charge->push_back(selected_Taus[k].charge());
    }
  */  


  //*****************ELECTRONS*****************
  T_Elec_Eta  = new std::vector<float>;
  T_Elec_IPwrtAveBS = new std::vector<float>;
  T_Elec_IPwrtPV = new std::vector<float>;
  T_Elec_dzwrtPV = new std::vector<float>;
  T_Elec_Px = new std::vector<float>;
  T_Elec_Py = new std::vector<float>;
  T_Elec_Pz = new std::vector<float>;
  T_Elec_Pt = new std::vector<float>;
  T_Elec_Energy = new std::vector<float>;
  T_Elec_Charge = new std::vector<int>;
  T_Elec_vz = new std::vector<float>;
  T_Elec_vy = new std::vector<float>;  
  T_Elec_vx = new std::vector<float>;
  T_Elec_nLost =  new std::vector<int>; 
  T_Elec_nHits  =  new std::vector<int>;
  T_Elec_SC_Et = new std::vector<float>;
  T_Elec_SC_Eta = new std::vector<float>; 
  T_Elec_nBrems  = new std::vector<int>;
  T_Elec_fBrem = new std::vector<float>;
  T_Elec_eSuperClusterOverP = new std::vector<float>;
  T_Elec_ecalEnergy = new std::vector<float>;
  T_Elec_dr03TkSumPt = new std::vector<float>; 
  T_Elec_dr03EcalSumEt = new std::vector<float>; 
  T_Elec_dr03HcalSumEt = new std::vector<float>; 
  T_Elec_passConversionVeto=new std::vector<bool>;
  T_Elec_isEB = new std::vector<bool>;
  T_Elec_isEE = new std::vector<bool>;
  T_Elec_sigmaIetaIeta = new std::vector<float>;
  T_Elec_deltaPhiIn = new std::vector<float>;
  T_Elec_deltaEtaIn =new  std::vector<float>;
  T_Elec_isEcalDriven = new std::vector<bool>;
  T_Elec_HtoE = new std::vector<float>;
  T_Elec_chargedHadronIso= new std::vector<float>;
  T_Elec_neutralHadronIso= new std::vector<float>;
  T_Elec_neutralIsoPFweight = new std::vector<float>;
  T_Elec_photonIso= new std::vector<float>;
  T_Elec_puChargedHadronIso = new std::vector<float>;
  T_Elec_isPF = new std::vector<bool>;
  T_Elec_PFElecPt = new std::vector<float>;
  T_Elec_PFElecPx = new std::vector<float>;
  T_Elec_PFElecPy = new std::vector<float>;
  T_Elec_PFElecPz = new std::vector<float>;
  T_Elec_PFElecE  = new std::vector<float>;
  T_Elec_MVAoutput= new std::vector<float>;

  std::map<float,pat::Electron> electronMap;
  for (size_t j = 0; j < electronHandle->size(); ++j) {
    electronMap[(*electronHandle)[j].pt()]=(*electronHandle)[j];   
  } 

  std::vector<pat::Electron> selected_electrons;
  for (std::map<float,pat::Electron>::reverse_iterator rit=electronMap.rbegin(); rit!=electronMap.rend(); ++rit) {
    selected_electrons.push_back( (*rit).second );
  }
 
  for (size_t k = 0; k < selected_electrons.size(); ++k) {
    float IP = 9999.;
    float dz = 9999.;
    int nLost = 9999, nHits=-9999;

    reco::GsfTrackRef trRef_elec = (*electronHandle)[k].gsfTrack();
    if (!trRef_elec.isNull()) {
      if (vtxs.size() > 0) {
	
	// to calculate the ImpactParameter, its Error and the Significance use
	IP             = fabs(trRef_elec->dxy(vtxs[0].position())) ;
	dz             = fabs(trRef_elec->dz(vtxs[0].position())) ;
      }
     	
      
      nHits = trRef_elec->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
      nLost= trRef_elec->numberOfLostHits(); 
      //     D0 = selected_electrons[k].gsfTrack()->dxy(beamSpot.position());
    }

    T_Elec_Eta->push_back(selected_electrons[k].eta());
    T_Elec_IPwrtAveBS->push_back(selected_electrons[k].dB());
    T_Elec_IPwrtPV->push_back(IP); 
    T_Elec_dzwrtPV->push_back(dz); 
    T_Elec_Px->push_back(selected_electrons[k].px());
    T_Elec_Py->push_back(selected_electrons[k].py());
    T_Elec_Pz->push_back(selected_electrons[k].pz());
    T_Elec_Pt->push_back(selected_electrons[k].pt());
    T_Elec_Energy->push_back(selected_electrons[k].energy());
    T_Elec_Charge->push_back(selected_electrons[k].charge());
    T_Elec_nBrems->push_back(selected_electrons[k].numberOfBrems());
    T_Elec_fBrem->push_back(selected_electrons[k].fbrem());
    T_Elec_eSuperClusterOverP->push_back(selected_electrons[k].eSuperClusterOverP());
    T_Elec_ecalEnergy->push_back(selected_electrons[k].ecalEnergy());
    T_Elec_dr03TkSumPt->push_back(selected_electrons[k].dr03TkSumPt());
    T_Elec_dr03EcalSumEt->push_back(selected_electrons[k].dr03EcalRecHitSumEt());
    T_Elec_dr03HcalSumEt->push_back(selected_electrons[k].dr03HcalTowerSumEt());
    T_Elec_vz->push_back(selected_electrons[k].vz());
    T_Elec_vy->push_back(selected_electrons[k].vy());
    T_Elec_vx->push_back(selected_electrons[k].vx());	
    T_Elec_nLost->push_back(nLost); 
    T_Elec_nHits->push_back(nHits);
    T_Elec_SC_Et->push_back( selected_electrons[k].superCluster()->energy()/TMath::CosH(selected_electrons[k].superCluster()->eta()));
    T_Elec_SC_Eta->push_back( selected_electrons[k].superCluster()->eta());
//    T_Elec_chargedHadronIso->push_back(selected_electrons[k].userFloat("electronPFIsoChHad03"));
//    T_Elec_neutralHadronIso->push_back(selected_electrons[k].userFloat("electronPFIsoNHad03"));
//    T_Elec_photonIso->push_back(selected_electrons[k].userFloat("electronPFIsoPhoton03"));
    T_Elec_chargedHadronIso->push_back(selected_electrons[k].chargedHadronIso());
    T_Elec_neutralHadronIso->push_back(selected_electrons[k].neutralHadronIso());
    T_Elec_photonIso->push_back(selected_electrons[k].photonIso());
    T_Elec_puChargedHadronIso->push_back(selected_electrons[k].puChargedHadronIso());
    T_Elec_passConversionVeto->push_back(selected_electrons[k].passConversionVeto());


    T_Elec_sigmaIetaIeta ->push_back( selected_electrons[k].sigmaIetaIeta());
    T_Elec_deltaPhiIn->push_back( selected_electrons[k].deltaPhiSuperClusterTrackAtVtx());
    T_Elec_deltaEtaIn->push_back( selected_electrons[k].deltaEtaSuperClusterTrackAtVtx());
    T_Elec_isEcalDriven -> push_back(selected_electrons[k].ecalDrivenSeed());
    T_Elec_HtoE ->push_back(selected_electrons[k].hadronicOverEm());
    T_Elec_isEB->push_back(selected_electrons[k].isEB());
    T_Elec_isEE->push_back(selected_electrons[k].isEE());
    T_Elec_isPF->push_back(selected_electrons[k].isPF());
    reco::GsfElectron::P4Kind pf = reco::GsfElectron::P4_PFLOW_COMBINATION;
    if(selected_electrons[k].isPF ())   {
      T_Elec_PFElecPt->push_back(selected_electrons[k].p4 (pf).pt());
      T_Elec_PFElecPx->push_back(selected_electrons[k].p4 (pf).px());
      T_Elec_PFElecPy->push_back(selected_electrons[k].p4 (pf).py());
      T_Elec_PFElecPz->push_back(selected_electrons[k].p4 (pf).pz());
      T_Elec_PFElecE->push_back(selected_electrons[k].p4 (pf).E());
    }

    else {
      T_Elec_PFElecPt->push_back(-99999.);
      T_Elec_PFElecPx->push_back(-99999.);
      T_Elec_PFElecPy->push_back(-99999.);
      T_Elec_PFElecPz->push_back(-99999.);
      T_Elec_PFElecE->push_back(-99999.);
    }
    T_Elec_MVAoutput->push_back( myMVATrig->mvaValue(selected_electrons[k],false));


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PF-Reweight ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    double electron_neutralIsoPFweightR04 = 0;

    // Get a list of the PF candidates used to build the muon, to exclude them
    std::vector<reco::CandidatePtr> footprint;

    for (unsigned int i=0, n=selected_electrons[k].numberOfSourceCandidatePtrs(); i<n; ++i) {
      footprint.push_back(selected_electrons[k].sourceCandidatePtr(i));
    }

    // Loop on PF candidates
    for (unsigned int i=0, n=pfHandle->size(); i<n; ++i) {

      const pat::PackedCandidate &pf = (*pfHandle)[i];

      if (deltaR(pf,selected_electrons[k]) < 0.4) {

	// PF candidate-based footprint removal
	if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfHandle,i)) != footprint.end()) {
	  continue;
	}

	if (pf.charge() == 0) {

	  double sumPU = 1.0;
	  double sumPV = 1.0;

	  for (unsigned int j=0, n=pfHandle->size(); j<n; ++j) {

	    const pat::PackedCandidate &pfj = (*pfHandle)[j];

	    if (pfj.charge() == 0) continue;

	    double sum = (pfj.pt() * pfj.pt()) / deltaR2(pf.eta(),pf.phi(),pfj.eta(),pfj.phi());

	    if (pfj.fromPV() >= 2) {
	      if (sum > 1.0) sumPV *= sum;
	    } else {
	      if (sum > 1.0) sumPU *= sum;
	    }
	  }

	  sumPU = 0.5 * log(sumPU);
	  sumPV = 0.5 * log(sumPV);

	  double pfw = sumPV / (sumPV + sumPU);

	  electron_neutralIsoPFweightR04 += (pfw * pf.pt());
	}
      }
    }

    T_Elec_neutralIsoPFweight->push_back(electron_neutralIsoPFweightR04);
  }


  SetJetInfo(0, jetsPF, vtxs, false);
  
  
  //******************MET***************
 
    const pat::METCollection *pMet = patMET.product();
    const pat::METCollection::const_iterator met = pMet->begin();
    const pat::MET theMET = *met;
  
   T_METPF_ET = theMET.pt();
   T_METPF_Phi = theMET.phi();
  
  if(!IsRealData){
    T_METgen_ET = theMET.genMET()->pt();
    T_METgen_Phi = theMET.genMET()->phi();
  }
 
  Tree->Fill();

  //Gen

  delete T_Gen_StopMass;
  delete T_Gen_Chi0Mass;
  delete T_Gen_CharginoMass;


  delete T_Gen_Stop_pdgId;	   
  delete T_Gen_Stop_MpdgId;
  delete T_Gen_Stop_energy;	   
  delete T_Gen_Stop_pt;	   
  delete T_Gen_Stop_eta;	   
  delete T_Gen_Stop_phi;        
  
  delete T_Gen_Chi0_pdgId;	   
  delete T_Gen_Chi0_MpdgId;
  delete T_Gen_Chi0_energy;	   
  delete T_Gen_Chi0_pt;	   
  delete T_Gen_Chi0_eta;	   
  delete T_Gen_Chi0_phi;        
  
  delete T_Gen_t_pdgId;	   
  delete T_Gen_t_MpdgId;
  delete T_Gen_t_energy;	   
  delete T_Gen_t_pt;	   	
  delete T_Gen_t_eta;	   
  delete T_Gen_t_phi;        

  delete T_Gen_ChiPM_pdgId;	   
  delete T_Gen_ChiPM_MpdgId;
  delete T_Gen_ChiPM_energy;	   
  delete T_Gen_ChiPM_pt;	   
  delete T_Gen_ChiPM_eta;	   
  delete T_Gen_ChiPM_phi;        

  delete T_Gen_Nu_pdgId;	
  delete T_Gen_Nu_MpdgId;
  delete T_Gen_Nu_energy;	
  delete T_Gen_Nu_pt;   	
  delete T_Gen_Nu_eta;	
  delete T_Gen_Nu_phi;        

  delete T_Gen_W_pdgId;	
  delete T_Gen_W_MpdgId;
  delete T_Gen_W_energy;	
  delete T_Gen_W_pt;   	
  delete T_Gen_W_eta;	
  delete T_Gen_W_phi;  
        
  delete T_Gen_Z_pdgId;	
  delete T_Gen_Z_MpdgId;
  delete T_Gen_Z_energy;	
  delete T_Gen_Z_pt;   	
  delete T_Gen_Z_eta;	
  delete T_Gen_Z_phi;    
      
  delete T_Gen_Muon_pdgId;
  delete T_Gen_Muon_Px;
  delete T_Gen_Muon_Py;
  delete T_Gen_Muon_Pz;
  delete T_Gen_Muon_Energy;

  delete T_Gen_Elec_pdgId;
  delete T_Gen_Elec_Px;
  delete T_Gen_Elec_Py;
  delete T_Gen_Elec_Pz;
  delete T_Gen_Elec_Energy;

  delete T_Gen_FinalMuon_pdgId;
  delete T_Gen_FinalMuon_Px;
  delete T_Gen_FinalMuon_Py;
  delete T_Gen_FinalMuon_Pz;
  delete T_Gen_FinalMuon_Energy;

  delete T_Gen_FinalElec_pdgId;
  delete T_Gen_FinalElec_Px;
  delete T_Gen_FinalElec_Py;
  delete T_Gen_FinalElec_Pz;
  delete T_Gen_FinalElec_Energy;

  delete T_Gen_b_pdgId;
  delete T_Gen_b_Px;
  delete T_Gen_b_Py;
  delete T_Gen_b_Pz;
  delete T_Gen_b_Energy;

  delete T_Gen_Muon_MpdgId;
  delete T_Gen_Muon_MPx;
  delete T_Gen_Muon_MPy;
  delete T_Gen_Muon_MPz;
  delete T_Gen_Muon_MEnergy;
  delete T_Gen_Muon_MSt;

  delete T_Gen_Elec_MpdgId;
  delete T_Gen_Elec_MPx;
  delete T_Gen_Elec_MPy;
  delete T_Gen_Elec_MPz;
  delete T_Gen_Elec_MEnergy;
  delete T_Gen_Elec_MSt;

  delete T_Gen_b_MpdgId;
  delete T_Gen_b_MPx;
  delete T_Gen_b_MPy;
  delete T_Gen_b_MPz;
  delete T_Gen_b_MEnergy;
  delete T_Gen_b_MSt;

  delete T_Gen_Tau_pdgId;	  
  delete T_Gen_Tau_MpdgId;
  delete T_Gen_Tau_Energy;	  
  delete T_Gen_Tau_Px;	  
  delete T_Gen_Tau_Py;	  
  delete T_Gen_Tau_Pz;        

  delete T_Gen_Tau_IsLepDec;
  delete T_Gen_Tau_LepDec_pdgId;
  delete T_Gen_Tau_LepDec_Px;
  delete T_Gen_Tau_LepDec_Py;
  delete T_Gen_Tau_LepDec_Pz;
  delete T_Gen_Tau_LepDec_Energy;


  delete T_Gen_PromptMuon_pdgId;
  delete T_Gen_PromptMuon_Px;
  delete T_Gen_PromptMuon_Py;
  delete T_Gen_PromptMuon_Pz;
  delete T_Gen_PromptMuon_Energy;

  delete T_Gen_PromptElec_pdgId;
  delete T_Gen_PromptElec_Px;
  delete T_Gen_PromptElec_Py;
  delete T_Gen_PromptElec_Pz;
  delete T_Gen_PromptElec_Energy;

  delete T_Gen_Promptb_pdgId;
  delete T_Gen_Promptb_Px;
  delete T_Gen_Promptb_Py;
  delete T_Gen_Promptb_Pz;
  delete T_Gen_Promptb_Energy;

  delete T_Gen_PromptMuon_MpdgId;
  delete T_Gen_PromptMuon_MPx;
  delete T_Gen_PromptMuon_MPy;
  delete T_Gen_PromptMuon_MPz;
  delete T_Gen_PromptMuon_MEnergy;
  delete T_Gen_PromptMuon_MSt;

  delete T_Gen_PromptElec_MpdgId;
  delete T_Gen_PromptElec_MPx;
  delete T_Gen_PromptElec_MPy;
  delete T_Gen_PromptElec_MPz;
  delete T_Gen_PromptElec_MEnergy;
  delete T_Gen_PromptElec_MSt;

  delete T_Gen_Promptb_MpdgId;
  delete T_Gen_Promptb_MPx;
  delete T_Gen_Promptb_MPy;
  delete T_Gen_Promptb_MPz;
  delete T_Gen_Promptb_MEnergy;
  delete T_Gen_Promptb_MSt;

  delete T_Gen_PromptTau_pdgId;	  
  delete T_Gen_PromptTau_MpdgId;
  delete T_Gen_PromptTau_Energy;	  
  delete T_Gen_PromptTau_Px;	  
  delete T_Gen_PromptTau_Py;	  
  delete T_Gen_PromptTau_Pz;        

  delete T_Gen_PromptTau_IsLepDec;
  delete T_Gen_PromptTau_LepDec_pdgId;
  delete T_Gen_PromptTau_LepDec_Px;
  delete T_Gen_PromptTau_LepDec_Py;
  delete T_Gen_PromptTau_LepDec_Pz;
  delete T_Gen_PromptTau_LepDec_Energy;

  //Vertex 
  delete T_Vertex_x;
  delete T_Vertex_y;
  delete T_Vertex_z;
  delete T_Vertex_Chi2Prob;
  delete T_Vertex_ndof;
  delete T_Vertex_rho;
  delete T_Vertex_isFake; 
  delete T_Vertex_tracksSize;
  delete T_Vertex_nTracks;

  //Muons
  delete T_Muon_Eta;
  delete T_Muon_IsGlobalMuon;
  delete T_Muon_IsGMPTMuons;
  delete T_Muon_IsAllTrackerMuons;
  delete T_Muon_IsTrackerMuonArbitrated;
  delete T_Muon_IsAllArbitrated;
  delete T_Muon_IsAllStandAloneMuons;
  delete T_Muon_IsTMLastStationTight;
  delete T_Muon_Px;
  delete T_Muon_Py;
  delete T_Muon_Pz;
  delete T_Muon_Pt;
  delete T_Muon_deltaPt;
  delete T_Muon_Energy;
  delete T_Muon_Charge;
  delete T_Muon_NormChi2GTrk;
  delete T_Muon_NValidHitsInTrk;
  delete T_Muon_NValidHitsSATrk;
  delete T_Muon_NValidHitsGTrk;
  delete T_Muon_NValidPixelHitsInTrk;
  delete T_Muon_Chi2InTrk;
  delete T_Muon_dofInTrk;
  delete T_Muon_dxyGTrack;
  delete T_Muon_dxyInTrack;
  delete T_Muon_dzGTrack;
  delete T_Muon_dzInTrack;
  delete T_Muon_IPwrtAveBSInTrack;
  delete T_Muon_NumOfMatchedStations;
  delete T_Muon_InnerTrackFound;
  delete T_Muon_vz;
  delete T_Muon_vy;
  delete T_Muon_vx;
  delete T_Muon_trkKink ;
  delete T_Muon_SegmentCompatibility;
  delete T_Muon_chargedParticleIsoR03;
  delete T_Muon_chargedHadronIsoR03;
  delete T_Muon_neutralHadronIsoR03;
  delete T_Muon_photonIsoR03;
  delete T_Muon_chargedHadronIsoR04;
  delete T_Muon_neutralHadronIsoR04;
  delete T_Muon_neutralIsoPFweightR04;
  delete T_Muon_photonIsoR04;
  delete T_Muon_sumPUPtR04;
  delete T_Muon_sumPUPtR03;
  delete T_Muon_PFMuonPt;
  delete T_Muon_PFMuonPx;
  delete T_Muon_PFMuonPy;
  delete T_Muon_PFMuonPz;
  delete T_Muon_PFMuonE;
  delete T_Muon_isPFMuon;
  delete T_Muon_NLayers;
  delete T_Muon_BestTrack_dxy;
  delete T_Muon_BestTrack_dz;
  delete T_Muon_BestTrack_vx;
  delete T_Muon_BestTrack_vy;
  delete T_Muon_BestTrack_vz;
  delete T_Muon_BestTrack_Px;
  delete T_Muon_BestTrack_Py;
  delete T_Muon_BestTrack_Pz;
  delete T_Muon_BestTrack_Pt;
  delete T_Muon_BestTrack_Phi;

  //Electrons
  delete T_Elec_Eta;
  delete T_Elec_IPwrtAveBS;
  delete T_Elec_IPwrtPV;
  delete T_Elec_dzwrtPV;
  delete T_Elec_Px;
  delete T_Elec_Py;
  delete T_Elec_Pz;
  delete T_Elec_Pt;
  delete T_Elec_Energy;
  delete T_Elec_Charge;
  delete T_Elec_vz;
  delete T_Elec_vy;
  delete T_Elec_vx;
  delete T_Elec_nLost;
  delete T_Elec_nHits;
  delete T_Elec_SC_Et;
  delete T_Elec_SC_Eta;
  delete T_Elec_nBrems;
  delete T_Elec_fBrem;
  delete T_Elec_eSuperClusterOverP;
  delete T_Elec_ecalEnergy;
  delete T_Elec_dr03TkSumPt;
  delete T_Elec_dr03EcalSumEt;
  delete T_Elec_dr03HcalSumEt;
  delete T_Elec_isEB;
  delete T_Elec_isEE;
  delete T_Elec_sigmaIetaIeta;
  delete T_Elec_deltaPhiIn;
  delete T_Elec_deltaEtaIn;
  delete T_Elec_isEcalDriven;
  delete T_Elec_HtoE;
  delete T_Elec_chargedHadronIso;
  delete T_Elec_neutralHadronIso;
  delete T_Elec_neutralIsoPFweight;
  delete T_Elec_photonIso;
  delete T_Elec_puChargedHadronIso;
  delete T_Elec_passConversionVeto;
  delete T_Elec_PFElecPt;
  delete T_Elec_PFElecPx;
  delete T_Elec_PFElecPy;
  delete T_Elec_PFElecPz;
  delete T_Elec_PFElecE;
  delete T_Elec_isPF;
  delete T_Elec_MVAoutput;

  /*  delete T_Tau_Px;
      delete T_Tau_Py;
      delete T_Tau_Pz;
      delete T_Tau_Energy ;
      delete T_Tau_Charge ;
  */

  //***************Jets***********
  for (int i = 0; i < NumberOfJetCollections; i++) {
    delete T_Jet_Px[i];
    delete T_Jet_Py[i];
    delete T_Jet_Pz[i];
    delete T_Jet_Et[i];
    delete T_Jet_Eta[i];
    delete T_Jet_Corr[i];
    delete T_Jet_Energy[i];
    delete T_Jet_Tag_HighEffTC[i];  
    delete T_Jet_Tag_CombInclusiveSVtxV2[i];
    delete T_Jet_Tag_CombMVA[i];
    delete T_Jet_Tag_CombSVtx[i];
    delete T_Jet_Tag_pfCombinedSVtx[i];
    delete T_Jet_Tag_JetBProb[i];
    delete T_Jet_Tag_JetProb[i];
    delete T_Jet_Tag_HighEffSimpSVtx[i];
    delete T_Jet_Tag_HighPurSimpSVtx[i];
    delete T_Jet_Tag_HighPurTC[i];
    delete T_Jet_Parton_Px[i];
    delete T_Jet_Parton_Py[i];
    delete T_Jet_Parton_Pz[i];
    delete T_Jet_Parton_Energy[i];
    delete T_Jet_Parton_Flavour[i];
    delete T_Jet_CharHadEnergyFrac[i];
    delete T_Jet_NeutHadEnergyFrac[i];
    delete T_Jet_CharEmEnergyFrac[i];
    delete T_Jet_NeutEmEnergyFrac[i];
    delete T_Jet_CharHadEnergy[i];
    delete T_Jet_NeutHadEnergy[i];
    delete T_Jet_CharEmEnergy[i];
    delete T_Jet_NeutEmEnergy[i];
    delete T_Jet_MuonMultiplicity[i];
    delete T_Jet_NeutralMultiplicity[i];
    delete T_Jet_ChargedMultiplicity[i];
    delete T_Jet_IDLoose[i];
    delete T_Jet_nDaughters[i];
    delete T_Jet_Uncertainty[i];

    delete T_Jet_GenJet_InvisibleE[i];
    delete T_Jet_GenJet_Px[i];
    delete T_Jet_GenJet_Py[i];
    delete T_Jet_GenJet_Pz[i];
    delete T_Jet_GenJet_Et[i];
    delete T_Jet_GenJet_Eta[i];
    delete T_Jet_GenJet_Energy[i];
    delete T_Jet_IsGenJet[i];
  }
  
  
}


void SUSYSkimToTreeTFS::SetJetInfo(int idx, edm::View<pat::Jet> JET, const reco::VertexCollection& vtxs, bool calojet) {
  T_Jet_Px[idx] = new std::vector<float>;
  T_Jet_Py[idx] = new std::vector<float>;
  T_Jet_Pz[idx] = new std::vector<float>;
  T_Jet_Et[idx] = new std::vector<float>;
  T_Jet_Eta[idx] = new std::vector<float>;
  T_Jet_Corr[idx] = new std::vector<float>;
  T_Jet_Energy[idx] = new std::vector<float>;
  T_Jet_Parton_Px[idx] = new std::vector<float>;
  T_Jet_Parton_Py[idx] = new std::vector<float>;
  T_Jet_Parton_Pz[idx] = new std::vector<float>;
  T_Jet_Parton_Energy[idx] = new std::vector<float>;
  T_Jet_Parton_Flavour[idx] = new std::vector<int>;
  T_Jet_Tag_HighEffTC[idx] = new std::vector<float>;
  T_Jet_Tag_CombInclusiveSVtxV2[idx] = new std::vector<float>;
  T_Jet_Tag_CombMVA[idx] = new std::vector<float>;
  T_Jet_Tag_CombSVtx[idx] = new std::vector<float>;
  T_Jet_Tag_pfCombinedSVtx[idx] = new std::vector<float>;
  T_Jet_Tag_JetBProb[idx] = new std::vector<float>;
  T_Jet_Tag_JetProb[idx] = new std::vector<float>;
  T_Jet_Tag_HighPurSimpSVtx[idx] = new std::vector<float>;
  T_Jet_Tag_HighEffSimpSVtx[idx] = new std::vector<float>;
  T_Jet_Tag_HighPurTC[idx] = new std::vector<float>;
  T_Jet_Uncertainty[idx] = new std::vector<float>;
 
  T_Jet_CharHadEnergyFrac[idx] = new std::vector<float>;
  T_Jet_NeutHadEnergyFrac[idx] = new std::vector<float>;
  T_Jet_CharEmEnergyFrac[idx] = new std::vector<float>;
  T_Jet_NeutEmEnergyFrac[idx] = new std::vector<float>;
  T_Jet_CharHadEnergy[idx] = new std::vector<float>;
  T_Jet_NeutHadEnergy[idx] = new std::vector<float>;
  T_Jet_CharEmEnergy[idx] = new std::vector<float>;
  T_Jet_NeutEmEnergy[idx] = new std::vector<float>;
  T_Jet_MuonMultiplicity[idx] = new std::vector<int>;
  T_Jet_NeutralMultiplicity[idx] = new std::vector<int>; 
  T_Jet_ChargedMultiplicity[idx] = new std::vector<int>;
  T_Jet_IDLoose[idx] = new std::vector<bool>;
  T_Jet_nDaughters[idx] = new std::vector<int>;
  
  T_Jet_GenJet_InvisibleE[idx] = new std::vector<float>;
  T_Jet_GenJet_Px[idx] = new std::vector<float>;
  T_Jet_GenJet_Py[idx] = new std::vector<float>;
  T_Jet_GenJet_Pz[idx] = new std::vector<float>;
  T_Jet_GenJet_Et[idx] = new std::vector<float>;
  T_Jet_GenJet_Eta[idx] = new std::vector<float>;
  T_Jet_GenJet_Energy[idx] = new std::vector<float>;
  T_Jet_IsGenJet[idx] = new std::vector<bool>;
  
  
  PFJetIDSelectionFunctor PFjetIDLoose( PFJetIDSelectionFunctor::FIRSTDATA,
					PFJetIDSelectionFunctor::LOOSE );

  // New 201
  // Create the JetCorrectorParameter objects, the order does not matter.
  	JetCorrectorParameters *L3JetPar,*L2JetPar,*L1JetPar;//*ResJetPar
    std::vector<JetCorrectorParameters> vPar;
  /*  if (IsRealData){
    ResJetPar = new JetCorrectorParameters("/gpfs/csic_users/jfernan/CMSSW_5_3_3_patch2/src/GTs/GR_P_V42_AN3_L2L3Residual_AK5PFchs.txt"); 
    L3JetPar  = new JetCorrectorParameters("/gpfs/csic_users/jfernan/CMSSW_5_3_3_patch2/src/GTs/GR_P_V42_AN3_L3Absolute_AK5PFchs.txt");
    L2JetPar  = new JetCorrectorParameters("/gpfs/csic_users/jfernan/CMSSW_5_3_3_patch2/src/GTs/GR_P_V42_AN3_L2Relative_AK5PFchs.txt");
    L1JetPar  = new JetCorrectorParameters("/gpfs/csic_users/jfernan/CMSSW_5_3_3_patch2/src/GTs/GR_P_V42_AN3_L1FastJet_AK5PFchs.txt");
    //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
    vPar.push_back(*L1JetPar);
    vPar.push_back(*L2JetPar);
    vPar.push_back(*L3JetPar);
    vPar.push_back(*ResJetPar);
    }
    else{*/
    L3JetPar  = new JetCorrectorParameters("PHYS14_V2_MC_L3Absolute_AK4PFchs.txt");
    L2JetPar  = new JetCorrectorParameters("PHYS14_V2_MC_L2Relative_AK4PFchs.txt");
    L1JetPar  = new JetCorrectorParameters("PHYS14_V2_MC_L1FastJet_AK4PFchs.txt");
    //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!!
    vPar.push_back(*L1JetPar);
    vPar.push_back(*L2JetPar);
    vPar.push_back(*L3JetPar);
    //}

    FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);

    float rho = T_Event_Rho;
  

   //Run1 Jet Uncertainties
    //edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    //iSetup.get<JetCorrectionsRecord>().get("AK5PF",JetCorParColl);
    //JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
    //JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar);
  JetCorrectorParameters *Total = new JetCorrectorParameters("Winter14_V5_DATA_UncertaintySources_AK5PFchs.txt", "Total");

  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(*Total);

  for (edm::View<pat::Jet>::const_iterator jet_iter=JET.begin(); jet_iter!= JET.end(); jet_iter++) { 
       double correction(1.0),et(0.0);
	 Jet mijetRAW = jet_iter->correctedJet("Uncorrected");
	 JetCorrector->setJetEta(jet_iter->eta());
	 JetCorrector->setJetPt(mijetRAW.pt());
	 JetCorrector->setJetA(jet_iter->jetArea());
	 JetCorrector->setRho(rho);
	 //   cout <<"et: "<<mijetRAW.et()<<" pt: "<<mijetRAW.pt()<<endl;

	 correction = JetCorrector->getCorrection();
	 et=(mijetRAW.et()*correction);
	 if(et<10) continue;
	 //	if(jet_iter->et()<10) continue;	
    T_Jet_Px[idx]->push_back(correction*mijetRAW.px());
    T_Jet_Py[idx]->push_back(correction*mijetRAW.py());
    T_Jet_Pz[idx]->push_back(correction*mijetRAW.pz());
    T_Jet_Et[idx]->push_back(et);
    T_Jet_Corr[idx]->push_back(correction);
/*	 
    T_Jet_Px[idx]->push_back(jet_iter->px());
    T_Jet_Py[idx]->push_back(jet_iter->py());
    T_Jet_Pz[idx]->push_back(jet_iter->pz());
    T_Jet_Et[idx]->push_back(jet_iter->et());*/

    T_Jet_Eta[idx]->push_back(jet_iter->eta());
    T_Jet_Energy[idx]->push_back(jet_iter->energy());

    float unc =-9999.;
    if(fabs(jet_iter->eta())<=5.2 && et<=1944.5){//(jet_iter->pt())<=1944.5){
    jecUnc->setJetEta(jet_iter->eta());
    jecUnc->setJetPt(et); // here you must use the CORRECTED jet pt
    unc = jecUnc->getUncertainty(true);}
    T_Jet_Uncertainty[idx]->push_back(unc);

    bool jetIdLoose_ = false;

    if (jet_iter->isPFJet()) {
      T_Jet_CharHadEnergyFrac[idx]->push_back(jet_iter->chargedHadronEnergyFraction());
      T_Jet_NeutHadEnergyFrac[idx]->push_back(jet_iter->neutralHadronEnergyFraction()); 
      T_Jet_CharEmEnergyFrac[idx]->push_back(jet_iter->chargedEmEnergyFraction()); 
      T_Jet_NeutEmEnergyFrac[idx]->push_back(jet_iter->neutralEmEnergyFraction());
      T_Jet_CharHadEnergy[idx]->push_back(jet_iter->chargedHadronEnergy());
      T_Jet_NeutHadEnergy[idx]->push_back(jet_iter->neutralHadronEnergy()); 
      T_Jet_CharEmEnergy[idx]->push_back(jet_iter->chargedEmEnergy()); 
      T_Jet_NeutEmEnergy[idx]->push_back(jet_iter->neutralEmEnergy());
      T_Jet_MuonMultiplicity[idx]->push_back(jet_iter->muonMultiplicity());
      T_Jet_NeutralMultiplicity[idx]->push_back(jet_iter->neutralMultiplicity());
      T_Jet_ChargedMultiplicity[idx]->push_back(jet_iter->chargedMultiplicity());
      jetIdLoose_ = PFjetIDLoose(*jet_iter);

      T_Jet_IDLoose[idx]->push_back(jetIdLoose_); 
      T_Jet_nDaughters[idx]->push_back(jet_iter->numberOfDaughters());
    }
    


    //
    //std::cout<<"partonFlavour"<<endl;
    T_Jet_Parton_Flavour[idx]->push_back(jet_iter->partonFlavour());
    // std::cout<<"genParton"<<endl;
    if (jet_iter->genParton()) {
      T_Jet_Parton_Px[idx]->push_back(jet_iter->genParton()->px());
      T_Jet_Parton_Py[idx]->push_back(jet_iter->genParton()->py());
      T_Jet_Parton_Pz[idx]->push_back(jet_iter->genParton()->pz());
      T_Jet_Parton_Energy[idx]->push_back(jet_iter->genParton()->energy());
    }
    else {
      T_Jet_Parton_Px[idx]->push_back(0);
      T_Jet_Parton_Py[idx]->push_back(0);
      T_Jet_Parton_Pz[idx]->push_back(0);
      T_Jet_Parton_Energy[idx]->push_back(0);
    }
   
    if(!IsRealData){
//         std::cout<<"GenJet"<<endl;
  try{    const reco::GenJet * mygenJet=jet_iter->genJet();
//      std::cout<<"No way"<<endl;
    
      if (mygenJet != 0) {
	T_Jet_IsGenJet[idx]->push_back(true);
	T_Jet_GenJet_Px[idx]->push_back(mygenJet->px());
	T_Jet_GenJet_Py[idx]->push_back(mygenJet->py());
	T_Jet_GenJet_Pz[idx]->push_back(mygenJet->pz());
	T_Jet_GenJet_Et[idx]->push_back(mygenJet->et());
	T_Jet_GenJet_Eta[idx]->push_back(mygenJet->eta());
	T_Jet_GenJet_Energy[idx]->push_back(mygenJet->energy());
	T_Jet_GenJet_InvisibleE[idx]->push_back(mygenJet->invisibleEnergy());
      }
      else {
	T_Jet_IsGenJet[idx]->push_back(false);
	T_Jet_GenJet_Px[idx]->push_back(0);
	T_Jet_GenJet_Py[idx]->push_back(0);
	T_Jet_GenJet_Pz[idx]->push_back(0);
	T_Jet_GenJet_Et[idx]->push_back(0);
	T_Jet_GenJet_Eta[idx]->push_back(0);
	T_Jet_GenJet_Energy[idx]->push_back(0);
	T_Jet_GenJet_InvisibleE[idx]->push_back(0);
      }
    } catch(...) {;}
    }
   
    
    T_Jet_Tag_HighEffTC[idx]->push_back(jet_iter->bDiscriminator("trackCountingHighEffBJetTags"));
    T_Jet_Tag_CombInclusiveSVtxV2[idx]->push_back(jet_iter->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags"));
    T_Jet_Tag_CombSVtx[idx]->push_back(jet_iter->bDiscriminator("combinedSecondaryVertexBJetTags"));//CSA14 only
    T_Jet_Tag_CombMVA[idx]->push_back(jet_iter->bDiscriminator("combinedMVABJetTags"));
    T_Jet_Tag_pfCombinedSVtx[idx]->push_back(jet_iter->bDiscriminator("pfCombinedSecondaryVertexBJetTags"));
    T_Jet_Tag_JetBProb[idx]->push_back(jet_iter->bDiscriminator("jetBProbabilityBJetTags"));
    T_Jet_Tag_JetProb[idx]->push_back(jet_iter->bDiscriminator("jetProbabilityBJetTags"));
    T_Jet_Tag_HighEffSimpSVtx[idx]->push_back(jet_iter->bDiscriminator("simpleSecondaryVertexHighEffBJetTags"));
    T_Jet_Tag_HighPurSimpSVtx[idx]->push_back(jet_iter->bDiscriminator("simpleSecondaryVertexHighPurBJetTags"));
    T_Jet_Tag_HighPurTC[idx]->push_back(jet_iter->bDiscriminator("trackCountingHighPurBJetTags"));
  }

   delete JetCorrector;
     delete L3JetPar;
     delete L2JetPar;
     delete L1JetPar;
  /*   if(IsRealData) delete ResJetPar;*/
}

void SUSYSkimToTreeTFS::SetJetBranchAddress(int idx, TString namecol, bool calojet) {
  
  Tree->Branch(TString(namecol + "_Px") , "std::vector<float>", &T_Jet_Px[idx]);
  Tree->Branch(TString(namecol + "_Py"), "std::vector<float>", &T_Jet_Py[idx]);
  Tree->Branch(TString(namecol + "_Pz"), "std::vector<float>", &T_Jet_Pz[idx]);
  Tree->Branch(TString(namecol + "_Et"), "std::vector<float>", &T_Jet_Et[idx]);
  Tree->Branch(TString(namecol + "_Eta"), "std::vector<float>", &T_Jet_Eta[idx]);
  Tree->Branch(TString(namecol + "_Energy"), "std::vector<float>", &T_Jet_Energy[idx]);
  Tree->Branch(TString(namecol + "_Corr"), "std::vector<float>", &T_Jet_Corr[idx]);
  Tree->Branch(TString(namecol + "_Tag_HighEffTC"), "std::vector<float>", &T_Jet_Tag_HighEffTC[idx]);
  Tree->Branch(TString(namecol + "_Tag_CombInclusiveSVtxV2"), "std::vector<float>", &T_Jet_Tag_CombInclusiveSVtxV2[idx]);
  Tree->Branch(TString(namecol + "_Tag_CombMVA"), "std::vector<float>", &T_Jet_Tag_CombMVA[idx]);
  Tree->Branch(TString(namecol + "_Tag_CombSVtx"), "std::vector<float>", &T_Jet_Tag_CombSVtx[idx]);
  Tree->Branch(TString(namecol + "_Tag_pfCombinedSVtx"), "std::vector<float>", &T_Jet_Tag_pfCombinedSVtx[idx]);
  Tree->Branch(TString(namecol + "_Tag_JetBProb"), "std::vector<float>", &T_Jet_Tag_JetBProb[idx]);
  Tree->Branch(TString(namecol + "_Tag_JetProb"), "std::vector<float>", &T_Jet_Tag_JetProb[idx]);
  Tree->Branch(TString(namecol + "_Tag_HighEffSimpSVtx"), "std::vector<float>", &T_Jet_Tag_HighEffSimpSVtx[idx]);
  Tree->Branch(TString(namecol + "_Tag_HighPurSimpSVtx"), "std::vector<float>", &T_Jet_Tag_HighPurSimpSVtx[idx]);
  Tree->Branch(TString(namecol + "_Tag_HighPurTC"), "std::vector<float>", &T_Jet_Tag_HighPurTC[idx]);
  Tree->Branch(TString(namecol + "_Parton_Px"), "std::vector<float>", &T_Jet_Parton_Px[idx]);
  Tree->Branch(TString(namecol + "_Parton_Py"), "std::vector<float>", &T_Jet_Parton_Py[idx]);
  Tree->Branch(TString(namecol + "_Parton_Pz"), "std::vector<float>", &T_Jet_Parton_Pz[idx]);
  Tree->Branch(TString(namecol + "_Parton_Energy"), "std::vector<float>", &T_Jet_Parton_Energy[idx]);
  Tree->Branch(TString(namecol + "_Parton_Flavour"), "std::vector<int>", &T_Jet_Parton_Flavour[idx]);
  
  Tree->Branch(TString(namecol + "_CharHadEnergyFrac"), "std::vector<float>", &T_Jet_CharHadEnergyFrac[idx]);
  Tree->Branch(TString(namecol + "_NeutHadEnergyFrac"), "std::vector<float>", &T_Jet_NeutHadEnergyFrac[idx]);
  Tree->Branch(TString(namecol + "_CharEmEnergyFrac"), "std::vector<float>", &T_Jet_CharEmEnergyFrac[idx]);
  Tree->Branch(TString(namecol + "_NeutEmEnergyFrac"), "std::vector<float>", &T_Jet_NeutEmEnergyFrac[idx]);
  Tree->Branch(TString(namecol + "_CharHadEnergy"), "std::vector<float>", &T_Jet_CharHadEnergy[idx]);
  Tree->Branch(TString(namecol + "_NeutHadEnergy"), "std::vector<float>", &T_Jet_NeutHadEnergy[idx]);
  Tree->Branch(TString(namecol + "_CharEmEnergy"), "std::vector<float>", &T_Jet_CharEmEnergy[idx]);
  Tree->Branch(TString(namecol + "_NeutEmEnergy"), "std::vector<float>", &T_Jet_NeutEmEnergy[idx]);
  Tree->Branch(TString(namecol + "_MuonMultiplicity"), "std::vector<int>", &T_Jet_MuonMultiplicity[idx]);
  Tree->Branch(TString(namecol + "_NeutralMultiplicity"), "std::vector<int>", &T_Jet_NeutralMultiplicity[idx]);
  Tree->Branch(TString(namecol + "_ChargedMultiplicity"), "std::vector<int>", &T_Jet_ChargedMultiplicity[idx]);
  Tree->Branch(TString(namecol + "_IDLoose"), "std::vector<bool>", &T_Jet_IDLoose[idx]);
  Tree->Branch(TString(namecol + "_nDaughters"), "std::vector<int>", &T_Jet_nDaughters[idx]);
 
 
  //   if(!IsRealData){
  Tree->Branch(TString(namecol + "_GenJet_InvisibleE"), "std::vector<float>", &T_Jet_GenJet_InvisibleE[idx]);
  Tree->Branch(TString(namecol + "_GenJet_Px"), "std::vector<float>", &T_Jet_GenJet_Px[idx]);
  Tree->Branch(TString(namecol + "_GenJet_Py"), "std::vector<float>", &T_Jet_GenJet_Py[idx]);
  Tree->Branch(TString(namecol + "_GenJet_Pz"), "std::vector<float>", &T_Jet_GenJet_Pz[idx]);
  Tree->Branch(TString(namecol + "_GenJet_Et"), "std::vector<float>", &T_Jet_GenJet_Et[idx]);
  Tree->Branch(TString(namecol + "_GenJet_Eta"), "std::vector<float>", &T_Jet_GenJet_Eta[idx]);
  Tree->Branch(TString(namecol + "_GenJet_Energy"), "std::vector<float>", &T_Jet_GenJet_Energy[idx]);
  Tree->Branch(TString(namecol + "_IsGenJet"), "std::vector<bool>", &T_Jet_IsGenJet[idx]);
  //  }
  
  
}


//-------

//It checks if there is a muon or electron in the tau decay chain
//if that is the case it stores the info of the first lepton found in the decay chain
void SUSYSkimToTreeTFS::LeptonicTauDecay(const reco::Candidate& tau, bool& elecdec, bool& muondec,
				      int &pdgId, float &px, float &py, float &pz, float &energy)
  
{
  
  // loop on tau decays, check for an electron or muon
  for(reco::Candidate::const_iterator daughter=tau.begin();daughter!=tau.end(); ++daughter){
    // if the tau daughter is a tau, it means the particle has still to be propagated.
    // In that case, return the result of the same method on that daughter.
    if(daughter->pdgId()==tau.pdgId()) return LeptonicTauDecay(*daughter, elecdec, muondec, pdgId, px, py, pz, energy);
    // check for leptons
    elecdec |= abs(daughter->pdgId()) == 11;
    muondec |= abs(daughter->pdgId()) == 13;
    
    if (abs(daughter->pdgId()) == 11 || abs(daughter->pdgId()) == 13) {
      px = daughter->px();
      py = daughter->py();
      pz = daughter->pz();
      energy = daughter->energy();
      pdgId = daughter->pdgId();
      return;
    }
  }
  
}

//////


//------



// ------------ method called once each job just before starting event loop  ------------
void 
SUSYSkimToTreeTFS::beginJob()
{
  edm::Service<TFileService> fs;
 // theHistosFile = new TFile(theHistosFileName.c_str(), "RECREATE");
 // theHistosFile->cd();
  
  Tree = fs->make<TTree>("Tree","Tree",0);
  //Events
  Tree->Branch("T_Event_Rho", &T_Event_Rho, "T_Event_Rho/F");
 Tree->Branch("T_EventF_HBHENoiseFilter", &T_EventF_HBHENoiseFilter, "T_EventF_HBHENoiseFilter/O");
 Tree->Branch("T_EventF_CSCTightHaloFilter", &T_EventF_CSCTightHaloFilter,"T_EventF_CSCTightHaloFilter/O");
 Tree->Branch("T_EventF_hcalLaserEventFilter", &T_EventF_hcalLaserEventFilter,"T_EventF_hcalLaserEventFilter/O");
 Tree->Branch("T_EventF_EcalDeadCellTriggerPrimitiveFilter", &T_EventF_EcalDeadCellTriggerPrimitiveFilter, "T_EventF_EcalDeadCellTriggerPrimitiveFilter/O");
 Tree->Branch("T_EventF_goodVertices", &T_EventF_goodVertices, "T_EventF_goodVertices/O");
 Tree->Branch("T_EventF_trackingFailureFilter", &T_EventF_trackingFailureFilter, "T_EventF_trackingFailureFilter/O");
 Tree->Branch("T_EventF_eeBadScFilter", &T_EventF_eeBadScFilter, "T_EventF_eeBadScFilter/O");
 Tree->Branch("T_EventF_ecalLaserCorrFilter", &T_EventF_ecalLaserCorrFilter, "T_EventF_ecalLaserCorrFilter/O");
 Tree->Branch("T_EventF_trkPOGFilters", &T_EventF_trkPOGFilters, "T_EventF_trkPOGFilters/O");
 Tree->Branch("T_EventF_trkPOG_manystripclus53X", &T_EventF_trkPOG_manystripclus53X, "T_EventF_trkPOG_manystripclus53X/O");
 Tree->Branch("T_EventF_trkPOG_toomanystripclus53X", &T_EventF_trkPOG_toomanystripclus53X, "T_EventF_trkPOG_toomanystripclus53X/O");
 Tree->Branch("T_EventF_trkPOG_logErrorTooManyClusters", &T_EventF_trkPOG_logErrorTooManyClusters, "T_EventF_trkPOG_logErrorTooManyClusters/O");
 Tree->Branch("T_EventF_METFilters", &T_EventF_METFilters, "T_EventF_METFilters/O");

  Tree->Branch("T_Event_RunNumber", &T_Event_RunNumber, "T_Event_RunNumber/I");
  Tree->Branch("T_Event_EventNumber", &T_Event_EventNumber, "T_Event_EventNumber/I");
  Tree->Branch("T_Event_LuminosityBlock", &T_Event_LuminosityBlock, "T_Event_LuminosityBlock/I");
  Tree->Branch("T_Event_processID", &T_Event_processID, "T_Event_processID/I");
  //  }
  
  Tree->Branch("T_Event_nPU", &T_Event_nPU, "T_Event_nPU/I");
  Tree->Branch("T_Event_nTruePU", &T_Event_nTruePU, "T_Event_nTruePU/F");
  Tree->Branch("T_Event_nPUm", &T_Event_nPUm, "T_Event_nPUm/I");
  Tree->Branch("T_Event_nPUp", &T_Event_nPUp, "T_Event_nPUp/I");
  Tree->Branch("T_Event_AveNTruePU", &T_Event_AveNTruePU, "T_Event_AveNTruePU/F"); 

  //HLT
  //Tree->Branch("T_HLT_Mu_vx", &T_HLT_Mu_vx, "T_HLT_Mu_vx/O");

  //  if(!IsRealData){
  //Gen 

  Tree->Branch("T_Gen_StopMass", "std::vector<float>", &T_Gen_StopMass);
  Tree->Branch("T_Gen_Chi0Mass", "std::vector<float>", &T_Gen_Chi0Mass);
  Tree->Branch("T_Gen_CharginoMass", "std::vector<float>", &T_Gen_CharginoMass);

  Tree->Branch("T_Gen_PromptMuon_pdgId", "std::vector<int>", &T_Gen_PromptMuon_pdgId);
  Tree->Branch("T_Gen_PromptMuon_Px", "std::vector<float>", &T_Gen_PromptMuon_Px);
  Tree->Branch("T_Gen_PromptMuon_Py", "std::vector<float>", &T_Gen_PromptMuon_Py);
  Tree->Branch("T_Gen_PromptMuon_Pz", "std::vector<float>", &T_Gen_PromptMuon_Pz);
  Tree->Branch("T_Gen_PromptMuon_Energy", "std::vector<float>", &T_Gen_PromptMuon_Energy);
  Tree->Branch("T_Gen_PromptMuon_MpdgId", "std::vector<int>", &T_Gen_PromptMuon_MpdgId);
  Tree->Branch("T_Gen_PromptMuon_MPx", "std::vector<float>", &T_Gen_PromptMuon_MPx);
  Tree->Branch("T_Gen_PromptMuon_MPy", "std::vector<float>", &T_Gen_PromptMuon_MPy);
  Tree->Branch("T_Gen_PromptMuon_MPz", "std::vector<float>", &T_Gen_PromptMuon_MPz);
  Tree->Branch("T_Gen_PromptMuon_MEnergy", "std::vector<float>", &T_Gen_PromptMuon_MEnergy);
  Tree->Branch("T_Gen_PromptMuon_MSt", "std::vector<int>", &T_Gen_PromptMuon_MSt);

  Tree->Branch("T_Gen_PromptElec_pdgId", "std::vector<int>", &T_Gen_PromptElec_pdgId);
  Tree->Branch("T_Gen_PromptElec_Px", "std::vector<float>", &T_Gen_PromptElec_Px);
  Tree->Branch("T_Gen_PromptElec_Py", "std::vector<float>", &T_Gen_PromptElec_Py);
  Tree->Branch("T_Gen_PromptElec_Pz", "std::vector<float>", &T_Gen_PromptElec_Pz);
  Tree->Branch("T_Gen_PromptElec_Energy", "std::vector<float>", &T_Gen_PromptElec_Energy);
  Tree->Branch("T_Gen_PromptElec_MpdgId", "std::vector<int>", &T_Gen_PromptElec_MpdgId);
  Tree->Branch("T_Gen_PromptElec_MPx", "std::vector<float>", &T_Gen_PromptElec_MPx);
  Tree->Branch("T_Gen_PromptElec_MPy", "std::vector<float>", &T_Gen_PromptElec_MPy);
  Tree->Branch("T_Gen_PromptElec_MPz", "std::vector<float>", &T_Gen_PromptElec_MPz);
  Tree->Branch("T_Gen_PromptElec_MEnergy", "std::vector<float>", &T_Gen_PromptElec_MEnergy);
  Tree->Branch("T_Gen_PromptElec_MSt", "std::vector<int>", &T_Gen_PromptElec_MSt);

  Tree->Branch("T_Gen_Promptb_pdgId", "std::vector<int>", &T_Gen_Promptb_pdgId);
  Tree->Branch("T_Gen_Promptb_Px", "std::vector<float>", &T_Gen_Promptb_Px);
  Tree->Branch("T_Gen_Promptb_Py", "std::vector<float>", &T_Gen_Promptb_Py);
  Tree->Branch("T_Gen_Promptb_Pz", "std::vector<float>", &T_Gen_Promptb_Pz);
  Tree->Branch("T_Gen_Promptb_Energy", "std::vector<float>", &T_Gen_Promptb_Energy);
  Tree->Branch("T_Gen_Promptb_MpdgId", "std::vector<int>", &T_Gen_Promptb_MpdgId);
  Tree->Branch("T_Gen_Promptb_MPx", "std::vector<float>", &T_Gen_Promptb_MPx);
  Tree->Branch("T_Gen_Promptb_MPy", "std::vector<float>", &T_Gen_Promptb_MPy);
  Tree->Branch("T_Gen_Promptb_MPz", "std::vector<float>", &T_Gen_Promptb_MPz);
  Tree->Branch("T_Gen_Promptb_MEnergy", "std::vector<float>", &T_Gen_Promptb_MEnergy);
  Tree->Branch("T_Gen_Promptb_MSt", "std::vector<int>", &T_Gen_Promptb_MSt);

  Tree->Branch("T_Gen_Muon_pdgId", "std::vector<int>", &T_Gen_Muon_pdgId);
  Tree->Branch("T_Gen_Muon_Px", "std::vector<float>", &T_Gen_Muon_Px);
  Tree->Branch("T_Gen_Muon_Py", "std::vector<float>", &T_Gen_Muon_Py);
  Tree->Branch("T_Gen_Muon_Pz", "std::vector<float>", &T_Gen_Muon_Pz);
  Tree->Branch("T_Gen_Muon_Energy", "std::vector<float>", &T_Gen_Muon_Energy);
  Tree->Branch("T_Gen_Muon_MpdgId", "std::vector<int>", &T_Gen_Muon_MpdgId);
  Tree->Branch("T_Gen_Muon_MPx", "std::vector<float>", &T_Gen_Muon_MPx);
  Tree->Branch("T_Gen_Muon_MPy", "std::vector<float>", &T_Gen_Muon_MPy);
  Tree->Branch("T_Gen_Muon_MPz", "std::vector<float>", &T_Gen_Muon_MPz);
  Tree->Branch("T_Gen_Muon_MEnergy", "std::vector<float>", &T_Gen_Muon_MEnergy);
  Tree->Branch("T_Gen_Muon_MSt", "std::vector<int>", &T_Gen_Muon_MSt);

  Tree->Branch("T_Gen_FinalMuon_pdgId", "std::vector<int>", &T_Gen_FinalMuon_pdgId);
  Tree->Branch("T_Gen_FinalMuon_Px", "std::vector<float>", &T_Gen_FinalMuon_Px);
  Tree->Branch("T_Gen_FinalMuon_Py", "std::vector<float>", &T_Gen_FinalMuon_Py);
  Tree->Branch("T_Gen_FinalMuon_Pz", "std::vector<float>", &T_Gen_FinalMuon_Pz);
  Tree->Branch("T_Gen_FinalMuon_Energy", "std::vector<float>", &T_Gen_FinalMuon_Energy);

  Tree->Branch("T_Gen_Elec_pdgId", "std::vector<int>", &T_Gen_Elec_pdgId);
  Tree->Branch("T_Gen_Elec_Px", "std::vector<float>", &T_Gen_Elec_Px);
  Tree->Branch("T_Gen_Elec_Py", "std::vector<float>", &T_Gen_Elec_Py);
  Tree->Branch("T_Gen_Elec_Pz", "std::vector<float>", &T_Gen_Elec_Pz);
  Tree->Branch("T_Gen_Elec_Energy", "std::vector<float>", &T_Gen_Elec_Energy);
  Tree->Branch("T_Gen_Elec_MpdgId", "std::vector<int>", &T_Gen_Elec_MpdgId);
  Tree->Branch("T_Gen_Elec_MPx", "std::vector<float>", &T_Gen_Elec_MPx);
  Tree->Branch("T_Gen_Elec_MPy", "std::vector<float>", &T_Gen_Elec_MPy);
  Tree->Branch("T_Gen_Elec_MPz", "std::vector<float>", &T_Gen_Elec_MPz);
  Tree->Branch("T_Gen_Elec_MEnergy", "std::vector<float>", &T_Gen_Elec_MEnergy);
  Tree->Branch("T_Gen_Elec_MSt", "std::vector<int>", &T_Gen_Elec_MSt);

  Tree->Branch("T_Gen_FinalElec_pdgId", "std::vector<int>", &T_Gen_FinalElec_pdgId);
  Tree->Branch("T_Gen_FinalElec_Px", "std::vector<float>", &T_Gen_FinalElec_Px);
  Tree->Branch("T_Gen_FinalElec_Py", "std::vector<float>", &T_Gen_FinalElec_Py);
  Tree->Branch("T_Gen_FinalElec_Pz", "std::vector<float>", &T_Gen_FinalElec_Pz);
  Tree->Branch("T_Gen_FinalElec_Energy", "std::vector<float>", &T_Gen_FinalElec_Energy);

  Tree->Branch("T_Gen_b_pdgId", "std::vector<int>", &T_Gen_b_pdgId);
  Tree->Branch("T_Gen_b_Px", "std::vector<float>", &T_Gen_b_Px);
  Tree->Branch("T_Gen_b_Py", "std::vector<float>", &T_Gen_b_Py);
  Tree->Branch("T_Gen_b_Pz", "std::vector<float>", &T_Gen_b_Pz);
  Tree->Branch("T_Gen_b_Energy", "std::vector<float>", &T_Gen_b_Energy);
  Tree->Branch("T_Gen_b_MpdgId", "std::vector<int>", &T_Gen_b_MpdgId);
  Tree->Branch("T_Gen_b_MPx", "std::vector<float>", &T_Gen_b_MPx);
  Tree->Branch("T_Gen_b_MPy", "std::vector<float>", &T_Gen_b_MPy);
  Tree->Branch("T_Gen_b_MPz", "std::vector<float>", &T_Gen_b_MPz);
  Tree->Branch("T_Gen_b_MEnergy", "std::vector<float>", &T_Gen_b_MEnergy);
  Tree->Branch("T_Gen_b_MSt", "std::vector<int>", &T_Gen_b_MSt);

  Tree->Branch("T_Gen_Stop_pdgId", "std::vector<int>",         &T_Gen_Stop_pdgId       );
  Tree->Branch("T_Gen_Stop_MpdgId", "std::vector<int>",   &T_Gen_Stop_MpdgId );
  Tree->Branch("T_Gen_Stop_energy", "std::vector<float>",      &T_Gen_Stop_energy      );
  Tree->Branch("T_Gen_Stop_pt", "std::vector<float>",          &T_Gen_Stop_pt          );
  Tree->Branch("T_Gen_Stop_eta", "std::vector<float>",         &T_Gen_Stop_eta         );
  Tree->Branch("T_Gen_Stop_phi", "std::vector<float>",         &T_Gen_Stop_phi         );
  
  Tree->Branch("T_Gen_Chi0_pdgId", "std::vector<int>",         &T_Gen_Chi0_pdgId       );
  Tree->Branch("T_Gen_Chi0_MpdgId", "std::vector<int>",   &T_Gen_Chi0_MpdgId );
  Tree->Branch("T_Gen_Chi0_energy", "std::vector<float>",      &T_Gen_Chi0_energy      );
  Tree->Branch("T_Gen_Chi0_pt", "std::vector<float>",          &T_Gen_Chi0_pt          );
  Tree->Branch("T_Gen_Chi0_eta", "std::vector<float>",         &T_Gen_Chi0_eta         );
  Tree->Branch("T_Gen_Chi0_phi", "std::vector<float>",         &T_Gen_Chi0_phi         );
  
  Tree->Branch("T_Gen_ChiPM_pdgId", "std::vector<int>",         &T_Gen_ChiPM_pdgId       );
  Tree->Branch("T_Gen_ChiPM_MpdgId", "std::vector<int>",   &T_Gen_ChiPM_MpdgId );
  Tree->Branch("T_Gen_ChiPM_energy", "std::vector<float>",      &T_Gen_ChiPM_energy      );
  Tree->Branch("T_Gen_ChiPM_pt", "std::vector<float>",          &T_Gen_ChiPM_pt          );
  Tree->Branch("T_Gen_ChiPM_eta", "std::vector<float>",         &T_Gen_ChiPM_eta         );
  Tree->Branch("T_Gen_ChiPM_phi", "std::vector<float>",         &T_Gen_ChiPM_phi         );

  Tree->Branch("T_Gen_t_pdgId", "std::vector<int>",         &T_Gen_t_pdgId       );
  Tree->Branch("T_Gen_t_MpdgId", "std::vector<int>",   &T_Gen_t_MpdgId );
  Tree->Branch("T_Gen_t_energy", "std::vector<float>",      &T_Gen_t_energy      );
  Tree->Branch("T_Gen_t_pt", "std::vector<float>",          &T_Gen_t_pt          );
  Tree->Branch("T_Gen_t_eta", "std::vector<float>",         &T_Gen_t_eta         );
  Tree->Branch("T_Gen_t_phi", "std::vector<float>",         &T_Gen_t_phi         );

  Tree->Branch("T_Gen_Nu_pdgId", "std::vector<int>",         &T_Gen_Nu_pdgId       );
  Tree->Branch("T_Gen_Nu_MpdgId", "std::vector<int>",   &T_Gen_Nu_MpdgId );
  Tree->Branch("T_Gen_Nu_energy", "std::vector<float>",      &T_Gen_Nu_energy      );
  Tree->Branch("T_Gen_Nu_pt", "std::vector<float>",          &T_Gen_Nu_pt          );
  Tree->Branch("T_Gen_Nu_eta", "std::vector<float>",         &T_Gen_Nu_eta         );
  Tree->Branch("T_Gen_Nu_phi", "std::vector<float>",         &T_Gen_Nu_phi         );

  Tree->Branch("T_Gen_Z_pdgId", "std::vector<int>",         &T_Gen_Z_pdgId       );
  Tree->Branch("T_Gen_Z_MpdgId", "std::vector<int>",   &T_Gen_Z_MpdgId );
  Tree->Branch("T_Gen_Z_energy", "std::vector<float>",      &T_Gen_Z_energy      );
  Tree->Branch("T_Gen_Z_pt", "std::vector<float>",          &T_Gen_Z_pt          );
  Tree->Branch("T_Gen_Z_eta", "std::vector<float>",         &T_Gen_Z_eta         );
  Tree->Branch("T_Gen_Z_phi", "std::vector<float>",         &T_Gen_Z_phi         );

  Tree->Branch("T_Gen_W_pdgId", "std::vector<int>",         &T_Gen_W_pdgId       );
  Tree->Branch("T_Gen_W_MpdgId", "std::vector<int>",   &T_Gen_W_MpdgId );
  Tree->Branch("T_Gen_W_energy", "std::vector<float>",      &T_Gen_W_energy      );
  Tree->Branch("T_Gen_W_pt", "std::vector<float>",          &T_Gen_W_pt          );
  Tree->Branch("T_Gen_W_eta", "std::vector<float>",         &T_Gen_W_eta         );
  Tree->Branch("T_Gen_W_phi", "std::vector<float>",         &T_Gen_W_phi         );

  Tree->Branch("T_Gen_PromptTau_pdgId", "std::vector<int>",          &T_Gen_PromptTau_pdgId       );
  Tree->Branch("T_Gen_PromptTau_Energy", "std::vector<float>",       &T_Gen_PromptTau_Energy      );
  Tree->Branch("T_Gen_PromptTau_Px", "std::vector<float>",           &T_Gen_PromptTau_Px          );
  Tree->Branch("T_Gen_PromptTau_Py", "std::vector<float>",          &T_Gen_PromptTau_Py         );
  Tree->Branch("T_Gen_PromptTau_Pz", "std::vector<float>",          &T_Gen_PromptTau_Pz         );

  Tree->Branch("T_Gen_PromptTau_MpdgId", "std::vector<int>",    &T_Gen_PromptTau_MpdgId );
  Tree->Branch("T_Gen_PromptTau_MEnergy", "std::vector<float>",       &T_Gen_PromptTau_MEnergy      );
  Tree->Branch("T_Gen_PromptTau_MPx", "std::vector<float>",           &T_Gen_PromptTau_MPx          );
  Tree->Branch("T_Gen_PromptTau_MPy", "std::vector<float>",          &T_Gen_PromptTau_MPy         );
  Tree->Branch("T_Gen_PromptTau_MPz", "std::vector<float>",          &T_Gen_PromptTau_MPz         );
  Tree->Branch("T_Gen_PromptTau_MSt", "std::vector<int>",    &T_Gen_PromptTau_MSt );

  Tree->Branch("T_Gen_PromptTau_IsLepDec", "std::vector<bool>", &T_Gen_PromptTau_IsLepDec);
  Tree->Branch("T_Gen_PromptTau_LepDec_pdgId", "std::vector<int>", &T_Gen_PromptTau_LepDec_pdgId);
  Tree->Branch("T_Gen_PromptTau_LepDec_Px", "std::vector<float>", &T_Gen_PromptTau_LepDec_Px);
  Tree->Branch("T_Gen_PromptTau_LepDec_Py", "std::vector<float>", &T_Gen_PromptTau_LepDec_Py);
  Tree->Branch("T_Gen_PromptTau_LepDec_Pz", "std::vector<float>", &T_Gen_PromptTau_LepDec_Pz);
  Tree->Branch("T_Gen_PromptTau_LepDec_Energy", "std::vector<float>", &T_Gen_PromptTau_LepDec_Energy);
  
  Tree->Branch("T_Gen_Tau_pdgId", "std::vector<int>",          &T_Gen_Tau_pdgId       );
  Tree->Branch("T_Gen_Tau_Energy", "std::vector<float>",       &T_Gen_Tau_Energy      );
  Tree->Branch("T_Gen_Tau_Px", "std::vector<float>",           &T_Gen_Tau_Px          );
  Tree->Branch("T_Gen_Tau_Py", "std::vector<float>",          &T_Gen_Tau_Py         );
  Tree->Branch("T_Gen_Tau_Pz", "std::vector<float>",          &T_Gen_Tau_Pz         );

  Tree->Branch("T_Gen_Tau_MpdgId", "std::vector<int>",    &T_Gen_Tau_MpdgId );
  Tree->Branch("T_Gen_Tau_MEnergy", "std::vector<float>",       &T_Gen_Tau_MEnergy      );
  Tree->Branch("T_Gen_Tau_MPx", "std::vector<float>",           &T_Gen_Tau_MPx          );
  Tree->Branch("T_Gen_Tau_MPy", "std::vector<float>",          &T_Gen_Tau_MPy         );
  Tree->Branch("T_Gen_Tau_MPz", "std::vector<float>",          &T_Gen_Tau_MPz         );
  Tree->Branch("T_Gen_Tau_MSt", "std::vector<int>",    &T_Gen_Tau_MSt );

  Tree->Branch("T_Gen_Tau_IsLepDec", "std::vector<bool>", &T_Gen_Tau_IsLepDec);
  Tree->Branch("T_Gen_Tau_LepDec_pdgId", "std::vector<int>", &T_Gen_Tau_LepDec_pdgId);
  Tree->Branch("T_Gen_Tau_LepDec_Px", "std::vector<float>", &T_Gen_Tau_LepDec_Px);
  Tree->Branch("T_Gen_Tau_LepDec_Py", "std::vector<float>", &T_Gen_Tau_LepDec_Py);
  Tree->Branch("T_Gen_Tau_LepDec_Pz", "std::vector<float>", &T_Gen_Tau_LepDec_Pz);
  Tree->Branch("T_Gen_Tau_LepDec_Energy", "std::vector<float>", &T_Gen_Tau_LepDec_Energy);
  //}

   
  //Muons
  Tree->Branch("T_Muon_Eta", "std::vector<float>", &T_Muon_Eta);
  Tree->Branch("T_Muon_IsGlobalMuon", "std::vector<bool>", &T_Muon_IsGlobalMuon);
  Tree-> Branch("T_Muon_IsAllTrackerMuons", "std::vector<bool>", &T_Muon_IsAllTrackerMuons);
  Tree-> Branch("T_Muon_IsTrackerMuonArbitrated", "std::vector<bool>", &T_Muon_IsTrackerMuonArbitrated);
  Tree-> Branch("T_Muon_IsGMPTMuons", "std::vector<bool>", &T_Muon_IsGMPTMuons);
  Tree-> Branch("T_Muon_IsAllStandAloneMuons", "std::vector<bool>", &T_Muon_IsAllStandAloneMuons);
  Tree-> Branch("T_Muon_IsTMLastStationTight", "std::vector<bool>", &T_Muon_IsTMLastStationTight);
 
  Tree->Branch("T_Muon_SegmentCompatibility","std::vector<float>", &T_Muon_SegmentCompatibility);
  Tree->Branch("T_Muon_trkKink","std::vector<float>", &T_Muon_trkKink);
  Tree->Branch("T_Muon_Px", "std::vector<float>", &T_Muon_Px);
  Tree->Branch("T_Muon_Py", "std::vector<float>", &T_Muon_Py);
  Tree->Branch("T_Muon_Pz", "std::vector<float>", &T_Muon_Pz);
  Tree->Branch("T_Muon_Pt", "std::vector<float>", &T_Muon_Pt);
  Tree->Branch("T_Muon_deltaPt", "std::vector<float>", &T_Muon_deltaPt);
  Tree->Branch("T_Muon_Energy", "std::vector<float>", &T_Muon_Energy);
  Tree->Branch("T_Muon_Charge", "std::vector<int>", &T_Muon_Charge);
  Tree->Branch("T_Muon_NormChi2GTrk", "std::vector<float>", &T_Muon_NormChi2GTrk);
  Tree->Branch("T_Muon_NValidHitsInTrk", "std::vector<int>", &T_Muon_NValidHitsInTrk);
  Tree->Branch("T_Muon_NValidPixelHitsInTrk", "std::vector<int>", &T_Muon_NValidPixelHitsInTrk);
  Tree->Branch("T_Muon_InnerTrackFound", "std::vector<int>", &T_Muon_InnerTrackFound);
  Tree->Branch("T_Muon_NValidHitsSATrk", "std::vector<int>", &T_Muon_NValidHitsSATrk);
  Tree->Branch("T_Muon_NValidHitsGTrk", "std::vector<int>", &T_Muon_NValidHitsGTrk);
  Tree->Branch("T_Muon_Chi2InTrk", "std::vector<float>", &T_Muon_Chi2InTrk);
  Tree->Branch("T_Muon_dofInTrk", "std::vector<float>", &T_Muon_dofInTrk);
  Tree->Branch("T_Muon_dxyGTrack", "std::vector<float>", &T_Muon_dxyGTrack);
  Tree->Branch("T_Muon_dxyInTrack", "std::vector<float>", &T_Muon_dxyInTrack);
  Tree->Branch("T_Muon_dzGTrack", "std::vector<float>", &T_Muon_dzGTrack);
  Tree->Branch("T_Muon_dzInTrack", "std::vector<float>", &T_Muon_dzInTrack);
  Tree->Branch("T_Muon_IPwrtAveBSInTrack", "std::vector<float>", &T_Muon_IPwrtAveBSInTrack);

  Tree->Branch("T_Muon_chargedHadronIsoR04", "std::vector<float>", &T_Muon_chargedHadronIsoR04);
  Tree->Branch("T_Muon_neutralHadronIsoR04", "std::vector<float>", &T_Muon_neutralHadronIsoR04);
  Tree->Branch("T_Muon_neutralIsoPFweightR04", "std::vector<float>", &T_Muon_neutralIsoPFweightR04);
  Tree->Branch("T_Muon_photonIsoR04", "std::vector<float>", &T_Muon_photonIsoR04);
  Tree->Branch("T_Muon_chargedParticleIsoR03", "std::vector<float>", &T_Muon_chargedParticleIsoR03);
  Tree->Branch("T_Muon_chargedHadronIsoR03", "std::vector<float>", &T_Muon_chargedHadronIsoR03);
  Tree->Branch("T_Muon_neutralHadronIsoR03", "std::vector<float>", &T_Muon_neutralHadronIsoR03);
  Tree->Branch("T_Muon_photonIsoR03", "std::vector<float>", &T_Muon_photonIsoR03);
  Tree->Branch("T_Muon_sumPUPtR04", "std::vector<float>", &T_Muon_sumPUPtR04);
  Tree->Branch("T_Muon_sumPUPtR03", "std::vector<float>", &T_Muon_sumPUPtR03);
  Tree->Branch("T_Muon_vz", "std::vector<float>", &T_Muon_vz);
  Tree->Branch("T_Muon_vy", "std::vector<float>", &T_Muon_vy);
  Tree->Branch("T_Muon_vx", "std::vector<float>", &T_Muon_vx);
  Tree->Branch("T_Muon_NumOfMatchedStations", "std::vector<int>", &T_Muon_NumOfMatchedStations);
  Tree->Branch("T_Muon_PFMuonPt","std::vector<float>", &T_Muon_PFMuonPt);
  Tree->Branch("T_Muon_PFMuonPx","std::vector<float>", &T_Muon_PFMuonPx);
  Tree->Branch("T_Muon_PFMuonPy","std::vector<float>", &T_Muon_PFMuonPy);
  Tree->Branch("T_Muon_PFMuonPz","std::vector<float>", &T_Muon_PFMuonPx);
  Tree->Branch("T_Muon_PFMuonE","std::vector<float>", &T_Muon_PFMuonE);
  Tree->Branch("T_Muon_isPFMuon","std::vector<bool>", &T_Muon_isPFMuon);
  Tree->Branch("T_Muon_NLayers","std::vector<int>", &T_Muon_NLayers);
  Tree->Branch("T_Muon_BestTrack_dxy", "std::vector<float>", &T_Muon_BestTrack_dxy);
  Tree->Branch("T_Muon_BestTrack_dz",  "std::vector<float>", &T_Muon_BestTrack_dz);
  Tree->Branch("T_Muon_BestTrack_vx",  "std::vector<float>", &T_Muon_BestTrack_vx);
  Tree->Branch("T_Muon_BestTrack_vy",  "std::vector<float>", &T_Muon_BestTrack_vy);
  Tree->Branch("T_Muon_BestTrack_vz",  "std::vector<float>", &T_Muon_BestTrack_vz);
  Tree->Branch("T_Muon_BestTrack_Px",  "std::vector<float>", &T_Muon_BestTrack_Px);
  Tree->Branch("T_Muon_BestTrack_Py",  "std::vector<float>", &T_Muon_BestTrack_Py);
  Tree->Branch("T_Muon_BestTrack_Pz",  "std::vector<float>", &T_Muon_BestTrack_Pz);
  Tree->Branch("T_Muon_BestTrack_Pt",  "std::vector<float>", &T_Muon_BestTrack_Pt);
  Tree->Branch("T_Muon_BestTrack_Phi", "std::vector<float>", &T_Muon_BestTrack_Phi);

  //Taus
  /*  Tree->Branch("T_Tau_Px", "std::vector<float>", &T_Tau_Px);
      Tree->Branch("T_Tau_Py", "std::vector<float>", &T_Tau_Py);
      Tree->Branch("T_Tau_Pz", "std::vector<float>", &T_Tau_Pz);
      Tree->Branch("T_Tau_Energy", "std::vector<float>", &T_Tau_Energy);
      Tree->Branch("T_Tau_Charge", "std::vector<int>", &T_Tau_Charge);
  */  
  
  //Vertex
  Tree->Branch("T_Vertex_x",          "std::vector<float>", &T_Vertex_x);
  Tree->Branch("T_Vertex_y",          "std::vector<float>", &T_Vertex_y);
  Tree->Branch("T_Vertex_z",          "std::vector<float>", &T_Vertex_z);
  Tree->Branch("T_Vertex_Chi2Prob",   "std::vector<float>", &T_Vertex_Chi2Prob);
  Tree->Branch("T_Vertex_ndof",       "std::vector<float>", &T_Vertex_ndof);
  Tree->Branch("T_Vertex_rho",        "std::vector<float>", &T_Vertex_rho);
  Tree->Branch("T_Vertex_isFake",     "std::vector<bool>",  &T_Vertex_isFake);
  Tree->Branch("T_Vertex_tracksSize", "std::vector<int>",   &T_Vertex_tracksSize);
  Tree->Branch("T_Vertex_nTracks",    "std::vector<int>",   &T_Vertex_nTracks);

  //Electrons  
  Tree->Branch("T_Elec_Eta", "std::vector<float>", &T_Elec_Eta);
  Tree->Branch("T_Elec_IPwrtAveBS", "std::vector<float>", &T_Elec_IPwrtAveBS);
  Tree->Branch("T_Elec_IPwrtPV", "std::vector<float>", &T_Elec_IPwrtPV);
  Tree->Branch("T_Elec_dzwrtPV", "std::vector<float>", &T_Elec_dzwrtPV);
  Tree->Branch("T_Elec_Px", "std::vector<float>", &T_Elec_Px);
  Tree->Branch("T_Elec_Py", "std::vector<float>", &T_Elec_Py);
  Tree->Branch("T_Elec_Pz", "std::vector<float>", &T_Elec_Pz);
  Tree->Branch("T_Elec_Pt", "std::vector<float>", &T_Elec_Pt);
  Tree->Branch("T_Elec_Energy", "std::vector<float>", &T_Elec_Energy);
  Tree->Branch("T_Elec_Charge", "std::vector<int>", &T_Elec_Charge);

  Tree->Branch("T_Elec_nBrems", "std::vector<int>", &T_Elec_nBrems); 
  Tree->Branch("T_Elec_fBrem", "std::vector<float>", &T_Elec_fBrem);
  Tree->Branch("T_Elec_eSuperClusterOverP", "std::vector<float>", &T_Elec_eSuperClusterOverP);
  Tree->Branch("T_Elec_ecalEnergy", "std::vector<float>", &T_Elec_ecalEnergy);
  Tree->Branch("T_Elec_dr03TkSumPt", "std::vector<float>", &T_Elec_dr03TkSumPt);
  Tree->Branch("T_Elec_dr03EcalSumEt", "std::vector<float>", &T_Elec_dr03EcalSumEt);
  Tree->Branch("T_Elec_dr03HcalSumEt", "std::vector<float>", &T_Elec_dr03HcalSumEt);
  Tree->Branch("T_Elec_isEB", "std::vector<bool>", &T_Elec_isEB);
  Tree->Branch("T_Elec_isEE", "std::vector<bool>", &T_Elec_isEE);
  Tree->Branch("T_Elec_chargedHadronIso", "std::vector<float>", &T_Elec_chargedHadronIso);
  Tree->Branch("T_Elec_neutralHadronIso", "std::vector<float>", &T_Elec_neutralHadronIso);
  Tree->Branch("T_Elec_neutralIsoPFweight", "std::vector<float>", &T_Elec_neutralIsoPFweight);
  Tree->Branch("T_Elec_photonIso", "std::vector<float>", &T_Elec_photonIso);
  Tree->Branch("T_Elec_puChargedHadronIso", "std::vector<float>", &T_Elec_puChargedHadronIso);
  Tree->Branch("T_Elec_passConversionVeto","std::vector<bool>",&T_Elec_passConversionVeto);
    
  Tree->Branch("T_Elec_sigmaIetaIeta", "std::vector<float>", &T_Elec_sigmaIetaIeta);
  Tree->Branch("T_Elec_deltaPhiIn", "std::vector<float>", &T_Elec_deltaPhiIn);
  Tree->Branch("T_Elec_deltaEtaIn", "std::vector<float>", &T_Elec_deltaEtaIn);
  Tree->Branch("T_Elec_isEcalDriven", "std::vector<bool>", &T_Elec_isEcalDriven);
  Tree->Branch("T_Elec_HtoE", "std::vector<float>", &T_Elec_HtoE);
 
  Tree->Branch("T_Elec_vz", "std::vector<float>", &T_Elec_vz);
  Tree->Branch("T_Elec_vy", "std::vector<float>", &T_Elec_vy);
  Tree->Branch("T_Elec_vx", "std::vector<float>", &T_Elec_vx);
  Tree->Branch("T_Elec_nLost", "std::vector<int>", &T_Elec_nLost);
  Tree->Branch("T_Elec_nHits", "std::vector<int>", &T_Elec_nHits);
  Tree->Branch("T_Elec_SC_Et","std::vector<float>", &T_Elec_SC_Et);
  Tree->Branch("T_Elec_SC_Eta","std::vector<float>", &T_Elec_SC_Eta); 

  Tree->Branch("T_Elec_PFElecPt","std::vector<float>", &T_Elec_PFElecPt);
  Tree->Branch("T_Elec_PFElecPx","std::vector<float>", &T_Elec_PFElecPx);
  Tree->Branch("T_Elec_PFElecPy","std::vector<float>", &T_Elec_PFElecPy);
  Tree->Branch("T_Elec_PFElecPz","std::vector<float>", &T_Elec_PFElecPx);
  Tree->Branch("T_Elec_PFElecE","std::vector<float>", &T_Elec_PFElecE);
  Tree->Branch("T_Elec_isPF","std::vector<bool>", &T_Elec_isPF);
  Tree->Branch("T_Elec_MVAoutput","std::vector<float>", &T_Elec_MVAoutput);
  
  //Jets
  //  SetJetBranchAddress(0, "T_JetAK", true);
  SetJetBranchAddress(0, "T_JetAKCHS", true);
  //  SetJetBranchAddress(2, "T_JetAKPF2PAT", false);

  
  
  //MET 
  Tree->Branch("T_METPF_ET", &T_METPF_ET, "T_METPF_ET/F");
  Tree->Branch("T_METPF_Phi", &T_METPF_Phi, "T_METPF_Phi/F");	
  //  if(!IsRealData){  
  Tree->Branch("T_METgen_ET", &T_METgen_ET, "T_METgen_ET/F");
  Tree->Branch("T_METgen_Phi", &T_METgen_Phi, "T_METgen_Phi/F");             
  //  }

}


// ------------ method called once each job just after ending the event loop  ------------
void SUSYSkimToTreeTFS::endJob() {}


// Define this as a plug-in
DEFINE_FWK_MODULE(SUSYSkimToTreeTFS);

