//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Dec 16 11:10:58 2014 by ROOT version 5.34/23
// from TChain Tree/Tree
//////////////////////////////////////////////////////////

#ifndef MuonFakeRate_h
#define MuonFakeRate_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class MuonFakeRate {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         T_Event_Rho;
   Float_t         T_Event_RhoIso;
   Float_t         T_Event_RhoNoPu;
   Float_t         T_Event_RhoIsoNoPu;
   Float_t         T_Event_RhoCentralNeutral;
   Int_t           T_Event_RunNumber;
   Int_t           T_Event_EventNumber;
   Int_t           T_Event_LuminosityBlock;
   Int_t           T_Event_processID;
   Int_t           T_Event_nPU;
   Float_t         T_Event_nTruePU;
   Int_t           T_Event_nPUm;
   Int_t           T_Event_nPUp;
   Float_t         T_Event_AveNTruePU;
   Bool_t          T_HLT_Mu9;
   Bool_t          T_HLT_Mu8_v1;
   Bool_t          T_HLT_Mu8_v2;
   Bool_t          T_HLT_Mu8_v3;
   Bool_t          T_HLT_Mu8_v4;
   Bool_t          T_HLT_Mu8_v5;
   Bool_t          T_HLT_Mu8_v6;
   Bool_t          T_HLT_Mu8_v7;
   Bool_t          T_HLT_Mu8_v8;
   Bool_t          T_HLT_Mu8_v9;
   Bool_t          T_HLT_Mu8_v10;
   Bool_t          T_HLT_Mu8_v11;
   Bool_t          T_HLT_Mu8_v12;
   Bool_t          T_HLT_Mu8_v16;
   Bool_t          T_HLT_Mu8_v17;
   Bool_t          T_HLT_Mu8_v18;
   Bool_t          T_HLT_Mu12_v1;
   Bool_t          T_HLT_Mu12_v2;
   Bool_t          T_HLT_Mu12_v3;
   Bool_t          T_HLT_Mu12_v4;
   Bool_t          T_HLT_Mu12_v16;
   Bool_t          T_HLT_Mu15_v1;
   Bool_t          T_HLT_Mu15_v2;
   Bool_t          T_HLT_Mu15_v3;
   Bool_t          T_HLT_Mu15_v4;
   Bool_t          T_HLT_Mu15_v5;
   Bool_t          T_HLT_Mu15_v6;
   Bool_t          T_HLT_Mu15_v7;
   Bool_t          T_HLT_Mu15_v8;
   Bool_t          T_HLT_Mu15_v9;
   Bool_t          T_HLT_Mu15_v10;
   Bool_t          T_HLT_Mu15_v11;
   Bool_t          T_HLT_Mu15_v12;
   Bool_t          T_HLT_Mu15_v13;
   Bool_t          T_HLT_Mu17_v3;
   Bool_t          T_HLT_Mu17_v4;
   Bool_t          T_HLT_Mu17_v5;
   Bool_t          T_HLT_IsoMu24_eta2p1_v11;
   Bool_t          T_HLT_IsoMu24_eta2p1_v12;
   Bool_t          T_HLT_IsoMu24_eta2p1_v13;
   Bool_t          T_HLT_Mu24_v1;
   Bool_t          T_HLT_Mu24_v2;
   Bool_t          T_HLT_Mu24_v3;
   Bool_t          T_HLT_Mu24_v4;
   Bool_t          T_HLT_Mu24_v5;
   Bool_t          T_HLT_Mu24_v6;
   Bool_t          T_HLT_Mu24_v7;
   Bool_t          T_HLT_Mu24_v8;
   Bool_t          T_HLT_Mu24_v9;
   Bool_t          T_HLT_Mu24_v10;
   Bool_t          T_HLT_Mu24_v11;
   Bool_t          T_HLT_Mu24_v12;
   Bool_t          T_HLT_Mu24_v13;
   Bool_t          T_HLT_Jet30_v1;
   Bool_t          T_HLT_Jet30_v2;
   Bool_t          T_HLT_Jet30_v3;
   Bool_t          T_HLT_Jet30_v4;
   Bool_t          T_HLT_Jet30_v5;
   Bool_t          T_HLT_Jet60_v1;
   Bool_t          T_HLT_Jet60_v2;
   Bool_t          T_HLT_Jet60_v3;
   Bool_t          T_HLT_Jet60_v4;
   Bool_t          T_HLT_Jet60_v5;
   vector<int>     *T_Gen_Muon_PID;
   vector<float>   *T_Gen_Muon_Px;
   vector<float>   *T_Gen_Muon_Py;
   vector<float>   *T_Gen_Muon_Pz;
   vector<float>   *T_Gen_Muon_Energy;
   vector<int>     *T_Gen_Muon_MPID;
   vector<float>   *T_Gen_Muon_MPx;
   vector<float>   *T_Gen_Muon_MPy;
   vector<float>   *T_Gen_Muon_MPz;
   vector<float>   *T_Gen_Muon_MEnergy;
   vector<int>     *T_Gen_Muon_MSt;
   vector<int>     *T_Gen_Elec_PID;
   vector<float>   *T_Gen_Elec_Px;
   vector<float>   *T_Gen_Elec_Py;
   vector<float>   *T_Gen_Elec_Pz;
   vector<float>   *T_Gen_Elec_Energy;
   vector<int>     *T_Gen_Elec_MPID;
   vector<float>   *T_Gen_Elec_MPx;
   vector<float>   *T_Gen_Elec_MPy;
   vector<float>   *T_Gen_Elec_MPz;
   vector<float>   *T_Gen_Elec_MEnergy;
   vector<int>     *T_Gen_Elec_MSt;
   vector<int>     *T_Gen_b_PID;
   vector<float>   *T_Gen_b_Px;
   vector<float>   *T_Gen_b_Py;
   vector<float>   *T_Gen_b_Pz;
   vector<float>   *T_Gen_b_Energy;
   vector<int>     *T_Gen_b_MPID;
   vector<float>   *T_Gen_b_MPx;
   vector<float>   *T_Gen_b_MPy;
   vector<float>   *T_Gen_b_MPz;
   vector<float>   *T_Gen_b_MEnergy;
   vector<int>     *T_Gen_b_MSt;
   vector<int>     *T_Gen_MuonSt3_PID;
   vector<float>   *T_Gen_MuonSt3_Px;
   vector<float>   *T_Gen_MuonSt3_Py;
   vector<float>   *T_Gen_MuonSt3_Pz;
   vector<float>   *T_Gen_MuonSt3_Energy;
   vector<int>     *T_Gen_ElecSt3_PID;
   vector<float>   *T_Gen_ElecSt3_Px;
   vector<float>   *T_Gen_ElecSt3_Py;
   vector<float>   *T_Gen_ElecSt3_Pz;
   vector<float>   *T_Gen_ElecSt3_Energy;
   vector<int>     *T_Gen_bSt3_PID;
   vector<float>   *T_Gen_bSt3_Px;
   vector<float>   *T_Gen_bSt3_Py;
   vector<float>   *T_Gen_bSt3_Pz;
   vector<float>   *T_Gen_bSt3_Energy;
   vector<int>     *T_Gen_TauSt3_PID;
   vector<float>   *T_Gen_TauSt3_Px;
   vector<float>   *T_Gen_TauSt3_Py;
   vector<float>   *T_Gen_TauSt3_Pz;
   vector<float>   *T_Gen_TauSt3_Energy;
   vector<bool>    *T_Gen_TauSt3_IsLepDec;
   vector<int>     *T_Gen_TauSt3_LepDec_PID;
   vector<float>   *T_Gen_TauSt3_LepDec_Px;
   vector<float>   *T_Gen_TauSt3_LepDec_Py;
   vector<float>   *T_Gen_TauSt3_LepDec_Pz;
   vector<float>   *T_Gen_TauSt3_LepDec_Energy;
   vector<float>   *T_PFreducedCand_Px;
   vector<float>   *T_PFreducedCand_Py;
   vector<float>   *T_PFreducedCand_Pz;
   vector<float>   *T_PFreducedCand_E;
   vector<int>     *T_PFreducedCand_ID;
   vector<float>   *T_PFreducedCand_vz;
   vector<float>   *T_Muon_Eta;
   vector<bool>    *T_Muon_IsGlobalMuon;
   vector<bool>    *T_Muon_IsAllStandAloneMuons;
   vector<bool>    *T_Muon_IsAllTrackerMuons;
   vector<bool>    *T_Muon_IsTrackerMuonArbitrated;
   vector<bool>    *T_Muon_IsAllArbitrated;
   vector<bool>    *T_Muon_IsTMLastStationLoose;
   vector<bool>    *T_Muon_IsTMLastStationTight;
   vector<bool>    *T_Muon_IsTM2DCompatibilityLoose;
   vector<bool>    *T_Muon_IsTM2DCompatibilityTight;
   vector<bool>    *T_Muon_IsTMOneStationLoose;
   vector<bool>    *T_Muon_IsTMOneStationTight;
   vector<bool>    *T_Muon_IsTMLSOPL;
   vector<bool>    *T_Muon_IsGMTkChiCompatibility;
   vector<bool>    *T_Muon_IsGMStaChiCompatibility;
   vector<bool>    *T_Muon_IsGMTkKinkTight;
   vector<bool>    *T_Muon_IsTMLastStationAngLoose;
   vector<bool>    *T_Muon_IsTMLastStationAngTight;
   vector<bool>    *T_Muon_IsTMOneStationAngLoose;
   vector<bool>    *T_Muon_IsTMOneStationAngTight;
   vector<bool>    *T_Muon_IsGMPTMuons;
   vector<float>   *T_Muon_SegmentCompatibility;
   vector<float>   *T_Muon_trkKink;
   vector<float>   *T_Muon_Px;
   vector<float>   *T_Muon_Py;
   vector<float>   *T_Muon_Pz;
   vector<float>   *T_Muon_Pt;
   vector<float>   *T_Muon_MVARings;
   vector<float>   *T_Muon_MVAID;
   vector<float>   *T_Muon_deltaPt;
   vector<float>   *T_Muon_Energy;
   vector<int>     *T_Muon_Charge;
   vector<float>   *T_Muon_NormChi2GTrk;
   vector<int>     *T_Muon_NValidHitsInTrk;
   vector<int>     *T_Muon_NValidPixelHitsInTrk;
   vector<int>     *T_Muon_InnerTrackFound;
   vector<int>     *T_Muon_NValidHitsSATrk;
   vector<int>     *T_Muon_NValidHitsGTrk;
   vector<float>   *T_Muon_Chi2InTrk;
   vector<float>   *T_Muon_dofInTrk;
   vector<float>   *T_Muon_IPAbsGTrack;
   vector<float>   *T_Muon_IPAbsInTrack;
   vector<float>   *T_Muon_IPwrtAveBSInTrack;
   vector<float>   *T_Muon_IP2DBiasedPV;
   vector<float>   *T_Muon_dxyPVBiasedPV;
   vector<float>   *T_Muon_dzPVBiasedPV;
   vector<float>   *T_Muon_chargedHadronIsoR03;
   vector<float>   *T_Muon_neutralHadronIsoR03;
   vector<float>   *T_Muon_photonIsoR03;
   vector<float>   *T_Muon_chargedHadronIsoR04;
   vector<float>   *T_Muon_neutralHadronIsoR04;
   vector<float>   *T_Muon_photonIsoR04;
   vector<float>   *T_Muon_sumPUPtR04;
   vector<float>   *T_Muon_sumPUPtR03;
   vector<float>   *T_Muon_vz;
   vector<float>   *T_Muon_vy;
   vector<float>   *T_Muon_vx;
   vector<int>     *T_Muon_NumOfMatches;
   vector<float>   *T_Muon_EcalVeto;
   vector<float>   *T_Muon_HcalVeto;
   vector<float>   *T_Muon_PFMuonPt;
   vector<float>   *T_Muon_PFMuonPx;
   vector<float>   *T_Muon_PFMuonPy;
   vector<float>   *T_Muon_PFMuonPz;
   vector<float>   *T_Muon_PFMuonE;
   vector<bool>    *T_Muon_isPFMuon;
   vector<int>     *T_Muon_NLayers;
   vector<float>   *T_Vertex_z;
   vector<float>   *T_Vertex_y;
   vector<float>   *T_Vertex_x;
   vector<float>   *T_Vertex_Chi2Prob;
   vector<float>   *T_Vertex_rho;
   vector<float>   *T_Vertex_ndof;
   vector<bool>    *T_Vertex_isFake;
   vector<int>     *T_Vertex_tracksSize;
   vector<float>   *T_JetAKCHS_Px;
   vector<float>   *T_JetAKCHS_Py;
   vector<float>   *T_JetAKCHS_Pz;
   vector<float>   *T_JetAKCHS_Et;
   vector<float>   *T_JetAKCHS_EtOffset;
   vector<float>   *T_JetAKCHS_PtOffset;
   vector<float>   *T_JetAKCHS_Eta;
   vector<float>   *T_JetAKCHS_Energy;
   vector<float>   *T_JetAKCHS_Corr;
   vector<float>   *T_JetAKCHS_Tag_HighEffTC;
   vector<float>   *T_JetAKCHS_Tag_CombSVtx;
   vector<float>   *T_JetAKCHS_Tag_CombSVtxMVA;
   vector<float>   *T_JetAKCHS_Tag_TauJet;
   vector<float>   *T_JetAKCHS_Tag_ImpParMVA;
   vector<float>   *T_JetAKCHS_Tag_JetBProb;
   vector<float>   *T_JetAKCHS_Tag_JetProb;
   vector<float>   *T_JetAKCHS_Tag_HighEffSimpSVtx;
   vector<float>   *T_JetAKCHS_Tag_HighPurSimpSVtx;
   vector<float>   *T_JetAKCHS_Tag_HighPurTC;
   vector<float>   *T_JetAKCHS_Parton_Px;
   vector<float>   *T_JetAKCHS_Parton_Py;
   vector<float>   *T_JetAKCHS_Parton_Pz;
   vector<float>   *T_JetAKCHS_Parton_Energy;
   vector<int>     *T_JetAKCHS_Parton_Flavour;
   vector<float>   *T_JetAKCHS_CharHadEnergyFrac;
   vector<float>   *T_JetAKCHS_NeutHadEnergyFrac;
   vector<float>   *T_JetAKCHS_CharEmEnergyFrac;
   vector<float>   *T_JetAKCHS_NeutEmEnergyFrac;
   vector<float>   *T_JetAKCHS_CharHadEnergy;
   vector<float>   *T_JetAKCHS_NeutHadEnergy;
   vector<float>   *T_JetAKCHS_CharEmEnergy;
   vector<float>   *T_JetAKCHS_NeutEmEnergy;
   vector<int>     *T_JetAKCHS_MuonMultiplicity;
   vector<int>     *T_JetAKCHS_NeutralMultiplicity;
   vector<int>     *T_JetAKCHS_ChargedMultiplicity;
   vector<bool>    *T_JetAKCHS_IDLoose;
   vector<float>   *T_JetAKCHS_GenJet_InvisibleE;
   vector<float>   *T_JetAKCHS_GenJet_Px;
   vector<float>   *T_JetAKCHS_GenJet_Py;
   vector<float>   *T_JetAKCHS_GenJet_Pz;
   vector<float>   *T_JetAKCHS_GenJet_Et;
   vector<float>   *T_JetAKCHS_GenJet_Eta;
   vector<float>   *T_JetAKCHS_GenJet_Energy;
   vector<bool>    *T_JetAKCHS_IsGenJet;
   Float_t         T_METPF_ET;
   Float_t         T_METPF_Phi;
   Float_t         T_METPF_Sig;
   Float_t         T_METgen_ET;
   Float_t         T_METgen_Phi;
   Bool_t          T_passTriggerDoubleMu;
   Bool_t          T_passTriggerDoubleEl;
   Bool_t          T_passTriggerSingleMu;
   Bool_t          T_passTriggerElMu;

   // List of branches
   TBranch        *b_T_Event_Rho;   //!
   TBranch        *b_T_Event_RhoIso;   //!
   TBranch        *b_T_Event_RhoNoPu;   //!
   TBranch        *b_T_Event_RhoIsoNoPu;   //!
   TBranch        *b_T_Event_RhoCentralNeutral;   //!
   TBranch        *b_T_Event_RunNumber;   //!
   TBranch        *b_T_Event_EventNumber;   //!
   TBranch        *b_T_Event_LuminosityBlock;   //!
   TBranch        *b_T_Event_processID;   //!
   TBranch        *b_T_Event_nPU;   //!
   TBranch        *b_T_Event_nTruePU;   //!
   TBranch        *b_T_Event_nPUm;   //!
   TBranch        *b_T_Event_nPUp;   //!
   TBranch        *b_T_Event_AveNTruePU;   //!
   TBranch        *b_T_HLT_Mu9;   //!
   TBranch        *b_T_HLT_Mu8_v1;   //!
   TBranch        *b_T_HLT_Mu8_v2;   //!
   TBranch        *b_T_HLT_Mu8_v3;   //!
   TBranch        *b_T_HLT_Mu8_v4;   //!
   TBranch        *b_T_HLT_Mu8_v5;   //!
   TBranch        *b_T_HLT_Mu8_v6;   //!
   TBranch        *b_T_HLT_Mu8_v7;   //!
   TBranch        *b_T_HLT_Mu8_v8;   //!
   TBranch        *b_T_HLT_Mu8_v9;   //!
   TBranch        *b_T_HLT_Mu8_v10;   //!
   TBranch        *b_T_HLT_Mu8_v11;   //!
   TBranch        *b_T_HLT_Mu8_v12;   //!
   TBranch        *b_T_HLT_Mu8_v16;   //!
   TBranch        *b_T_HLT_Mu8_v17;   //!
   TBranch        *b_T_HLT_Mu8_v18;   //!
   TBranch        *b_T_HLT_Mu12_v1;   //!
   TBranch        *b_T_HLT_Mu12_v2;   //!
   TBranch        *b_T_HLT_Mu12_v3;   //!
   TBranch        *b_T_HLT_Mu12_v4;   //!
   TBranch        *b_T_HLT_Mu12_v16;   //!
   TBranch        *b_T_HLT_Mu15_v1;   //!
   TBranch        *b_T_HLT_Mu15_v2;   //!
   TBranch        *b_T_HLT_Mu15_v3;   //!
   TBranch        *b_T_HLT_Mu15_v4;   //!
   TBranch        *b_T_HLT_Mu15_v5;   //!
   TBranch        *b_T_HLT_Mu15_v6;   //!
   TBranch        *b_T_HLT_Mu15_v7;   //!
   TBranch        *b_T_HLT_Mu15_v8;   //!
   TBranch        *b_T_HLT_Mu15_v9;   //!
   TBranch        *b_T_HLT_Mu15_v10;   //!
   TBranch        *b_T_HLT_Mu15_v11;   //!
   TBranch        *b_T_HLT_Mu15_v12;   //!
   TBranch        *b_T_HLT_Mu15_v13;   //!
   TBranch        *b_T_HLT_Mu17_v3;   //!
   TBranch        *b_T_HLT_Mu17_v4;   //!
   TBranch        *b_T_HLT_Mu17_v5;   //!
   TBranch        *b_T_HLT_IsoMu24_eta2p1_v11;   //!
   TBranch        *b_T_HLT_IsoMu24_eta2p1_v12;   //!
   TBranch        *b_T_HLT_IsoMu24_eta2p1_v13;   //!
   TBranch        *b_T_HLT_Mu24_v1;   //!
   TBranch        *b_T_HLT_Mu24_v2;   //!
   TBranch        *b_T_HLT_Mu24_v3;   //!
   TBranch        *b_T_HLT_Mu24_v4;   //!
   TBranch        *b_T_HLT_Mu24_v5;   //!
   TBranch        *b_T_HLT_Mu24_v6;   //!
   TBranch        *b_T_HLT_Mu24_v7;   //!
   TBranch        *b_T_HLT_Mu24_v8;   //!
   TBranch        *b_T_HLT_Mu24_v9;   //!
   TBranch        *b_T_HLT_Mu24_v10;   //!
   TBranch        *b_T_HLT_Mu24_v11;   //!
   TBranch        *b_T_HLT_Mu24_v12;   //!
   TBranch        *b_T_HLT_Mu24_v13;   //!
   TBranch        *b_T_HLT_Jet30_v1;   //!
   TBranch        *b_T_HLT_Jet30_v2;   //!
   TBranch        *b_T_HLT_Jet30_v3;   //!
   TBranch        *b_T_HLT_Jet30_v4;   //!
   TBranch        *b_T_HLT_Jet30_v5;   //!
   TBranch        *b_T_HLT_Jet60_v1;   //!
   TBranch        *b_T_HLT_Jet60_v2;   //!
   TBranch        *b_T_HLT_Jet60_v3;   //!
   TBranch        *b_T_HLT_Jet60_v4;   //!
   TBranch        *b_T_HLT_Jet60_v5;   //!
   TBranch        *b_T_Gen_Muon_PID;   //!
   TBranch        *b_T_Gen_Muon_Px;   //!
   TBranch        *b_T_Gen_Muon_Py;   //!
   TBranch        *b_T_Gen_Muon_Pz;   //!
   TBranch        *b_T_Gen_Muon_Energy;   //!
   TBranch        *b_T_Gen_Muon_MPID;   //!
   TBranch        *b_T_Gen_Muon_MPx;   //!
   TBranch        *b_T_Gen_Muon_MPy;   //!
   TBranch        *b_T_Gen_Muon_MPz;   //!
   TBranch        *b_T_Gen_Muon_MEnergy;   //!
   TBranch        *b_T_Gen_Muon_MSt;   //!
   TBranch        *b_T_Gen_Elec_PID;   //!
   TBranch        *b_T_Gen_Elec_Px;   //!
   TBranch        *b_T_Gen_Elec_Py;   //!
   TBranch        *b_T_Gen_Elec_Pz;   //!
   TBranch        *b_T_Gen_Elec_Energy;   //!
   TBranch        *b_T_Gen_Elec_MPID;   //!
   TBranch        *b_T_Gen_Elec_MPx;   //!
   TBranch        *b_T_Gen_Elec_MPy;   //!
   TBranch        *b_T_Gen_Elec_MPz;   //!
   TBranch        *b_T_Gen_Elec_MEnergy;   //!
   TBranch        *b_T_Gen_Elec_MSt;   //!
   TBranch        *b_T_Gen_b_PID;   //!
   TBranch        *b_T_Gen_b_Px;   //!
   TBranch        *b_T_Gen_b_Py;   //!
   TBranch        *b_T_Gen_b_Pz;   //!
   TBranch        *b_T_Gen_b_Energy;   //!
   TBranch        *b_T_Gen_b_MPID;   //!
   TBranch        *b_T_Gen_b_MPx;   //!
   TBranch        *b_T_Gen_b_MPy;   //!
   TBranch        *b_T_Gen_b_MPz;   //!
   TBranch        *b_T_Gen_b_MEnergy;   //!
   TBranch        *b_T_Gen_b_MSt;   //!
   TBranch        *b_T_Gen_MuonSt3_PID;   //!
   TBranch        *b_T_Gen_MuonSt3_Px;   //!
   TBranch        *b_T_Gen_MuonSt3_Py;   //!
   TBranch        *b_T_Gen_MuonSt3_Pz;   //!
   TBranch        *b_T_Gen_MuonSt3_Energy;   //!
   TBranch        *b_T_Gen_ElecSt3_PID;   //!
   TBranch        *b_T_Gen_ElecSt3_Px;   //!
   TBranch        *b_T_Gen_ElecSt3_Py;   //!
   TBranch        *b_T_Gen_ElecSt3_Pz;   //!
   TBranch        *b_T_Gen_ElecSt3_Energy;   //!
   TBranch        *b_T_Gen_bSt3_PID;   //!
   TBranch        *b_T_Gen_bSt3_Px;   //!
   TBranch        *b_T_Gen_bSt3_Py;   //!
   TBranch        *b_T_Gen_bSt3_Pz;   //!
   TBranch        *b_T_Gen_bSt3_Energy;   //!
   TBranch        *b_T_Gen_TauSt3_PID;   //!
   TBranch        *b_T_Gen_TauSt3_Px;   //!
   TBranch        *b_T_Gen_TauSt3_Py;   //!
   TBranch        *b_T_Gen_TauSt3_Pz;   //!
   TBranch        *b_T_Gen_TauSt3_Energy;   //!
   TBranch        *b_T_Gen_TauSt3_IsLepDec;   //!
   TBranch        *b_T_Gen_TauSt3_LepDec_PID;   //!
   TBranch        *b_T_Gen_TauSt3_LepDec_Px;   //!
   TBranch        *b_T_Gen_TauSt3_LepDec_Py;   //!
   TBranch        *b_T_Gen_TauSt3_LepDec_Pz;   //!
   TBranch        *b_T_Gen_TauSt3_LepDec_Energy;   //!
   TBranch        *b_T_PFreducedCand_Px;   //!
   TBranch        *b_T_PFreducedCand_Py;   //!
   TBranch        *b_T_PFreducedCand_Pz;   //!
   TBranch        *b_T_PFreducedCand_E;   //!
   TBranch        *b_T_PFreducedCand_ID;   //!
   TBranch        *b_T_PFreducedCand_vz;   //!
   TBranch        *b_T_Muon_Eta;   //!
   TBranch        *b_T_Muon_IsGlobalMuon;   //!
   TBranch        *b_T_Muon_IsAllStandAloneMuons;   //!
   TBranch        *b_T_Muon_IsAllTrackerMuons;   //!
   TBranch        *b_T_Muon_IsTrackerMuonArbitrated;   //!
   TBranch        *b_T_Muon_IsAllArbitrated;   //!
   TBranch        *b_T_Muon_IsTMLastStationLoose;   //!
   TBranch        *b_T_Muon_IsTMLastStationTight;   //!
   TBranch        *b_T_Muon_IsTM2DCompatibilityLoose;   //!
   TBranch        *b_T_Muon_IsTM2DCompatibilityTight;   //!
   TBranch        *b_T_Muon_IsTMOneStationLoose;   //!
   TBranch        *b_T_Muon_IsTMOneStationTight;   //!
   TBranch        *b_T_Muon_IsTMLSOPL;   //!
   TBranch        *b_T_Muon_IsGMTkChiCompatibility;   //!
   TBranch        *b_T_Muon_IsGMStaChiCompatibility;   //!
   TBranch        *b_T_Muon_IsGMTkKinkTight;   //!
   TBranch        *b_T_Muon_IsTMLastStationAngLoose;   //!
   TBranch        *b_T_Muon_IsTMLastStationAngTight;   //!
   TBranch        *b_T_Muon_IsTMOneStationAngLoose;   //!
   TBranch        *b_T_Muon_IsTMOneStationAngTight;   //!
   TBranch        *b_T_Muon_IsGMPTMuons;   //!
   TBranch        *b_T_Muon_SegmentCompatibility;   //!
   TBranch        *b_T_Muon_trkKink;   //!
   TBranch        *b_T_Muon_Px;   //!
   TBranch        *b_T_Muon_Py;   //!
   TBranch        *b_T_Muon_Pz;   //!
   TBranch        *b_T_Muon_Pt;   //!
   TBranch        *b_T_Muon_MVARings;   //!
   TBranch        *b_T_Muon_MVAID;   //!
   TBranch        *b_T_Muon_deltaPt;   //!
   TBranch        *b_T_Muon_Energy;   //!
   TBranch        *b_T_Muon_Charge;   //!
   TBranch        *b_T_Muon_NormChi2GTrk;   //!
   TBranch        *b_T_Muon_NValidHitsInTrk;   //!
   TBranch        *b_T_Muon_NValidPixelHitsInTrk;   //!
   TBranch        *b_T_Muon_InnerTrackFound;   //!
   TBranch        *b_T_Muon_NValidHitsSATrk;   //!
   TBranch        *b_T_Muon_NValidHitsGTrk;   //!
   TBranch        *b_T_Muon_Chi2InTrk;   //!
   TBranch        *b_T_Muon_dofInTrk;   //!
   TBranch        *b_T_Muon_IPAbsGTrack;   //!
   TBranch        *b_T_Muon_IPAbsInTrack;   //!
   TBranch        *b_T_Muon_IPwrtAveBSInTrack;   //!
   TBranch        *b_T_Muon_IP2DBiasedPV;   //!
   TBranch        *b_T_Muon_dxyPVBiasedPV;   //!
   TBranch        *b_T_Muon_dzPVBiasedPV;   //!
   TBranch        *b_T_Muon_chargedHadronIsoR03;   //!
   TBranch        *b_T_Muon_neutralHadronIsoR03;   //!
   TBranch        *b_T_Muon_photonIsoR03;   //!
   TBranch        *b_T_Muon_chargedHadronIsoR04;   //!
   TBranch        *b_T_Muon_neutralHadronIsoR04;   //!
   TBranch        *b_T_Muon_photonIsoR04;   //!
   TBranch        *b_T_Muon_sumPUPtR04;   //!
   TBranch        *b_T_Muon_sumPUPtR03;   //!
   TBranch        *b_T_Muon_vz;   //!
   TBranch        *b_T_Muon_vy;   //!
   TBranch        *b_T_Muon_vx;   //!
   TBranch        *b_T_Muon_NumOfMatches;   //!
   TBranch        *b_T_Muon_EcalVeto;   //!
   TBranch        *b_T_Muon_HcalVeto;   //!
   TBranch        *b_T_Muon_PFMuonPt;   //!
   TBranch        *b_T_Muon_PFMuonPx;   //!
   TBranch        *b_T_Muon_PFMuonPy;   //!
   TBranch        *b_T_Muon_PFMuonPz;   //!
   TBranch        *b_T_Muon_PFMuonE;   //!
   TBranch        *b_T_Muon_isPFMuon;   //!
   TBranch        *b_T_Muon_NLayers;   //!
   TBranch        *b_T_Vertex_z;   //!
   TBranch        *b_T_Vertex_y;   //!
   TBranch        *b_T_Vertex_x;   //!
   TBranch        *b_T_Vertex_Chi2Prob;   //!
   TBranch        *b_T_Vertex_rho;   //!
   TBranch        *b_T_Vertex_ndof;   //!
   TBranch        *b_T_Vertex_isFake;   //!
   TBranch        *b_T_Vertex_tracksSize;   //!
   TBranch        *b_T_JetAKCHS_Px;   //!
   TBranch        *b_T_JetAKCHS_Py;   //!
   TBranch        *b_T_JetAKCHS_Pz;   //!
   TBranch        *b_T_JetAKCHS_Et;   //!
   TBranch        *b_T_JetAKCHS_EtOffset;   //!
   TBranch        *b_T_JetAKCHS_PtOffset;   //!
   TBranch        *b_T_JetAKCHS_Eta;   //!
   TBranch        *b_T_JetAKCHS_Energy;   //!
   TBranch        *b_T_JetAKCHS_Corr;   //!
   TBranch        *b_T_JetAKCHS_Tag_HighEffTC;   //!
   TBranch        *b_T_JetAKCHS_Tag_CombSVtx;   //!
   TBranch        *b_T_JetAKCHS_Tag_CombSVtxMVA;   //!
   TBranch        *b_T_JetAKCHS_Tag_TauJet;   //!
   TBranch        *b_T_JetAKCHS_Tag_ImpParMVA;   //!
   TBranch        *b_T_JetAKCHS_Tag_JetBProb;   //!
   TBranch        *b_T_JetAKCHS_Tag_JetProb;   //!
   TBranch        *b_T_JetAKCHS_Tag_HighEffSimpSVtx;   //!
   TBranch        *b_T_JetAKCHS_Tag_HighPurSimpSVtx;   //!
   TBranch        *b_T_JetAKCHS_Tag_HighPurTC;   //!
   TBranch        *b_T_JetAKCHS_Parton_Px;   //!
   TBranch        *b_T_JetAKCHS_Parton_Py;   //!
   TBranch        *b_T_JetAKCHS_Parton_Pz;   //!
   TBranch        *b_T_JetAKCHS_Parton_Energy;   //!
   TBranch        *b_T_JetAKCHS_Parton_Flavour;   //!
   TBranch        *b_T_JetAKCHS_CharHadEnergyFrac;   //!
   TBranch        *b_T_JetAKCHS_NeutHadEnergyFrac;   //!
   TBranch        *b_T_JetAKCHS_CharEmEnergyFrac;   //!
   TBranch        *b_T_JetAKCHS_NeutEmEnergyFrac;   //!
   TBranch        *b_T_JetAKCHS_CharHadEnergy;   //!
   TBranch        *b_T_JetAKCHS_NeutHadEnergy;   //!
   TBranch        *b_T_JetAKCHS_CharEmEnergy;   //!
   TBranch        *b_T_JetAKCHS_NeutEmEnergy;   //!
   TBranch        *b_T_JetAKCHS_MuonMultiplicity;   //!
   TBranch        *b_T_JetAKCHS_NeutralMultiplicity;   //!
   TBranch        *b_T_JetAKCHS_ChargedMultiplicity;   //!
   TBranch        *b_T_JetAKCHS_IDLoose;   //!
   TBranch        *b_T_JetAKCHS_GenJet_InvisibleE;   //!
   TBranch        *b_T_JetAKCHS_GenJet_Px;   //!
   TBranch        *b_T_JetAKCHS_GenJet_Py;   //!
   TBranch        *b_T_JetAKCHS_GenJet_Pz;   //!
   TBranch        *b_T_JetAKCHS_GenJet_Et;   //!
   TBranch        *b_T_JetAKCHS_GenJet_Eta;   //!
   TBranch        *b_T_JetAKCHS_GenJet_Energy;   //!
   TBranch        *b_T_JetAKCHS_IsGenJet;   //!
   TBranch        *b_T_METPF_ET;   //!
   TBranch        *b_T_METPF_Phi;   //!
   TBranch        *b_T_METPF_Sig;   //!
   TBranch        *b_T_METgen_ET;   //!
   TBranch        *b_T_METgen_Phi;   //!
   TBranch        *b_T_passTriggerDoubleMu;   //!
   TBranch        *b_T_passTriggerDoubleEl;   //!
   TBranch        *b_T_passTriggerSingleMu;   //!
   TBranch        *b_T_passTriggerElMu;   //!

   MuonFakeRate(TTree *tree=0);
   virtual ~MuonFakeRate();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MuonFakeRate_cxx
