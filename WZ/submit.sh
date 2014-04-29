#!/bin/bash


if [ $# -lt 5 ]; then

    echo "  "
    echo "  ./submit.sh  SYSTEMATIC  MODE  JET_CHANNEL  MUON_JET_ET  ELEC_JET_ET"
    echo "  "
    echo "  SYSTEMATIC"
    echo "     0 = noSyst"
    echo "     1 = metUpSyst"
    echo "     2 = metDownSyst"
    echo "     3 = muonUpSyst"
    echo "     4 = muonDownSyst"
    echo "     5 = electronUpSyst"
    echo "     6 = electronDownSyst"
    echo "     7 = pileupSyst"
    echo "     8 = fakesSyst"
    echo "  "
    echo "  MODE"
    echo "     0 = nominal analysis"
    echo "     1 = data-driven estimation of the Z+jets and top backgrounds"
    echo "     2 = ATLAS method to determine the Z+jets and top backgrounds"
    echo "  "
    echo "  JET_CHANNEL"
    echo "    -1 = inclusive"
    echo "     0 = 0-jet bin"
    echo "     1 = 1-jet bin"
    echo "     2 = 2-jet bin"
    echo "  "
    echo "  MUON_JET_ET"
    echo "     05, 10, 15, 20, 25, 30, 35, 40, 45, 50"
    echo "  "
    echo "  ELEC_JET_ET"
    echo "     15, 35, 50"
    echo "  "

    exit -1
fi


SYSTEMATIC=$1
MODE=$2
JET_CHANNEL=$3
MUON_JET_ET=$4
ELEC_JET_ET=$5


# Working directory. From where root should be invoked
WORKINGDIR=`pwd`


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
074_WZJetsMad_step3NoFilter \
076_WZJetsMad_TuneZ2star    \
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
    local syst=$2
    local datamode=$3
    local njet=$4
    local muonjet=$5
    local elecjet=$6
    local filename=job_AnalysisWZ_${sample}_${syst}_${datamode}_${njet}_${muonjet}_${elecjet}.sh
    cat > $filename <<EOF
#!/bin/bash
source /nfs/fanae/root_releases/root.5.34.09/bin/thisroot.sh
cd $WORKINGDIR
root -l -b -q "runAnalysisWZ.C(\"${sample}\",${syst},${datamode},${njet},\"${muonjet}\",\"${elecjet}\")";
EOF
    chmod u+x $filename
}


submit_job() {
    local sample=$1
    local syst=$2
    local datamode=$3
    local njet=$4
    local muonjet=$5
    local elecjet=$6
    local filename=job_AnalysisWZ_${sample}_${syst}_${datamode}_${njet}_${muonjet}_${elecjet}.sh
    qsub $filename
}


if [ ${SYSTEMATIC} == 0 ]; then
    
    for SAMPLE in ${DATA_SAMPLES}; do
	create_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
	submit_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
    done

    for SAMPLE in ${MC_SAMPLES}; do
	create_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
	submit_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
    done

elif [ $SYSTEMATIC == 8 ]; then

    for SAMPLE in ${DATA_SAMPLES}; do
	create_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
	submit_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
    done

else

    for SAMPLE in ${MC_SAMPLES}; do
	create_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
	submit_job $SAMPLE $SYSTEMATIC $MODE ${JET_CHANNEL} ${MUON_JET_ET} ${ELEC_JET_ET}
    done

fi

