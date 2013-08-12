#!/bin/bash

if [ $# -lt 2 ]; then

    echo "  "
    echo "  ./run.sh SYSTEMATIC NJET"
    echo "  "
    echo "  0 = noSyst"
    echo "  1 = metUpSyst"
    echo "  2 = metDownSyst"
    echo "  3 = muonUpSyst"
    echo "  4 = muonDownSyst"
    echo "  5 = electronUpSyst"
    echo "  6 = electronDownSyst"
    echo "  7 = pileupSyst"
    echo "  8 = fakesSyst"
    echo "  "

    exit -1
fi

SYSTEMATIC=$1
NJET=$2


DATA_SAMPLES="      \
Run2012A_889.4pbinv \
Run2012B_4417pbinv  \
Run2012C_7094pbinv  \
Run2012D_7308pbinv  \
"


MC_SAMPLES="         \
000_WWJets2LMad      \
001_GluGluToWWTo4L   \
008_GamGamWW         \
011_TtWFullDR        \
012_TbartWFullDR     \
019_TTTo2L2Nu2B      \
036_DY10toLLMad      \
037_DY50toLLMad      \
051_ZbbToLL          \
064_ZZ2Mu2tau        \
065_ZZ4E             \
066_ZZ2E2Tau         \
067_ZZ4Mu            \
068_ZZ2E2Mu          \
069_ZZ4Tau           \
072_GluGluZZ2L2L     \
073_GluGluZZ4L       \
074_WZJetsMad        \
075_ZZJetsMad        \
078_WZTo2L2QMad      \
079_ZZTo2L2QMad      \
080_WJetsToLNuMad    \
081_WbbToLNu         \
082_WGstarToElNuMad  \
083_WGstarToMuNuMad  \
084_WGstarToTauNuMad \
085_WgammaToLNuG     \
086_ZgammaToLLuG     \
088_WWGJets          \
089_WZZJets          \
090_ZZZJets          \
091_WWZJets          \
092_WWWJets          \
093_TTWJets          \
094_TTZJets          \
095_TTWWJets         \
096_TTGJets          \
097_HZZ4L            \
"


if [ ${SYSTEMATIC} == 0 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do

        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},0,${NJET})";  # Nominal selection
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},2,${NJET})";  # ATLAS data-driven

    done

    for SAMPLE in ${MC_SAMPLES}; do
    
	root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},0,${NJET})";  # Nominal selection
	root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},2,${NJET})";  # ATLAS data-driven
	
    done

elif [ ${SYSTEMATIC} == 8 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do

	for MUON_JET_PT in 05 10 15 20 25 30 35 40 45 50; do

	    for ELEC_JET_PT in 15 35 50; do
		
		root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,${NJET},\"${MUON_JET_PT}\",\"${ELEC_JET_PT}\")";

	    done

	done

    done

else

    for SAMPLE in ${MC_SAMPLES}; do
    
	root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},0,${NJET})";
	
    done
fi

