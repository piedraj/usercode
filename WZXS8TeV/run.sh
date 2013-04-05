#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./run.sh SYSTEMATIC"
    echo "  "
    echo "  nominal analysis with SYSTEMATIC = 0"
    echo "  met systematic   with SYSTEMATIC = 1"
    echo "  "
    exit -1
fi

SYSTEMATIC=$1


DATA_SAMPLES=" \
DoubleElectron \
DoubleMu       \
MuEG
"


MC_SAMPLES="     \
DYJets\_Madgraph \
ZJets\_Madgraph  \
ZbbToLL          \
WbbToLNu         \
WJets_Madgraph   \
WGstarToElNuMad  \
WGstarToMuNuMad  \
WGstarToTauNuMad \
WgammaToLNuG     \
ZgammaToLLG      \
TTbar_Madgraph   \
TW               \
TbarW            \
WW               \
WZTo3LNu         \
WZTo2L2QMad      \
WZTo2QLNuMad     \
ZZTo2L2QMad      \
ggZZ2L2L         \
ggZZ4L           \
ZZ2Mu2Tau        \
ZZ4E             \
ZZ2E2Tau         \
ZZ4Mu            \
ZZ2E2Mu          \
ZZ4Tau           \
HZZ4L            \
WWGJets          \
WZZJets          \
ZZZJets          \
WWZJets          \
WWWJets          \
TTWJets          \
TTZJets          \
TTWWJets         \
TTGJets          \
"


if [ ${SYSTEMATIC} == 0 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do
	
	root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",0,${SYSTEMATIC})";
	root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",1,${SYSTEMATIC})";

    done
fi

for SAMPLE in ${MC_SAMPLES}; do
    
    root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",0,${SYSTEMATIC})";
    
done

