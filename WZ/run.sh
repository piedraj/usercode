#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./run.sh SYSTEMATIC"
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


DATA_SAMPLES="          \
102_DoubleElectron2012A \
112_DoubleElectron2012A \
202_DoubleElectron2012B \
307_DoubleElectron2012C \
312_DoubleElectron2012C \
322_DoubleElectron2012C \
332_DoubleElectron2012C \
342_DoubleElectron2012C \
352_DoubleElectron2012C \
402_DoubleElectron2012D \
103_DoubleMuon2012A     \
113_DoubleMuon2012A     \
203_DoubleMuon2012B     \
308_DoubleMuon2012C     \
313_DoubleMuon2012C     \
323_DoubleMuon2012C     \
333_DoubleMuon2012C     \
343_DoubleMuon2012C     \
353_DoubleMuon2012C     \
403_DoubleMuon2012D     \
104_MuEG2012A           \
114_MuEG2012A           \
204_MuEG2012B           \
309_MuEG2012C           \
314_MuEG2012C           \
324_MuEG2012C           \
334_MuEG2012C           \
344_MuEG2012C           \
354_MuEG2012C           \
404_MuEG2012D           \
"


MC_SAMPLES="             \
000_WWJets2LMad          \
001_GluGluToWWTo4L       \
008_GamGamWW             \
010_TTJetsMad            \
011_TtWFullDR            \
012_TbartWFullDR         \
036_DY10toLLMad          \
037_DY50toLLMad          \
051_ZbbToLL              \
064_ZZ2Mu2tau            \
065_ZZ4E                 \
066_ZZ2E2Tau             \
067_ZZ4Mu                \
068_ZZ2E2Mu              \
069_ZZ4Tau               \
072_GluGluZZ2L2L         \
073_GluGluZZ4L           \
074_WZJetsMad            \
075_ZZJetsMad            \
078_WZTo2L2QMad          \
079_ZZTo2L2QMad          \
080_WJetsToLNuMad        \
082_WGstarToElNuMad      \
083_WGstarToMuNuMad      \
084_WGstarToTauNuMad     \
085_WgammaToLNuG         \
086_ZgammaToLLuG         \
088_WWGJets              \
089_WZZJets              \
090_ZZZJets              \
091_WbbToLNu             \
091_WWZJets              \
092_WWWJets              \
093_TTWJets              \
094_TTZJets              \
095_TTWWJets             \
096_TTGJets              \
097_HZZ4L                \
"


if [ ${SYSTEMATIC} == 0 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do

        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},0)";

    done

elif [ ${SYSTEMATIC} == 8 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do

        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,15,15)";
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,15,35)";
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,15,50)";

        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,20,15)";
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,20,35)";  # Nominal one
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,20,50)";

        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,25,15)";
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,25,35)";
        root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},1,25,50)";

    done

else

    for SAMPLE in ${MC_SAMPLES}; do
    
	root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},0)";
	
    done
fi

