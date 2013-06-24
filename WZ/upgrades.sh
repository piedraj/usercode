#!/bin/bash


DIRECTORY="upgrades"


ZJETS_SAMPLES=" \
036_DY10toLLMad \
037_DY50toLLMad \
051_ZbbToLL     \
"


for SAMPLE in ${ZJETS_SAMPLES}; do
    
    root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",0,0,20,35,\"${DIRECTORY}\")";

    root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",0,1,20,35,\"${DIRECTORY}\")";

done


pushd ${DIRECTORY}/analysis

hadd -f -k ZJets.root    \
    036_DY10toLLMad.root \
    037_DY50toLLMad.root \
    051_ZbbToLL.root

hadd -f -k ZJets_PPF.root    \
    036_DY10toLLMad_PPF.root \
    037_DY50toLLMad_PPF.root \
    051_ZbbToLL_PPF.root

popd

