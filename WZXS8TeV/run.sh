#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./run.sh MET_SYS CLOSURE"
    echo "  "
    exit -1
fi

MET_SYS=$1
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


for SAMPLE in ${DATA_SAMPLES}; do

    root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",0,${MET_SYS},${CLOSURE})";
    root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",1,${MET_SYS},${CLOSURE})";

done

for SAMPLE in ${MC_SAMPLES}; do

    root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",0,${MET_SYS},${CLOSURE})";
    
done
