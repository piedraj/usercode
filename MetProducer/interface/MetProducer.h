#ifndef MYPRODUCERS_METPRODUCER_METPRODUCER_H
#define MYPRODUCERS_METPRODUCER_METPRODUCER_H


#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "Math/VectorUtil.h"


#include <memory>
#include <vector> 


class MetProducer : public edm::EDProducer {
 public:
  explicit MetProducer(const edm::ParameterSet&);
  ~MetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  reco::MET calcChMET(edm::Handle<reco::CandidateView> candsH);
  reco::MET calcMET  (edm::Handle<reco::CandidateView> candsH,
		      float_t minNeuPt,
		      float_t maxEta);

  edm::InputTag pfMetTag_;
  edm::InputTag pfNoPuCandsTag_;
};


#endif
