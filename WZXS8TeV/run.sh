#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./run.sh SYST CLOSURE"
    echo "  "
    echo "  nominal analysis with SYST = 0"
    echo "  met systematic   with SYST = 1"
    echo "  "
    exit -1
fi

SYST=$1
CLOSURE=$2


DATA_SAMPLES=" \
DoubleElectron \
DoubleMu       \
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
ZZ               \
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


if [ $SYST == 0 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do
	
	root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",0,$SYST,$CLOSURE)";
	root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",1,$SYST,$CLOSURE)";

    done
fi

for SAMPLE in ${MC_SAMPLES}; do
    
    root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",0,$SYST,$CLOSURE)";
    
done
