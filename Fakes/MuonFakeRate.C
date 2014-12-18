#define MuonFakeRate_cxx

#include "MuonFakeRate.h"
#include <TFile.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <iostream>


const Int_t nptbin  = 7;
const Int_t netabin = 4;

const Float_t ptedges [nptbin+1]  = {10, 15, 20, 25, 30, 35, 40, 50};  // [GeV]
const Float_t etaedges[netabin+1] = {0.0, 1.0, 1.479, 2.0, 2.5};


enum {Loose, Tight};

struct Lepton
{
  UInt_t         index;
  UInt_t         type;
  Int_t          charge;
  Float_t        mva;
  TLorentzVector v;
};

vector<Lepton>         Muons;
vector<TLorentzVector> Jets;


// Declare output
//------------------------------------------------------------------------------
const UInt_t njetet = 20;

TFile* output;

TH1F* h_mva_loose[njetet];
TH1F* h_mva_tight[njetet];

TH2F* h_pt_eta_loose[njetet];
TH2F* h_pt_eta_tight[njetet];


//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------
void MuonFakeRate::Loop()
{
  if (fChain == 0) return;


  // Define output
  //----------------------------------------------------------------------------
  gSystem->mkdir("rootfiles", kTRUE);

  output = new TFile("rootfiles/muonFakeRate.root", "recreate");
   
  TH1::SetDefaultSumw2();

  for (UInt_t i=0; i<njetet; i++) {

    Float_t jetet = 5. * (i + 1);

    h_mva_loose[i] = new TH1F(Form("h_mva_loose_%.0f", jetet), "", 50, -1, 1);
    h_mva_tight[i] = new TH1F(Form("h_mva_tight_%.0f", jetet), "", 50, -1, 1);

    h_pt_eta_loose[i] = new TH2F(Form("h_pt_eta_loose_%.0f", jetet), "", nptbin, ptedges, netabin, etaedges);  
    h_pt_eta_tight[i] = new TH2F(Form("h_pt_eta_tight_%.0f", jetet), "", nptbin, ptedges, netabin, etaedges);
  }


  // Prepare the entries and loop
  //----------------------------------------------------------------------------
  Long64_t nentries = fChain->GetEntriesFast();

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);

    if (ientry < 0) break;

    fChain->GetEntry(jentry);

    if (jentry%10000 == 0) std::cout << "." << std::flush;


    // Accept events with at least one muon and one jet
    //--------------------------------------------------------------------------
    if (T_Muon_Px->size()     < 1) continue;
    if (T_JetAKCHS_Px->size() < 1) continue;


    // Loop over muons
    //--------------------------------------------------------------------------
    Muons.clear();

    Float_t leadingMuonPt = -1;
    Int_t   leadingMuon   = -1;
  
    for (UInt_t i=0; i<T_Muon_Px->size(); i++) {
      
      if (T_Muon_Pt->at(i) <= 10.) continue;

      if (fabs(T_Muon_Eta->at(i)) >= 2.4) continue;
      
      Bool_t isStandAloneMuon = (T_Muon_IsAllStandAloneMuons->at(i) &&
				 !T_Muon_IsGlobalMuon->at(i) &&
				 !T_Muon_IsAllTrackerMuons->at(i));

      Bool_t isGlobalMuon = ((T_Muon_IsGlobalMuon->at(i) &&
			      T_Muon_NValidHitsSATrk->at(i) > 0 &&
			      T_Muon_NormChi2GTrk->at(i) < 10 &&
			      T_Muon_NumOfMatches->at(i) > 1) ||
			     (T_Muon_IsAllTrackerMuons->at(i) &&
			      T_Muon_IsTMLastStationTight->at(i)));

      Bool_t isLooseMuon = (!isStandAloneMuon &&
			    isGlobalMuon &&
			    T_Muon_isPFMuon->at(i) &&
			    T_Muon_trkKink->at(i) < 20 &&
			    T_Muon_NLayers->at(i) > 5 &&
			    T_Muon_NValidPixelHitsInTrk->at(i) > 0 &&
			    (T_Muon_deltaPt->at(i) / T_Muon_Pt->at(i)) < 0.1 &&
			    fabs(T_Muon_dzPVBiasedPV->at(i)) < 0.1 &&
			    fabs(T_Muon_IP2DBiasedPV->at(i)) < 0.2 &&
			    T_Muon_MVARings->at(i) > -0.6);


      // Accept loose muons
      //------------------------------------------------------------------------
      if (!isLooseMuon) continue;


      // Select the leading muon
      //------------------------------------------------------------------------
      if (T_Muon_Pt->at(i) > leadingMuonPt) {
	leadingMuonPt = T_Muon_Pt->at(i);
	leadingMuon   = i;
      }


      // Fill the loose muons vector
      //------------------------------------------------------------------------
      TLorentzVector tlv;

      tlv.SetPxPyPzE(T_Muon_Px->at(i),
		     T_Muon_Py->at(i),
		     T_Muon_Pz->at(i),
		     T_Muon_Energy->at(i));
      
      Lepton Lep;

      Lep.index = i;

      Lep.charge = T_Muon_Charge->at(i);

      Lep.v = tlv;

      Lep.mva = T_Muon_MVARings->at(i);

      Bool_t isTightMuon = isLooseMuon;

      isTightMuon &= ((tlv.Pt() <  20. && fabs(T_Muon_IP2DBiasedPV->at(i)) < 0.01) ||
		      (tlv.Pt() >= 20. && fabs(T_Muon_IP2DBiasedPV->at(i)) < 0.02));

      isTightMuon &= ((fabs(tlv.Eta()) <  1.479 && tlv.Pt() >  20. && T_Muon_MVARings->at(i) > 0.82) ||
		      (fabs(tlv.Eta()) >= 1.479 && tlv.Pt() >  20. && T_Muon_MVARings->at(i) > 0.86) ||
		      (fabs(tlv.Eta()) <  1.479 && tlv.Pt() <= 20. && T_Muon_MVARings->at(i) > 0.86) ||
		      (fabs(tlv.Eta()) >= 1.479 && tlv.Pt() <= 20. && T_Muon_MVARings->at(i) > 0.82));

      Lep.type = (isTightMuon) ? Tight : Loose;

      Muons.push_back(Lep);
    }


    // Accept events with at least one loose muon
    //--------------------------------------------------------------------------
    if (leadingMuonPt < 0) continue;


    // Loop over jets
    //--------------------------------------------------------------------------
    Jets.clear();

    Float_t leadingJetEt = -1;
    Int_t   leadingJet   = -1;

    for (UInt_t j=0; j<T_JetAKCHS_Px->size(); j++) {

      TLorentzVector tlv;

      tlv.SetPxPyPzE(T_JetAKCHS_Px->at(j),
		     T_JetAKCHS_Py->at(j),
		     T_JetAKCHS_Pz->at(j),
		     T_JetAKCHS_Energy->at(j));

      Jets.push_back(tlv);

      if (Jets[j].Et() > leadingJetEt) {
	leadingJetEt = Jets[j].Et();
	leadingJet   = j; 
      }
    }


    // Define the trigger weight
    //--------------------------------------------------------------------------
    Bool_t isMu8  = (T_HLT_Mu8_v16 && Muons[leadingMuon].v.Pt() > 10. && Muons[leadingMuon].v.Pt() <= 20.);
    Bool_t isMu17 = (T_HLT_Mu17_v3 && Muons[leadingMuon].v.Pt() > 20.);

    Float_t triggerW = 1; 
      
    if (isMu8)  triggerW = 1. /  1.986;  // Moriond13 JSON
    if (isMu17) triggerW = 1. / 24.132;  // Moriond13 JSON

    
    // Accept events that pass the trigger
   //--------------------------------------------------------------------------
    if (!isMu8 && !isMu17) continue;


    // Accept events out of the Z peak and above the B Physics phase space
    //--------------------------------------------------------------------------
    Bool_t IsZ    = false;
    Bool_t IsOnia = false;

    for (UInt_t i=0; i<Muons.size(); i++) {

      if (Muons[i].type != Tight) continue;

      for (UInt_t j=i+1; j<Muons.size(); j++) {

	if (Muons[i].charge * Muons[j].charge > 0) continue;

	Float_t mll = (Muons[i].v + Muons[j].v).M();

	if (mll >= 76. && mll <= 106.) IsZ    = true;
	if (mll <= 12.)                IsOnia = true;
      }
    }
    
    if (IsZ || IsOnia) continue;


    // Accept events with low MET
    //--------------------------------------------------------------------------
    if (T_METPF_ET >= 20.) continue;


    // Fill histograms
    //--------------------------------------------------------------------------
    for (UInt_t i=0; i<Muons.size(); i++) {

      if (Muons[i].v.DeltaR(Jets[leadingJet]) < 0.5) continue;

      Float_t px1 = Muons[i].v.Px();
      Float_t py1 = Muons[i].v.Py();
      Float_t pt1 = Muons[i].v.Pt();
	
      Float_t px2 = T_METPF_ET * cos(T_METPF_Phi);
      Float_t py2 = T_METPF_ET * sin(T_METPF_Phi);
      Float_t pt2 = T_METPF_ET;
    
      Float_t mt = (pt1+pt2)*(pt1+pt2) - (px1+px2)*(px1+px2) - (py1+py2)*(py1+py2);

      mt = sqrt(fabs(mt));
      
      if (mt >= 20.) continue;  // Reduce contamination from W (prompt) leptons


      // Fill histograms with different jet Et thresholds
      //------------------------------------------------------------------------
      for (UInt_t ijetet=0; ijetet<njetet; ijetet++) {

	Float_t jetet = 5. * (ijetet + 1);

	if (Jets[leadingJet].Et() <= jetet) continue;
      

	// Fill loose histograms
	//------------------------------------------------------------------------
	h_mva_loose[ijetet]->Fill(Muons[i].mva, triggerW);
	
	h_pt_eta_loose[ijetet]->Fill(Muons[i].v.Pt(), Muons[i].v.Eta(), triggerW);
	
	
	// Fill tight histograms
	//------------------------------------------------------------------------
	if (Muons[i].type == Loose) continue;

	h_mva_tight[ijetet]->Fill(Muons[i].mva, triggerW);
	    
	h_pt_eta_tight[ijetet]->Fill(Muons[i].v.Pt(), Muons[i].v.Eta(), triggerW);
      }
    }
  }


  printf("\n");


  // Save the histograms
  //----------------------------------------------------------------------------
  output->cd();

  for (UInt_t i=0; i<njetet; i++)
    {
      h_mva_loose[i]->Write();
      h_mva_tight[i]->Write();

      h_pt_eta_loose[i]->Write();
      h_pt_eta_tight[i]->Write();
    }

  output->Close();
}
