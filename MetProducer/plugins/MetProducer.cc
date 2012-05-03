#include "piedra/MetProducer/interface/MetProducer.h"


//------------------------------------------------------------------------------
// MetProducer
//------------------------------------------------------------------------------
MetProducer::MetProducer(const edm::ParameterSet& cfg)
{
  pfMetTag_       = cfg.getParameter<edm::InputTag>("pfMetTag"); 
  pfNoPuCandsTag_ = cfg.getParameter<edm::InputTag>("pfNoPuCandsTag");
}


//------------------------------------------------------------------------------
// produce
//------------------------------------------------------------------------------
void MetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<reco::PFMETCollection> pfMetH;
  iEvent.getByLabel(pfMetTag_, pfMetH);

  edm::Handle<reco::CandidateView> candsHPfNoPu;
  iEvent.getByLabel(pfNoPuCandsTag_,candsHPfNoPu);

  reco::PFMETRef pfMet         = reco::PFMETRef(pfMetH,0);
  reco::MET      chsChargedMet = calcChMET(candsHPfNoPu);
  reco::MET      chsMet        = calcMET(candsHPfNoPu, 4.0, 3.0);

  float_t pfMetPhi = (pfMet.isNonnull()) ? pfMet->phi() : -9999;
  float_t pfMetPt  = (pfMet.isNonnull()) ? pfMet->pt()  : -9999;

  float_t chsChargedMetPhi = chsChargedMet.phi();
  float_t chsChargedMetPt  = chsChargedMet.pt();

  float_t chsMetPhi = chsMet.phi();
  float_t chsMetPt  = chsMet.pt();

  std::cout << "\n         pfMet phi: " << pfMetPhi         << " \t pt: " << pfMetPt
	    << "\n chsChargedMet phi: " << chsChargedMetPhi << " \t pt: " << chsChargedMetPt
	    << "\n        chsMet phi: " << chsMetPhi        << " \t pt: " << chsMetPt
	    << "\n" << std::endl;
}


//------------------------------------------------------------------------------
// calcMET
//------------------------------------------------------------------------------
reco::MET MetProducer::calcMET(edm::Handle<reco::CandidateView> candsH,
			       float_t minNeuPt,
			       float_t maxEta)
{
  using namespace std;

  reco::Candidate::LorentzVector totalP4;

  for (reco::CandidateView::const_iterator it=candsH->begin(), ed=candsH->end();
      it!=ed; ++it) {

    if (it->charge() == 0 && (it->pt() < minNeuPt || fabs(it->eta()) > maxEta)) continue;

    totalP4 += it->p4();
  }
  
  reco::Candidate::LorentzVector invertedP4(-totalP4);

  reco::MET met(invertedP4, reco::Candidate::Point(0,0,0));

  return met;
}


//------------------------------------------------------------------------------
// calcChMET
//------------------------------------------------------------------------------
reco::MET MetProducer::calcChMET(edm::Handle<reco::CandidateView> candsH)
{
  using namespace std;

  reco::Candidate::LorentzVector totalP4;

  for (reco::CandidateView::const_iterator it=candsH->begin(), ed=candsH->end();
       it != ed; ++it) {

    if (it->charge() == 0) continue;

    totalP4 += it->p4();
  }

  reco::Candidate::LorentzVector invertedP4(-totalP4);

  reco::MET met(invertedP4, reco::Candidate::Point(0,0,0));

  return met;
}


MetProducer::~MetProducer() { } 
void MetProducer::beginJob() { }
void MetProducer::endJob() { }
DEFINE_FWK_MODULE(MetProducer);
