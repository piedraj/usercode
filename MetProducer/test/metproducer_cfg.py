################################################################################
#
# CMS
#
################################################################################
import FWCore.ParameterSet.Config as cms

process = cms.Process('OWNPARTICLES')

process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/p/piedra/public/test.root'))

process.out = cms.OutputModule('PoolOutputModule',
                               fileName = cms.untracked.string('test.root'))


################################################################################
#
# pfNoPilUp
#
################################################################################
process.load("CommonTools.ParticleFlow.pfNoPileUp_cff")

process.pfPileUp.PFCandidates       = 'particleFlow'
process.pfNoPileUp.bottomCollection = 'particleFlow'


################################################################################
#
# MetProducer
#
################################################################################
process.myProducer = cms.EDProducer('MetProducer',
                                    pfMetTag = cms.InputTag("pfMet"),
                                    pfNoPuCandsTag = cms.InputTag("pfNoPileUp")
)

  
################################################################################
#
# Path
#
################################################################################
process.p = cms.Path(process.pfNoPileUpSequence * process.myProducer)

process.e = cms.EndPath(process.out)
