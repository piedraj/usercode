#!/bin/bash


FOLDERS="                \
analysis                 \
systematics/met          \
systematics/muonUp       \
systematics/muonDown     \
systematics/electronUp   \
systematics/electronDown \
"


for FOLDER in $FOLDERS; do

    pushd results/Summer12_53X/WH/$FOLDER

    if [ "$FOLDER" = "analysis" ]; then

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
	ggZZ2L2L.root    \
	ggZZ4L.root      \
	ZZTo2L2QMad.root \
	ZZ2Mu2Tau.root   \
	ZZ4E.root        \
	ZZ2E2Tau.root    \
	ZZ4Mu.root       \
	ZZ2E2Mu.root     \
	ZZ4Tau.root      \
	HZZ4L.root

    hadd -f -k ZJets.root    \
	DYJets_Madgraph.root \
	ZJets_Madgraph.root  \
	ZbbToLL.root

    hadd -f -k Top.root     \
	TTbar_Madgraph.root \
	TW.root             \
	TbarW.root

    hadd -f -k WV.root        \
	WW.root               \
	WbbToLNu.root         \
	WJets_Madgraph.root   \
	WGstarToElNuMad.root  \
	WGstarToMuNuMad.root  \
	WGstarToTauNuMad.root \
	WgammaToLNuG.root     \
	WZTo2L2QMad.root      \
	WZTo2QLNuMad.root

    hadd -f -k VVV.root \
	WWGJets.root    \
	WZZJets.root    \
	ZZZJets.root    \
	WWZJets.root    \
	WWWJets.root    \
	TTWJets.root    \
	TTZJets.root    \
	TTWWJets.root   \
	TTGJets.root
    
    popd

done