2015/01/16

1. The code used by Javier Fernandez is located here,

/afs/cern.ch/user/j/jfernan2/public/TopAnalysis72X/

2. It requires CMSSW_7_2_0 (or later). With the default
SCRAM_ARCH in lxplus there is no such version,

echo $SCRAM_ARCH
slc6_amd64_gcc472

scram list | grep CMSSW_7 | grep -v afs | awk '{print $2}'
CMSSW_7_0_0_pre0
CMSSW_7_0_0_pre1
CMSSW_7_0_0_pre2

3. Set the architecture to slc6_amd64_gcc481

setenv SCRAM_ARCH slc6_amd64_gcc481

4. Get the CMSSW release,

cmsrel CMSSW_7_2_0
cd CMSSW_7_2_0/src/
cmsenv

5. Copy and compile the code,

git cms-merge-topic HuguesBrun:trigElecIdInCommonIsoSelection720
cp -r /afs/cern.ch/user/j/jfernan2/public/TopAnalysis72X/* .

scram b -j 10

6. Test the code,

voms-proxy-init

cmsRun skimToTreeSUSYMCtfs.py


### CRAB3 ###

2015/01/19

cmsenv
source /cvmfs/cms.cern.ch/crab3/crab.csh

voms-proxy-init

crab submit -c crabConfig.py

crab status --dir crab_TTJets_PU30bx50

