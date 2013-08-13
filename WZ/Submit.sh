#!/bin/bash


# Working directory. From where root should be invoked
WORKINGDIR=`pwd`


SYSTEMATIC=$1
MODE=$2
NJET=$3


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


create_job() {
    local sample=$1
    local systemat=$2
    local datamode=$3
    local njets=$4
    local filename=job_${sample}_${systemat}_${datamode}_${njets}.sh
    cat > $filename <<EOF
#!/bin/bash
source /nfs/fanae/root_releases/root.5.34.09/bin/thisroot.sh
cd $WORKINGDIR
root -l -b -q "runAnalysisWZ.C(\"${SAMPLE}\",${SYSTEMATIC},${MODE},${NJET},\"20\",\"35\")";
EOF
    chmod u+x $filename
}


submit_job() {
    local sample=$1
    local systemat=$2
    local datamode=$3
    local njets=$4
    local filename=job_${sample}_${systemat}_${datamode}_${njets}.sh
    qsub $filename
}


for SAMPLE in ${DATA_SAMPLES}; do
    create_job $SAMPLE $SYSTEMATIC $MODE $NJET
    submit_job $SAMPLE $SYSTEMATIC $MODE $NJET
done


if [ $SYSTEMATIC != 8 ]; then

    for SAMPLE in ${MC_SAMPLES}; do
	create_job $SAMPLE $SYSTEMATIC $MODE $NJET
	submit_job $SAMPLE $SYSTEMATIC $MODE $NJET
    done
fi