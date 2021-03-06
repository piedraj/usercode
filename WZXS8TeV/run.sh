#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./run.sh SYSTEMATIC"
    echo "  "
    echo "  0 = noSyst"
    echo "  1 = metSyst"
    echo "  2 = muonUpSyst"
    echo "  3 = muonDownSyst"
    echo "  4 = electronUpSyst"
    echo "  5 = electronDownSyst"
    echo "  "
    exit -1
fi

SYSTEMATIC=$1


DATA_SAMPLES=" \
DoubleElectron \
DoubleMu       \
MuEG
"


#
# Main MC list
#
MC_SAMPLES="     \
ZJets\_Madgraph  \
ZbbToLL          \
WGstarToMuNuMad  \
ZgammaToLLG      \
TTbar_Madgraph   \
TW               \
TbarW            \
WZTo3LNu         \
ggZZ4L           \
ZZ2Mu2Tau        \
ZZ4E             \
ZZ2E2Tau         \
ZZ4Mu            \
ZZ2E2Mu          \
HZZ4L            \
WZZJets          \
WWZJets          \
TTWJets          \
TTZJets          \
"


#
# Full MC list
#
# MC_SAMPLES="     \
# DYJets\_Madgraph \
# ZJets\_Madgraph  \
# ZbbToLL          \
# WbbToLNu         \
# WJets_Madgraph   \
# WGstarToElNuMad  \
# WGstarToMuNuMad  \
# WGstarToTauNuMad \
# WgammaToLNuG     \
# ZgammaToLLG      \
# TTbar_Madgraph   \
# TW               \
# TbarW            \
# WW               \
# WZTo3LNu         \
# WZTo2L2QMad      \
# WZTo2QLNuMad     \
# ZZTo2L2QMad      \
# ggZZ2L2L         \
# ggZZ4L           \
# ZZ2Mu2Tau        \
# ZZ4E             \
# ZZ2E2Tau         \
# ZZ4Mu            \
# ZZ2E2Mu          \
# ZZ4Tau           \
# HZZ4L            \
# WWGJets          \
# WZZJets          \
# ZZZJets          \
# WWZJets          \
# WWWJets          \
# TTWJets          \
# TTZJets          \
# TTWWJets         \
# TTGJets          \
# "


if [ ${SYSTEMATIC} == 0 ]; then
    
    for SAMPLE in ${DATA_SAMPLES}; do
	
	./clean.sh; root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",${SYSTEMATIC},0)";

	./clean.sh; root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",${SYSTEMATIC},1)";

    done
fi

for SAMPLE in ${MC_SAMPLES}; do
    
    ./clean.sh; root -l -b -q "RunPROOF_WZ.C(\"${SAMPLE}\",${SYSTEMATIC},0)";
    
done

