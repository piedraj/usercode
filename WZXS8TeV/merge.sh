#!/bin/bash


FOLDERS="                            \
CalibratedE/analysis                 \
CalibratedE/systematics/met          \
CalibratedE/systematics/muonUp       \
CalibratedE/systematics/muonDown     \
CalibratedE/systematics/electronUp   \
CalibratedE/systematics/electronDown \
"


for FOLDER in $FOLDERS; do

    pushd results/Summer12_53X/WH/$FOLDER

    if [ "$FOLDER" = "CalibratedE/analysis" ]; then

	hadd -f -k Data.root    \
	    DoubleElectron.root \
	    DoubleMu.root       \
	    MuEG.root

	hadd -f -k Data_PPF.root    \
	    DoubleElectron_PPF.root \
	    DoubleMu_PPF.root       \
	    MuEG_PPF.root
	
    fi

    hadd -f -k ZZ.root   \
	ggZZ4L.root      \
	ZZ2Mu2Tau.root   \
	ZZ4E.root        \
	ZZ2E2Tau.root    \
	ZZ4Mu.root       \
	ZZ2E2Mu.root     \
	HZZ4L.root
#       ZZ4Tau.root      \
#	ggZZ2L2L.root    \
#	ZZTo2L2QMad.root \

    hadd -f -k ZJets.root    \
	ZJets_Madgraph.root  \
	ZbbToLL.root
#	DYJets_Madgraph.root \

    hadd -f -k Top.root     \
	TTbar_Madgraph.root \
	TW.root             \
	TbarW.root

    hadd -f -k WV.root        \
	WGstarToMuNuMad.root
#	WW.root               \
#	WbbToLNu.root         \
#	WJets_Madgraph.root   \
#	WGstarToElNuMad.root  \
#	WGstarToTauNuMad.root \
#	WgammaToLNuG.root     \
#	WZTo2L2QMad.root      \
#	WZTo2QLNuMad.root

    hadd -f -k VVV.root \
	WZZJets.root    \
	WWZJets.root    \
	TTWJets.root    \
	TTZJets.root
#	WWWJets.root    \
#	ZZZJets.root    \
#	WWGJets.root    \
#	TTWWJets.root   \
#	TTGJets.root
    
    popd

done