MuonFakeRate::MuonFakeRate(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("Tree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("Tree","Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuA_810_FR.root/Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuA_ECALrecovery_82pbinv_FR.root/Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuB_4404pbinv_FR.root/Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuC_6312pbinv_FR.root/Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuC_91pbinv_FR.root/Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuC_ReReco_495pbinv_FR.root/Tree");
      chain->Add("/pool/ciencias/LatinosSkims/MuData8TeVRun2012ABC/Tree_DoubleMuD_7274pbinv_MuFR.root/Tree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

MuonFakeRate::~MuonFakeRate()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MuonFakeRate::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MuonFakeRate::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MuonFakeRate::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   T_Gen_Muon_PID = 0;
   T_Gen_Muon_Px = 0;
   T_Gen_Muon_Py = 0;
   T_Gen_Muon_Pz = 0;
   T_Gen_Muon_Energy = 0;
   T_Gen_Muon_MPID = 0;
   T_Gen_Muon_MPx = 0;
   T_Gen_Muon_MPy = 0;
   T_Gen_Muon_MPz = 0;
   T_Gen_Muon_MEnergy = 0;
   T_Gen_Muon_MSt = 0;
   T_Gen_Elec_PID = 0;
   T_Gen_Elec_Px = 0;
   T_Gen_Elec_Py = 0;
   T_Gen_Elec_Pz = 0;
   T_Gen_Elec_Energy = 0;
   T_Gen_Elec_MPID = 0;
   T_Gen_Elec_MPx = 0;
   T_Gen_Elec_MPy = 0;
   T_Gen_Elec_MPz = 0;
   T_Gen_Elec_MEnergy = 0;
   T_Gen_Elec_MSt = 0;
   T_Gen_b_PID = 0;
   T_Gen_b_Px = 0;
   T_Gen_b_Py = 0;
   T_Gen_b_Pz = 0;
   T_Gen_b_Energy = 0;
   T_Gen_b_MPID = 0;
   T_Gen_b_MPx = 0;
   T_Gen_b_MPy = 0;
   T_Gen_b_MPz = 0;
   T_Gen_b_MEnergy = 0;
   T_Gen_b_MSt = 0;
   T_Gen_MuonSt3_PID = 0;
   T_Gen_MuonSt3_Px = 0;
   T_Gen_MuonSt3_Py = 0;
   T_Gen_MuonSt3_Pz = 0;
   T_Gen_MuonSt3_Energy = 0;
   T_Gen_ElecSt3_PID = 0;
   T_Gen_ElecSt3_Px = 0;
   T_Gen_ElecSt3_Py = 0;
   T_Gen_ElecSt3_Pz = 0;
   T_Gen_ElecSt3_Energy = 0;
   T_Gen_bSt3_PID = 0;
   T_Gen_bSt3_Px = 0;
   T_Gen_bSt3_Py = 0;
   T_Gen_bSt3_Pz = 0;
   T_Gen_bSt3_Energy = 0;
   T_Gen_TauSt3_PID = 0;
   T_Gen_TauSt3_Px = 0;
   T_Gen_TauSt3_Py = 0;
   T_Gen_TauSt3_Pz = 0;
   T_Gen_TauSt3_Energy = 0;
   T_Gen_TauSt3_IsLepDec = 0;
   T_Gen_TauSt3_LepDec_PID = 0;
   T_Gen_TauSt3_LepDec_Px = 0;
   T_Gen_TauSt3_LepDec_Py = 0;
   T_Gen_TauSt3_LepDec_Pz = 0;
   T_Gen_TauSt3_LepDec_Energy = 0;
   T_PFreducedCand_Px = 0;
   T_PFreducedCand_Py = 0;
   T_PFreducedCand_Pz = 0;
   T_PFreducedCand_E = 0;
   T_PFreducedCand_ID = 0;
   T_PFreducedCand_vz = 0;
   T_Muon_Eta = 0;
   T_Muon_IsGlobalMuon = 0;
   T_Muon_IsAllStandAloneMuons = 0;
   T_Muon_IsAllTrackerMuons = 0;
   T_Muon_IsTrackerMuonArbitrated = 0;
   T_Muon_IsAllArbitrated = 0;
   T_Muon_IsTMLastStationLoose = 0;
   T_Muon_IsTMLastStationTight = 0;
   T_Muon_IsTM2DCompatibilityLoose = 0;
   T_Muon_IsTM2DCompatibilityTight = 0;
   T_Muon_IsTMOneStationLoose = 0;
   T_Muon_IsTMOneStationTight = 0;
   T_Muon_IsTMLSOPL = 0;
   T_Muon_IsGMTkChiCompatibility = 0;
   T_Muon_IsGMStaChiCompatibility = 0;
   T_Muon_IsGMTkKinkTight = 0;
   T_Muon_IsTMLastStationAngLoose = 0;
   T_Muon_IsTMLastStationAngTight = 0;
   T_Muon_IsTMOneStationAngLoose = 0;
   T_Muon_IsTMOneStationAngTight = 0;
   T_Muon_IsGMPTMuons = 0;
   T_Muon_SegmentCompatibility = 0;
   T_Muon_trkKink = 0;
   T_Muon_Px = 0;
   T_Muon_Py = 0;
   T_Muon_Pz = 0;
   T_Muon_Pt = 0;
   T_Muon_MVARings = 0;
   T_Muon_MVAID = 0;
   T_Muon_deltaPt = 0;
   T_Muon_Energy = 0;
   T_Muon_Charge = 0;
   T_Muon_NormChi2GTrk = 0;
   T_Muon_NValidHitsInTrk = 0;
   T_Muon_NValidPixelHitsInTrk = 0;
   T_Muon_InnerTrackFound = 0;
   T_Muon_NValidHitsSATrk = 0;
   T_Muon_NValidHitsGTrk = 0;
   T_Muon_Chi2InTrk = 0;
   T_Muon_dofInTrk = 0;
   T_Muon_IPAbsGTrack = 0;
   T_Muon_IPAbsInTrack = 0;
   T_Muon_IPwrtAveBSInTrack = 0;
   T_Muon_IP2DBiasedPV = 0;
   T_Muon_dxyPVBiasedPV = 0;
   T_Muon_dzPVBiasedPV = 0;
   T_Muon_chargedHadronIsoR03 = 0;
   T_Muon_neutralHadronIsoR03 = 0;
   T_Muon_photonIsoR03 = 0;
   T_Muon_chargedHadronIsoR04 = 0;
   T_Muon_neutralHadronIsoR04 = 0;
   T_Muon_photonIsoR04 = 0;
   T_Muon_sumPUPtR04 = 0;
   T_Muon_sumPUPtR03 = 0;
   T_Muon_vz = 0;
   T_Muon_vy = 0;
   T_Muon_vx = 0;
   T_Muon_NumOfMatches = 0;
   T_Muon_EcalVeto = 0;
   T_Muon_HcalVeto = 0;
   T_Muon_PFMuonPt = 0;
   T_Muon_PFMuonPx = 0;
   T_Muon_PFMuonPy = 0;
   T_Muon_PFMuonPz = 0;
   T_Muon_PFMuonE = 0;
   T_Muon_isPFMuon = 0;
   T_Muon_NLayers = 0;
   T_Vertex_z = 0;
   T_Vertex_y = 0;
   T_Vertex_x = 0;
   T_Vertex_Chi2Prob = 0;
   T_Vertex_rho = 0;
   T_Vertex_ndof = 0;
   T_Vertex_isFake = 0;
   T_Vertex_tracksSize = 0;
   T_JetAKCHS_Px = 0;
   T_JetAKCHS_Py = 0;
   T_JetAKCHS_Pz = 0;
   T_JetAKCHS_Et = 0;
   T_JetAKCHS_EtOffset = 0;
   T_JetAKCHS_PtOffset = 0;
   T_JetAKCHS_Eta = 0;
   T_JetAKCHS_Energy = 0;
   T_JetAKCHS_Corr = 0;
   T_JetAKCHS_Tag_HighEffTC = 0;
   T_JetAKCHS_Tag_CombSVtx = 0;
   T_JetAKCHS_Tag_CombSVtxMVA = 0;
   T_JetAKCHS_Tag_TauJet = 0;
   T_JetAKCHS_Tag_ImpParMVA = 0;
   T_JetAKCHS_Tag_JetBProb = 0;
   T_JetAKCHS_Tag_JetProb = 0;
   T_JetAKCHS_Tag_HighEffSimpSVtx = 0;
   T_JetAKCHS_Tag_HighPurSimpSVtx = 0;
   T_JetAKCHS_Tag_HighPurTC = 0;
   T_JetAKCHS_Parton_Px = 0;
   T_JetAKCHS_Parton_Py = 0;
   T_JetAKCHS_Parton_Pz = 0;
   T_JetAKCHS_Parton_Energy = 0;
   T_JetAKCHS_Parton_Flavour = 0;
   T_JetAKCHS_CharHadEnergyFrac = 0;
   T_JetAKCHS_NeutHadEnergyFrac = 0;
   T_JetAKCHS_CharEmEnergyFrac = 0;
   T_JetAKCHS_NeutEmEnergyFrac = 0;
   T_JetAKCHS_CharHadEnergy = 0;
   T_JetAKCHS_NeutHadEnergy = 0;
   T_JetAKCHS_CharEmEnergy = 0;
   T_JetAKCHS_NeutEmEnergy = 0;
   T_JetAKCHS_MuonMultiplicity = 0;
   T_JetAKCHS_NeutralMultiplicity = 0;
   T_JetAKCHS_ChargedMultiplicity = 0;
   T_JetAKCHS_IDLoose = 0;
   T_JetAKCHS_GenJet_InvisibleE = 0;
   T_JetAKCHS_GenJet_Px = 0;
   T_JetAKCHS_GenJet_Py = 0;
   T_JetAKCHS_GenJet_Pz = 0;
   T_JetAKCHS_GenJet_Et = 0;
   T_JetAKCHS_GenJet_Eta = 0;
   T_JetAKCHS_GenJet_Energy = 0;
   T_JetAKCHS_IsGenJet = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("T_Event_Rho", &T_Event_Rho, &b_T_Event_Rho);
   fChain->SetBranchAddress("T_Event_RhoIso", &T_Event_RhoIso, &b_T_Event_RhoIso);
   fChain->SetBranchAddress("T_Event_RhoNoPu", &T_Event_RhoNoPu, &b_T_Event_RhoNoPu);
   fChain->SetBranchAddress("T_Event_RhoIsoNoPu", &T_Event_RhoIsoNoPu, &b_T_Event_RhoIsoNoPu);
   fChain->SetBranchAddress("T_Event_RhoCentralNeutral", &T_Event_RhoCentralNeutral, &b_T_Event_RhoCentralNeutral);
   fChain->SetBranchAddress("T_Event_RunNumber", &T_Event_RunNumber, &b_T_Event_RunNumber);
   fChain->SetBranchAddress("T_Event_EventNumber", &T_Event_EventNumber, &b_T_Event_EventNumber);
   fChain->SetBranchAddress("T_Event_LuminosityBlock", &T_Event_LuminosityBlock, &b_T_Event_LuminosityBlock);
   fChain->SetBranchAddress("T_Event_processID", &T_Event_processID, &b_T_Event_processID);
   fChain->SetBranchAddress("T_Event_nPU", &T_Event_nPU, &b_T_Event_nPU);
   fChain->SetBranchAddress("T_Event_nTruePU", &T_Event_nTruePU, &b_T_Event_nTruePU);
   fChain->SetBranchAddress("T_Event_nPUm", &T_Event_nPUm, &b_T_Event_nPUm);
   fChain->SetBranchAddress("T_Event_nPUp", &T_Event_nPUp, &b_T_Event_nPUp);
   fChain->SetBranchAddress("T_Event_AveNTruePU", &T_Event_AveNTruePU, &b_T_Event_AveNTruePU);
   fChain->SetBranchAddress("T_HLT_Mu9", &T_HLT_Mu9, &b_T_HLT_Mu9);
   fChain->SetBranchAddress("T_HLT_Mu8_v1", &T_HLT_Mu8_v1, &b_T_HLT_Mu8_v1);
   fChain->SetBranchAddress("T_HLT_Mu8_v2", &T_HLT_Mu8_v2, &b_T_HLT_Mu8_v2);
   fChain->SetBranchAddress("T_HLT_Mu8_v3", &T_HLT_Mu8_v3, &b_T_HLT_Mu8_v3);
   fChain->SetBranchAddress("T_HLT_Mu8_v4", &T_HLT_Mu8_v4, &b_T_HLT_Mu8_v4);
   fChain->SetBranchAddress("T_HLT_Mu8_v5", &T_HLT_Mu8_v5, &b_T_HLT_Mu8_v5);
   fChain->SetBranchAddress("T_HLT_Mu8_v6", &T_HLT_Mu8_v6, &b_T_HLT_Mu8_v6);
   fChain->SetBranchAddress("T_HLT_Mu8_v7", &T_HLT_Mu8_v7, &b_T_HLT_Mu8_v7);
   fChain->SetBranchAddress("T_HLT_Mu8_v8", &T_HLT_Mu8_v8, &b_T_HLT_Mu8_v8);
   fChain->SetBranchAddress("T_HLT_Mu8_v9", &T_HLT_Mu8_v9, &b_T_HLT_Mu8_v9);
   fChain->SetBranchAddress("T_HLT_Mu8_v10", &T_HLT_Mu8_v10, &b_T_HLT_Mu8_v10);
   fChain->SetBranchAddress("T_HLT_Mu8_v11", &T_HLT_Mu8_v11, &b_T_HLT_Mu8_v11);
   fChain->SetBranchAddress("T_HLT_Mu8_v12", &T_HLT_Mu8_v12, &b_T_HLT_Mu8_v12);
   fChain->SetBranchAddress("T_HLT_Mu8_v16", &T_HLT_Mu8_v16, &b_T_HLT_Mu8_v16);
   fChain->SetBranchAddress("T_HLT_Mu8_v17", &T_HLT_Mu8_v17, &b_T_HLT_Mu8_v17);
   fChain->SetBranchAddress("T_HLT_Mu8_v18", &T_HLT_Mu8_v18, &b_T_HLT_Mu8_v18);
   fChain->SetBranchAddress("T_HLT_Mu12_v1", &T_HLT_Mu12_v1, &b_T_HLT_Mu12_v1);
   fChain->SetBranchAddress("T_HLT_Mu12_v2", &T_HLT_Mu12_v2, &b_T_HLT_Mu12_v2);
   fChain->SetBranchAddress("T_HLT_Mu12_v3", &T_HLT_Mu12_v3, &b_T_HLT_Mu12_v3);
   fChain->SetBranchAddress("T_HLT_Mu12_v4", &T_HLT_Mu12_v4, &b_T_HLT_Mu12_v4);
   fChain->SetBranchAddress("T_HLT_Mu12_v16", &T_HLT_Mu12_v16, &b_T_HLT_Mu12_v16);
   fChain->SetBranchAddress("T_HLT_Mu15_v1", &T_HLT_Mu15_v1, &b_T_HLT_Mu15_v1);
   fChain->SetBranchAddress("T_HLT_Mu15_v2", &T_HLT_Mu15_v2, &b_T_HLT_Mu15_v2);
   fChain->SetBranchAddress("T_HLT_Mu15_v3", &T_HLT_Mu15_v3, &b_T_HLT_Mu15_v3);
   fChain->SetBranchAddress("T_HLT_Mu15_v4", &T_HLT_Mu15_v4, &b_T_HLT_Mu15_v4);
   fChain->SetBranchAddress("T_HLT_Mu15_v5", &T_HLT_Mu15_v5, &b_T_HLT_Mu15_v5);
   fChain->SetBranchAddress("T_HLT_Mu15_v6", &T_HLT_Mu15_v6, &b_T_HLT_Mu15_v6);
   fChain->SetBranchAddress("T_HLT_Mu15_v7", &T_HLT_Mu15_v7, &b_T_HLT_Mu15_v7);
   fChain->SetBranchAddress("T_HLT_Mu15_v8", &T_HLT_Mu15_v8, &b_T_HLT_Mu15_v8);
   fChain->SetBranchAddress("T_HLT_Mu15_v9", &T_HLT_Mu15_v9, &b_T_HLT_Mu15_v9);
   fChain->SetBranchAddress("T_HLT_Mu15_v10", &T_HLT_Mu15_v10, &b_T_HLT_Mu15_v10);
   fChain->SetBranchAddress("T_HLT_Mu15_v11", &T_HLT_Mu15_v11, &b_T_HLT_Mu15_v11);
   fChain->SetBranchAddress("T_HLT_Mu15_v12", &T_HLT_Mu15_v12, &b_T_HLT_Mu15_v12);
   fChain->SetBranchAddress("T_HLT_Mu15_v13", &T_HLT_Mu15_v13, &b_T_HLT_Mu15_v13);
   fChain->SetBranchAddress("T_HLT_Mu17_v3", &T_HLT_Mu17_v3, &b_T_HLT_Mu17_v3);
   fChain->SetBranchAddress("T_HLT_Mu17_v4", &T_HLT_Mu17_v4, &b_T_HLT_Mu17_v4);
   fChain->SetBranchAddress("T_HLT_Mu17_v5", &T_HLT_Mu17_v5, &b_T_HLT_Mu17_v5);
   fChain->SetBranchAddress("T_HLT_IsoMu24_eta2p1_v11", &T_HLT_IsoMu24_eta2p1_v11, &b_T_HLT_IsoMu24_eta2p1_v11);
   fChain->SetBranchAddress("T_HLT_IsoMu24_eta2p1_v12", &T_HLT_IsoMu24_eta2p1_v12, &b_T_HLT_IsoMu24_eta2p1_v12);
   fChain->SetBranchAddress("T_HLT_IsoMu24_eta2p1_v13", &T_HLT_IsoMu24_eta2p1_v13, &b_T_HLT_IsoMu24_eta2p1_v13);
   fChain->SetBranchAddress("T_HLT_Mu24_v1", &T_HLT_Mu24_v1, &b_T_HLT_Mu24_v1);
   fChain->SetBranchAddress("T_HLT_Mu24_v2", &T_HLT_Mu24_v2, &b_T_HLT_Mu24_v2);
   fChain->SetBranchAddress("T_HLT_Mu24_v3", &T_HLT_Mu24_v3, &b_T_HLT_Mu24_v3);
   fChain->SetBranchAddress("T_HLT_Mu24_v4", &T_HLT_Mu24_v4, &b_T_HLT_Mu24_v4);
   fChain->SetBranchAddress("T_HLT_Mu24_v5", &T_HLT_Mu24_v5, &b_T_HLT_Mu24_v5);
   fChain->SetBranchAddress("T_HLT_Mu24_v6", &T_HLT_Mu24_v6, &b_T_HLT_Mu24_v6);
   fChain->SetBranchAddress("T_HLT_Mu24_v7", &T_HLT_Mu24_v7, &b_T_HLT_Mu24_v7);
   fChain->SetBranchAddress("T_HLT_Mu24_v8", &T_HLT_Mu24_v8, &b_T_HLT_Mu24_v8);
   fChain->SetBranchAddress("T_HLT_Mu24_v9", &T_HLT_Mu24_v9, &b_T_HLT_Mu24_v9);
   fChain->SetBranchAddress("T_HLT_Mu24_v10", &T_HLT_Mu24_v10, &b_T_HLT_Mu24_v10);
   fChain->SetBranchAddress("T_HLT_Mu24_v11", &T_HLT_Mu24_v11, &b_T_HLT_Mu24_v11);
   fChain->SetBranchAddress("T_HLT_Mu24_v12", &T_HLT_Mu24_v12, &b_T_HLT_Mu24_v12);
   fChain->SetBranchAddress("T_HLT_Mu24_v13", &T_HLT_Mu24_v13, &b_T_HLT_Mu24_v13);
   fChain->SetBranchAddress("T_HLT_Jet30_v1", &T_HLT_Jet30_v1, &b_T_HLT_Jet30_v1);
   fChain->SetBranchAddress("T_HLT_Jet30_v2", &T_HLT_Jet30_v2, &b_T_HLT_Jet30_v2);
   fChain->SetBranchAddress("T_HLT_Jet30_v3", &T_HLT_Jet30_v3, &b_T_HLT_Jet30_v3);
   fChain->SetBranchAddress("T_HLT_Jet30_v4", &T_HLT_Jet30_v4, &b_T_HLT_Jet30_v4);
   fChain->SetBranchAddress("T_HLT_Jet30_v5", &T_HLT_Jet30_v5, &b_T_HLT_Jet30_v5);
   fChain->SetBranchAddress("T_HLT_Jet60_v1", &T_HLT_Jet60_v1, &b_T_HLT_Jet60_v1);
   fChain->SetBranchAddress("T_HLT_Jet60_v2", &T_HLT_Jet60_v2, &b_T_HLT_Jet60_v2);
   fChain->SetBranchAddress("T_HLT_Jet60_v3", &T_HLT_Jet60_v3, &b_T_HLT_Jet60_v3);
   fChain->SetBranchAddress("T_HLT_Jet60_v4", &T_HLT_Jet60_v4, &b_T_HLT_Jet60_v4);
   fChain->SetBranchAddress("T_HLT_Jet60_v5", &T_HLT_Jet60_v5, &b_T_HLT_Jet60_v5);
   fChain->SetBranchAddress("T_Gen_Muon_PID", &T_Gen_Muon_PID, &b_T_Gen_Muon_PID);
   fChain->SetBranchAddress("T_Gen_Muon_Px", &T_Gen_Muon_Px, &b_T_Gen_Muon_Px);
   fChain->SetBranchAddress("T_Gen_Muon_Py", &T_Gen_Muon_Py, &b_T_Gen_Muon_Py);
   fChain->SetBranchAddress("T_Gen_Muon_Pz", &T_Gen_Muon_Pz, &b_T_Gen_Muon_Pz);
   fChain->SetBranchAddress("T_Gen_Muon_Energy", &T_Gen_Muon_Energy, &b_T_Gen_Muon_Energy);
   fChain->SetBranchAddress("T_Gen_Muon_MPID", &T_Gen_Muon_MPID, &b_T_Gen_Muon_MPID);
   fChain->SetBranchAddress("T_Gen_Muon_MPx", &T_Gen_Muon_MPx, &b_T_Gen_Muon_MPx);
   fChain->SetBranchAddress("T_Gen_Muon_MPy", &T_Gen_Muon_MPy, &b_T_Gen_Muon_MPy);
   fChain->SetBranchAddress("T_Gen_Muon_MPz", &T_Gen_Muon_MPz, &b_T_Gen_Muon_MPz);
   fChain->SetBranchAddress("T_Gen_Muon_MEnergy", &T_Gen_Muon_MEnergy, &b_T_Gen_Muon_MEnergy);
   fChain->SetBranchAddress("T_Gen_Muon_MSt", &T_Gen_Muon_MSt, &b_T_Gen_Muon_MSt);
   fChain->SetBranchAddress("T_Gen_Elec_PID", &T_Gen_Elec_PID, &b_T_Gen_Elec_PID);
   fChain->SetBranchAddress("T_Gen_Elec_Px", &T_Gen_Elec_Px, &b_T_Gen_Elec_Px);
   fChain->SetBranchAddress("T_Gen_Elec_Py", &T_Gen_Elec_Py, &b_T_Gen_Elec_Py);
   fChain->SetBranchAddress("T_Gen_Elec_Pz", &T_Gen_Elec_Pz, &b_T_Gen_Elec_Pz);
   fChain->SetBranchAddress("T_Gen_Elec_Energy", &T_Gen_Elec_Energy, &b_T_Gen_Elec_Energy);
   fChain->SetBranchAddress("T_Gen_Elec_MPID", &T_Gen_Elec_MPID, &b_T_Gen_Elec_MPID);
   fChain->SetBranchAddress("T_Gen_Elec_MPx", &T_Gen_Elec_MPx, &b_T_Gen_Elec_MPx);
   fChain->SetBranchAddress("T_Gen_Elec_MPy", &T_Gen_Elec_MPy, &b_T_Gen_Elec_MPy);
   fChain->SetBranchAddress("T_Gen_Elec_MPz", &T_Gen_Elec_MPz, &b_T_Gen_Elec_MPz);
   fChain->SetBranchAddress("T_Gen_Elec_MEnergy", &T_Gen_Elec_MEnergy, &b_T_Gen_Elec_MEnergy);
   fChain->SetBranchAddress("T_Gen_Elec_MSt", &T_Gen_Elec_MSt, &b_T_Gen_Elec_MSt);
   fChain->SetBranchAddress("T_Gen_b_PID", &T_Gen_b_PID, &b_T_Gen_b_PID);
   fChain->SetBranchAddress("T_Gen_b_Px", &T_Gen_b_Px, &b_T_Gen_b_Px);
   fChain->SetBranchAddress("T_Gen_b_Py", &T_Gen_b_Py, &b_T_Gen_b_Py);
   fChain->SetBranchAddress("T_Gen_b_Pz", &T_Gen_b_Pz, &b_T_Gen_b_Pz);
   fChain->SetBranchAddress("T_Gen_b_Energy", &T_Gen_b_Energy, &b_T_Gen_b_Energy);
   fChain->SetBranchAddress("T_Gen_b_MPID", &T_Gen_b_MPID, &b_T_Gen_b_MPID);
   fChain->SetBranchAddress("T_Gen_b_MPx", &T_Gen_b_MPx, &b_T_Gen_b_MPx);
   fChain->SetBranchAddress("T_Gen_b_MPy", &T_Gen_b_MPy, &b_T_Gen_b_MPy);
   fChain->SetBranchAddress("T_Gen_b_MPz", &T_Gen_b_MPz, &b_T_Gen_b_MPz);
   fChain->SetBranchAddress("T_Gen_b_MEnergy", &T_Gen_b_MEnergy, &b_T_Gen_b_MEnergy);
   fChain->SetBranchAddress("T_Gen_b_MSt", &T_Gen_b_MSt, &b_T_Gen_b_MSt);
   fChain->SetBranchAddress("T_Gen_MuonSt3_PID", &T_Gen_MuonSt3_PID, &b_T_Gen_MuonSt3_PID);
   fChain->SetBranchAddress("T_Gen_MuonSt3_Px", &T_Gen_MuonSt3_Px, &b_T_Gen_MuonSt3_Px);
   fChain->SetBranchAddress("T_Gen_MuonSt3_Py", &T_Gen_MuonSt3_Py, &b_T_Gen_MuonSt3_Py);
   fChain->SetBranchAddress("T_Gen_MuonSt3_Pz", &T_Gen_MuonSt3_Pz, &b_T_Gen_MuonSt3_Pz);
   fChain->SetBranchAddress("T_Gen_MuonSt3_Energy", &T_Gen_MuonSt3_Energy, &b_T_Gen_MuonSt3_Energy);
   fChain->SetBranchAddress("T_Gen_ElecSt3_PID", &T_Gen_ElecSt3_PID, &b_T_Gen_ElecSt3_PID);
   fChain->SetBranchAddress("T_Gen_ElecSt3_Px", &T_Gen_ElecSt3_Px, &b_T_Gen_ElecSt3_Px);
   fChain->SetBranchAddress("T_Gen_ElecSt3_Py", &T_Gen_ElecSt3_Py, &b_T_Gen_ElecSt3_Py);
   fChain->SetBranchAddress("T_Gen_ElecSt3_Pz", &T_Gen_ElecSt3_Pz, &b_T_Gen_ElecSt3_Pz);
   fChain->SetBranchAddress("T_Gen_ElecSt3_Energy", &T_Gen_ElecSt3_Energy, &b_T_Gen_ElecSt3_Energy);
   fChain->SetBranchAddress("T_Gen_bSt3_PID", &T_Gen_bSt3_PID, &b_T_Gen_bSt3_PID);
   fChain->SetBranchAddress("T_Gen_bSt3_Px", &T_Gen_bSt3_Px, &b_T_Gen_bSt3_Px);
   fChain->SetBranchAddress("T_Gen_bSt3_Py", &T_Gen_bSt3_Py, &b_T_Gen_bSt3_Py);
   fChain->SetBranchAddress("T_Gen_bSt3_Pz", &T_Gen_bSt3_Pz, &b_T_Gen_bSt3_Pz);
   fChain->SetBranchAddress("T_Gen_bSt3_Energy", &T_Gen_bSt3_Energy, &b_T_Gen_bSt3_Energy);
   fChain->SetBranchAddress("T_Gen_TauSt3_PID", &T_Gen_TauSt3_PID, &b_T_Gen_TauSt3_PID);
   fChain->SetBranchAddress("T_Gen_TauSt3_Px", &T_Gen_TauSt3_Px, &b_T_Gen_TauSt3_Px);
   fChain->SetBranchAddress("T_Gen_TauSt3_Py", &T_Gen_TauSt3_Py, &b_T_Gen_TauSt3_Py);
   fChain->SetBranchAddress("T_Gen_TauSt3_Pz", &T_Gen_TauSt3_Pz, &b_T_Gen_TauSt3_Pz);
   fChain->SetBranchAddress("T_Gen_TauSt3_Energy", &T_Gen_TauSt3_Energy, &b_T_Gen_TauSt3_Energy);
   fChain->SetBranchAddress("T_Gen_TauSt3_IsLepDec", &T_Gen_TauSt3_IsLepDec, &b_T_Gen_TauSt3_IsLepDec);
   fChain->SetBranchAddress("T_Gen_TauSt3_LepDec_PID", &T_Gen_TauSt3_LepDec_PID, &b_T_Gen_TauSt3_LepDec_PID);
   fChain->SetBranchAddress("T_Gen_TauSt3_LepDec_Px", &T_Gen_TauSt3_LepDec_Px, &b_T_Gen_TauSt3_LepDec_Px);
   fChain->SetBranchAddress("T_Gen_TauSt3_LepDec_Py", &T_Gen_TauSt3_LepDec_Py, &b_T_Gen_TauSt3_LepDec_Py);
   fChain->SetBranchAddress("T_Gen_TauSt3_LepDec_Pz", &T_Gen_TauSt3_LepDec_Pz, &b_T_Gen_TauSt3_LepDec_Pz);
   fChain->SetBranchAddress("T_Gen_TauSt3_LepDec_Energy", &T_Gen_TauSt3_LepDec_Energy, &b_T_Gen_TauSt3_LepDec_Energy);
   fChain->SetBranchAddress("T_PFreducedCand_Px", &T_PFreducedCand_Px, &b_T_PFreducedCand_Px);
   fChain->SetBranchAddress("T_PFreducedCand_Py", &T_PFreducedCand_Py, &b_T_PFreducedCand_Py);
   fChain->SetBranchAddress("T_PFreducedCand_Pz", &T_PFreducedCand_Pz, &b_T_PFreducedCand_Pz);
   fChain->SetBranchAddress("T_PFreducedCand_E", &T_PFreducedCand_E, &b_T_PFreducedCand_E);
   fChain->SetBranchAddress("T_PFreducedCand_ID", &T_PFreducedCand_ID, &b_T_PFreducedCand_ID);
   fChain->SetBranchAddress("T_PFreducedCand_vz", &T_PFreducedCand_vz, &b_T_PFreducedCand_vz);
   fChain->SetBranchAddress("T_Muon_Eta", &T_Muon_Eta, &b_T_Muon_Eta);
   fChain->SetBranchAddress("T_Muon_IsGlobalMuon", &T_Muon_IsGlobalMuon, &b_T_Muon_IsGlobalMuon);
   fChain->SetBranchAddress("T_Muon_IsAllStandAloneMuons", &T_Muon_IsAllStandAloneMuons, &b_T_Muon_IsAllStandAloneMuons);
   fChain->SetBranchAddress("T_Muon_IsAllTrackerMuons", &T_Muon_IsAllTrackerMuons, &b_T_Muon_IsAllTrackerMuons);
   fChain->SetBranchAddress("T_Muon_IsTrackerMuonArbitrated", &T_Muon_IsTrackerMuonArbitrated, &b_T_Muon_IsTrackerMuonArbitrated);
   fChain->SetBranchAddress("T_Muon_IsAllArbitrated", &T_Muon_IsAllArbitrated, &b_T_Muon_IsAllArbitrated);
   fChain->SetBranchAddress("T_Muon_IsTMLastStationLoose", &T_Muon_IsTMLastStationLoose, &b_T_Muon_IsTMLastStationLoose);
   fChain->SetBranchAddress("T_Muon_IsTMLastStationTight", &T_Muon_IsTMLastStationTight, &b_T_Muon_IsTMLastStationTight);
   fChain->SetBranchAddress("T_Muon_IsTM2DCompatibilityLoose", &T_Muon_IsTM2DCompatibilityLoose, &b_T_Muon_IsTM2DCompatibilityLoose);
   fChain->SetBranchAddress("T_Muon_IsTM2DCompatibilityTight", &T_Muon_IsTM2DCompatibilityTight, &b_T_Muon_IsTM2DCompatibilityTight);
   fChain->SetBranchAddress("T_Muon_IsTMOneStationLoose", &T_Muon_IsTMOneStationLoose, &b_T_Muon_IsTMOneStationLoose);
   fChain->SetBranchAddress("T_Muon_IsTMOneStationTight", &T_Muon_IsTMOneStationTight, &b_T_Muon_IsTMOneStationTight);
   fChain->SetBranchAddress("T_Muon_IsTMLSOPL", &T_Muon_IsTMLSOPL, &b_T_Muon_IsTMLSOPL);
   fChain->SetBranchAddress("T_Muon_IsGMTkChiCompatibility", &T_Muon_IsGMTkChiCompatibility, &b_T_Muon_IsGMTkChiCompatibility);
   fChain->SetBranchAddress("T_Muon_IsGMStaChiCompatibility", &T_Muon_IsGMStaChiCompatibility, &b_T_Muon_IsGMStaChiCompatibility);
   fChain->SetBranchAddress("T_Muon_IsGMTkKinkTight", &T_Muon_IsGMTkKinkTight, &b_T_Muon_IsGMTkKinkTight);
   fChain->SetBranchAddress("T_Muon_IsTMLastStationAngLoose", &T_Muon_IsTMLastStationAngLoose, &b_T_Muon_IsTMLastStationAngLoose);
   fChain->SetBranchAddress("T_Muon_IsTMLastStationAngTight", &T_Muon_IsTMLastStationAngTight, &b_T_Muon_IsTMLastStationAngTight);
   fChain->SetBranchAddress("T_Muon_IsTMOneStationAngLoose", &T_Muon_IsTMOneStationAngLoose, &b_T_Muon_IsTMOneStationAngLoose);
   fChain->SetBranchAddress("T_Muon_IsTMOneStationAngTight", &T_Muon_IsTMOneStationAngTight, &b_T_Muon_IsTMOneStationAngTight);
   fChain->SetBranchAddress("T_Muon_IsGMPTMuons", &T_Muon_IsGMPTMuons, &b_T_Muon_IsGMPTMuons);
   fChain->SetBranchAddress("T_Muon_SegmentCompatibility", &T_Muon_SegmentCompatibility, &b_T_Muon_SegmentCompatibility);
   fChain->SetBranchAddress("T_Muon_trkKink", &T_Muon_trkKink, &b_T_Muon_trkKink);
   fChain->SetBranchAddress("T_Muon_Px", &T_Muon_Px, &b_T_Muon_Px);
   fChain->SetBranchAddress("T_Muon_Py", &T_Muon_Py, &b_T_Muon_Py);
   fChain->SetBranchAddress("T_Muon_Pz", &T_Muon_Pz, &b_T_Muon_Pz);
   fChain->SetBranchAddress("T_Muon_Pt", &T_Muon_Pt, &b_T_Muon_Pt);
   fChain->SetBranchAddress("T_Muon_MVARings", &T_Muon_MVARings, &b_T_Muon_MVARings);
   fChain->SetBranchAddress("T_Muon_MVAID", &T_Muon_MVAID, &b_T_Muon_MVAID);
   fChain->SetBranchAddress("T_Muon_deltaPt", &T_Muon_deltaPt, &b_T_Muon_deltaPt);
   fChain->SetBranchAddress("T_Muon_Energy", &T_Muon_Energy, &b_T_Muon_Energy);
   fChain->SetBranchAddress("T_Muon_Charge", &T_Muon_Charge, &b_T_Muon_Charge);
   fChain->SetBranchAddress("T_Muon_NormChi2GTrk", &T_Muon_NormChi2GTrk, &b_T_Muon_NormChi2GTrk);
   fChain->SetBranchAddress("T_Muon_NValidHitsInTrk", &T_Muon_NValidHitsInTrk, &b_T_Muon_NValidHitsInTrk);
   fChain->SetBranchAddress("T_Muon_NValidPixelHitsInTrk", &T_Muon_NValidPixelHitsInTrk, &b_T_Muon_NValidPixelHitsInTrk);
   fChain->SetBranchAddress("T_Muon_InnerTrackFound", &T_Muon_InnerTrackFound, &b_T_Muon_InnerTrackFound);
   fChain->SetBranchAddress("T_Muon_NValidHitsSATrk", &T_Muon_NValidHitsSATrk, &b_T_Muon_NValidHitsSATrk);
   fChain->SetBranchAddress("T_Muon_NValidHitsGTrk", &T_Muon_NValidHitsGTrk, &b_T_Muon_NValidHitsGTrk);
   fChain->SetBranchAddress("T_Muon_Chi2InTrk", &T_Muon_Chi2InTrk, &b_T_Muon_Chi2InTrk);
   fChain->SetBranchAddress("T_Muon_dofInTrk", &T_Muon_dofInTrk, &b_T_Muon_dofInTrk);
   fChain->SetBranchAddress("T_Muon_IPAbsGTrack", &T_Muon_IPAbsGTrack, &b_T_Muon_IPAbsGTrack);
   fChain->SetBranchAddress("T_Muon_IPAbsInTrack", &T_Muon_IPAbsInTrack, &b_T_Muon_IPAbsInTrack);
   fChain->SetBranchAddress("T_Muon_IPwrtAveBSInTrack", &T_Muon_IPwrtAveBSInTrack, &b_T_Muon_IPwrtAveBSInTrack);
   fChain->SetBranchAddress("T_Muon_IP2DBiasedPV", &T_Muon_IP2DBiasedPV, &b_T_Muon_IP2DBiasedPV);
   fChain->SetBranchAddress("T_Muon_dxyPVBiasedPV", &T_Muon_dxyPVBiasedPV, &b_T_Muon_dxyPVBiasedPV);
   fChain->SetBranchAddress("T_Muon_dzPVBiasedPV", &T_Muon_dzPVBiasedPV, &b_T_Muon_dzPVBiasedPV);
   fChain->SetBranchAddress("T_Muon_chargedHadronIsoR03", &T_Muon_chargedHadronIsoR03, &b_T_Muon_chargedHadronIsoR03);
   fChain->SetBranchAddress("T_Muon_neutralHadronIsoR03", &T_Muon_neutralHadronIsoR03, &b_T_Muon_neutralHadronIsoR03);
   fChain->SetBranchAddress("T_Muon_photonIsoR03", &T_Muon_photonIsoR03, &b_T_Muon_photonIsoR03);
   fChain->SetBranchAddress("T_Muon_chargedHadronIsoR04", &T_Muon_chargedHadronIsoR04, &b_T_Muon_chargedHadronIsoR04);
   fChain->SetBranchAddress("T_Muon_neutralHadronIsoR04", &T_Muon_neutralHadronIsoR04, &b_T_Muon_neutralHadronIsoR04);
   fChain->SetBranchAddress("T_Muon_photonIsoR04", &T_Muon_photonIsoR04, &b_T_Muon_photonIsoR04);
   fChain->SetBranchAddress("T_Muon_sumPUPtR04", &T_Muon_sumPUPtR04, &b_T_Muon_sumPUPtR04);
   fChain->SetBranchAddress("T_Muon_sumPUPtR03", &T_Muon_sumPUPtR03, &b_T_Muon_sumPUPtR03);
   fChain->SetBranchAddress("T_Muon_vz", &T_Muon_vz, &b_T_Muon_vz);
   fChain->SetBranchAddress("T_Muon_vy", &T_Muon_vy, &b_T_Muon_vy);
   fChain->SetBranchAddress("T_Muon_vx", &T_Muon_vx, &b_T_Muon_vx);
   fChain->SetBranchAddress("T_Muon_NumOfMatches", &T_Muon_NumOfMatches, &b_T_Muon_NumOfMatches);
   fChain->SetBranchAddress("T_Muon_EcalVeto", &T_Muon_EcalVeto, &b_T_Muon_EcalVeto);
   fChain->SetBranchAddress("T_Muon_HcalVeto", &T_Muon_HcalVeto, &b_T_Muon_HcalVeto);
   fChain->SetBranchAddress("T_Muon_PFMuonPt", &T_Muon_PFMuonPt, &b_T_Muon_PFMuonPt);
   fChain->SetBranchAddress("T_Muon_PFMuonPx", &T_Muon_PFMuonPx, &b_T_Muon_PFMuonPx);
   fChain->SetBranchAddress("T_Muon_PFMuonPy", &T_Muon_PFMuonPy, &b_T_Muon_PFMuonPy);
   fChain->SetBranchAddress("T_Muon_PFMuonPz", &T_Muon_PFMuonPz, &b_T_Muon_PFMuonPz);
   fChain->SetBranchAddress("T_Muon_PFMuonE", &T_Muon_PFMuonE, &b_T_Muon_PFMuonE);
   fChain->SetBranchAddress("T_Muon_isPFMuon", &T_Muon_isPFMuon, &b_T_Muon_isPFMuon);
   fChain->SetBranchAddress("T_Muon_NLayers", &T_Muon_NLayers, &b_T_Muon_NLayers);
   fChain->SetBranchAddress("T_Vertex_z", &T_Vertex_z, &b_T_Vertex_z);
   fChain->SetBranchAddress("T_Vertex_y", &T_Vertex_y, &b_T_Vertex_y);
   fChain->SetBranchAddress("T_Vertex_x", &T_Vertex_x, &b_T_Vertex_x);
   fChain->SetBranchAddress("T_Vertex_Chi2Prob", &T_Vertex_Chi2Prob, &b_T_Vertex_Chi2Prob);
   fChain->SetBranchAddress("T_Vertex_rho", &T_Vertex_rho, &b_T_Vertex_rho);
   fChain->SetBranchAddress("T_Vertex_ndof", &T_Vertex_ndof, &b_T_Vertex_ndof);
   fChain->SetBranchAddress("T_Vertex_isFake", &T_Vertex_isFake, &b_T_Vertex_isFake);
   fChain->SetBranchAddress("T_Vertex_tracksSize", &T_Vertex_tracksSize, &b_T_Vertex_tracksSize);
   fChain->SetBranchAddress("T_JetAKCHS_Px", &T_JetAKCHS_Px, &b_T_JetAKCHS_Px);
   fChain->SetBranchAddress("T_JetAKCHS_Py", &T_JetAKCHS_Py, &b_T_JetAKCHS_Py);
   fChain->SetBranchAddress("T_JetAKCHS_Pz", &T_JetAKCHS_Pz, &b_T_JetAKCHS_Pz);
   fChain->SetBranchAddress("T_JetAKCHS_Et", &T_JetAKCHS_Et, &b_T_JetAKCHS_Et);
   fChain->SetBranchAddress("T_JetAKCHS_EtOffset", &T_JetAKCHS_EtOffset, &b_T_JetAKCHS_EtOffset);
   fChain->SetBranchAddress("T_JetAKCHS_PtOffset", &T_JetAKCHS_PtOffset, &b_T_JetAKCHS_PtOffset);
   fChain->SetBranchAddress("T_JetAKCHS_Eta", &T_JetAKCHS_Eta, &b_T_JetAKCHS_Eta);
   fChain->SetBranchAddress("T_JetAKCHS_Energy", &T_JetAKCHS_Energy, &b_T_JetAKCHS_Energy);
   fChain->SetBranchAddress("T_JetAKCHS_Corr", &T_JetAKCHS_Corr, &b_T_JetAKCHS_Corr);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_HighEffTC", &T_JetAKCHS_Tag_HighEffTC, &b_T_JetAKCHS_Tag_HighEffTC);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_CombSVtx", &T_JetAKCHS_Tag_CombSVtx, &b_T_JetAKCHS_Tag_CombSVtx);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_CombSVtxMVA", &T_JetAKCHS_Tag_CombSVtxMVA, &b_T_JetAKCHS_Tag_CombSVtxMVA);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_TauJet", &T_JetAKCHS_Tag_TauJet, &b_T_JetAKCHS_Tag_TauJet);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_ImpParMVA", &T_JetAKCHS_Tag_ImpParMVA, &b_T_JetAKCHS_Tag_ImpParMVA);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_JetBProb", &T_JetAKCHS_Tag_JetBProb, &b_T_JetAKCHS_Tag_JetBProb);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_JetProb", &T_JetAKCHS_Tag_JetProb, &b_T_JetAKCHS_Tag_JetProb);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_HighEffSimpSVtx", &T_JetAKCHS_Tag_HighEffSimpSVtx, &b_T_JetAKCHS_Tag_HighEffSimpSVtx);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_HighPurSimpSVtx", &T_JetAKCHS_Tag_HighPurSimpSVtx, &b_T_JetAKCHS_Tag_HighPurSimpSVtx);
   fChain->SetBranchAddress("T_JetAKCHS_Tag_HighPurTC", &T_JetAKCHS_Tag_HighPurTC, &b_T_JetAKCHS_Tag_HighPurTC);
   fChain->SetBranchAddress("T_JetAKCHS_Parton_Px", &T_JetAKCHS_Parton_Px, &b_T_JetAKCHS_Parton_Px);
   fChain->SetBranchAddress("T_JetAKCHS_Parton_Py", &T_JetAKCHS_Parton_Py, &b_T_JetAKCHS_Parton_Py);
   fChain->SetBranchAddress("T_JetAKCHS_Parton_Pz", &T_JetAKCHS_Parton_Pz, &b_T_JetAKCHS_Parton_Pz);
   fChain->SetBranchAddress("T_JetAKCHS_Parton_Energy", &T_JetAKCHS_Parton_Energy, &b_T_JetAKCHS_Parton_Energy);
   fChain->SetBranchAddress("T_JetAKCHS_Parton_Flavour", &T_JetAKCHS_Parton_Flavour, &b_T_JetAKCHS_Parton_Flavour);
   fChain->SetBranchAddress("T_JetAKCHS_CharHadEnergyFrac", &T_JetAKCHS_CharHadEnergyFrac, &b_T_JetAKCHS_CharHadEnergyFrac);
   fChain->SetBranchAddress("T_JetAKCHS_NeutHadEnergyFrac", &T_JetAKCHS_NeutHadEnergyFrac, &b_T_JetAKCHS_NeutHadEnergyFrac);
   fChain->SetBranchAddress("T_JetAKCHS_CharEmEnergyFrac", &T_JetAKCHS_CharEmEnergyFrac, &b_T_JetAKCHS_CharEmEnergyFrac);
   fChain->SetBranchAddress("T_JetAKCHS_NeutEmEnergyFrac", &T_JetAKCHS_NeutEmEnergyFrac, &b_T_JetAKCHS_NeutEmEnergyFrac);
   fChain->SetBranchAddress("T_JetAKCHS_CharHadEnergy", &T_JetAKCHS_CharHadEnergy, &b_T_JetAKCHS_CharHadEnergy);
   fChain->SetBranchAddress("T_JetAKCHS_NeutHadEnergy", &T_JetAKCHS_NeutHadEnergy, &b_T_JetAKCHS_NeutHadEnergy);
   fChain->SetBranchAddress("T_JetAKCHS_CharEmEnergy", &T_JetAKCHS_CharEmEnergy, &b_T_JetAKCHS_CharEmEnergy);
   fChain->SetBranchAddress("T_JetAKCHS_NeutEmEnergy", &T_JetAKCHS_NeutEmEnergy, &b_T_JetAKCHS_NeutEmEnergy);
   fChain->SetBranchAddress("T_JetAKCHS_MuonMultiplicity", &T_JetAKCHS_MuonMultiplicity, &b_T_JetAKCHS_MuonMultiplicity);
   fChain->SetBranchAddress("T_JetAKCHS_NeutralMultiplicity", &T_JetAKCHS_NeutralMultiplicity, &b_T_JetAKCHS_NeutralMultiplicity);
   fChain->SetBranchAddress("T_JetAKCHS_ChargedMultiplicity", &T_JetAKCHS_ChargedMultiplicity, &b_T_JetAKCHS_ChargedMultiplicity);
   fChain->SetBranchAddress("T_JetAKCHS_IDLoose", &T_JetAKCHS_IDLoose, &b_T_JetAKCHS_IDLoose);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_InvisibleE", &T_JetAKCHS_GenJet_InvisibleE, &b_T_JetAKCHS_GenJet_InvisibleE);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_Px", &T_JetAKCHS_GenJet_Px, &b_T_JetAKCHS_GenJet_Px);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_Py", &T_JetAKCHS_GenJet_Py, &b_T_JetAKCHS_GenJet_Py);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_Pz", &T_JetAKCHS_GenJet_Pz, &b_T_JetAKCHS_GenJet_Pz);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_Et", &T_JetAKCHS_GenJet_Et, &b_T_JetAKCHS_GenJet_Et);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_Eta", &T_JetAKCHS_GenJet_Eta, &b_T_JetAKCHS_GenJet_Eta);
   fChain->SetBranchAddress("T_JetAKCHS_GenJet_Energy", &T_JetAKCHS_GenJet_Energy, &b_T_JetAKCHS_GenJet_Energy);
   fChain->SetBranchAddress("T_JetAKCHS_IsGenJet", &T_JetAKCHS_IsGenJet, &b_T_JetAKCHS_IsGenJet);
   fChain->SetBranchAddress("T_METPF_ET", &T_METPF_ET, &b_T_METPF_ET);
   fChain->SetBranchAddress("T_METPF_Phi", &T_METPF_Phi, &b_T_METPF_Phi);
   fChain->SetBranchAddress("T_METPF_Sig", &T_METPF_Sig, &b_T_METPF_Sig);
   fChain->SetBranchAddress("T_METgen_ET", &T_METgen_ET, &b_T_METgen_ET);
   fChain->SetBranchAddress("T_METgen_Phi", &T_METgen_Phi, &b_T_METgen_Phi);
   fChain->SetBranchAddress("T_passTriggerDoubleMu", &T_passTriggerDoubleMu, &b_T_passTriggerDoubleMu);
   fChain->SetBranchAddress("T_passTriggerDoubleEl", &T_passTriggerDoubleEl, &b_T_passTriggerDoubleEl);
   fChain->SetBranchAddress("T_passTriggerSingleMu", &T_passTriggerSingleMu, &b_T_passTriggerSingleMu);
   fChain->SetBranchAddress("T_passTriggerElMu", &T_passTriggerElMu, &b_T_passTriggerElMu);
   Notify();
}

Bool_t MuonFakeRate::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MuonFakeRate::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MuonFakeRate::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MuonFakeRate_cxx
