#!/bin/bash


BASE_FOLDER="results-for-approval"


FOLDERS="                             \
$BASE_FOLDER/analysis                 \
$BASE_FOLDER/atlas                    \
$BASE_FOLDER/systematics/metUp        \
$BASE_FOLDER/systematics/metDown      \
$BASE_FOLDER/systematics/muonUp       \
$BASE_FOLDER/systematics/muonDown     \
$BASE_FOLDER/systematics/electronUp   \
$BASE_FOLDER/systematics/electronDown \
$BASE_FOLDER/systematics/pileup       \
"


for FOLDER in $FOLDERS; do
    
    pushd $FOLDER

    if [ "$FOLDER" = "$BASE_FOLDER/analysis" ] || [ "$FOLDER" = "$BASE_FOLDER/atlas" ] ; then

	hadd -f -k Data.root *2012?.root 
    fi

    hadd -f -k ZZ.root        \
	064_ZZ2Mu2tau.root    \
	065_ZZ4E.root         \
	066_ZZ2E2Tau.root     \
	067_ZZ4Mu.root        \
	068_ZZ2E2Mu.root      \
	069_ZZ4Tau.root       \
	072_GluGluZZ2L2L.root \
        073_GluGluZZ4L.root   \
        075_ZZJetsMad.root    \
        079_ZZTo2L2QMad.root  \
	097_HZZ4L.root

    hadd -f -k ZJets.root    \
	036_DY10toLLMad.root \
	037_DY50toLLMad.root \
	051_ZbbToLL.root

    hadd -f -k Top.root       \
	011_TtWFullDR.root    \
	012_TbartWFullDR.root \
	019_TTTo2L2Nu2B.root

    hadd -f -k WV.root            \
	000_WWJets2LMad.root      \
	001_GluGluToWWTo4L.root   \
	008_GamGamWW.root         \
	078_WZTo2L2QMad.root      \
	080_WJetsToLNuMad.root    \
	081_WbbToLNu.root         \
	082_WGstarToElNuMad.root  \
	083_WGstarToMuNuMad.root  \
	084_WGstarToTauNuMad.root \
	085_WgammaToLNuG.root

    hadd -f -k VVV.root   \
	088_WWGJets.root  \
	089_WZZJets.root  \
	090_ZZZJets.root  \
	091_WWZJets.root  \
	092_WWWJets.root  \
	093_TTWJets.root  \
	094_TTZJets.root  \
	095_TTWWJets.root \
	096_TTGJets.root
    
    popd

done


DATADRIVEN_FOLDERS="
$BASE_FOLDER/systematics/muonJet15_elecJet15 \
$BASE_FOLDER/systematics/muonJet15_elecJet35 \
$BASE_FOLDER/systematics/muonJet15_elecJet50 \
$BASE_FOLDER/systematics/muonJet20_elecJet15 \
$BASE_FOLDER/systematics/muonJet20_elecJet35 \
$BASE_FOLDER/systematics/muonJet20_elecJet50 \
$BASE_FOLDER/systematics/muonJet25_elecJet15 \
$BASE_FOLDER/systematics/muonJet25_elecJet35 \
$BASE_FOLDER/systematics/muonJet25_elecJet50 \
"


for FOLDER in $DATADRIVEN_FOLDERS; do
    
    pushd $FOLDER
    
    hadd -f -k Data_PPF.root *2012?_PPF.root

    popd

done

