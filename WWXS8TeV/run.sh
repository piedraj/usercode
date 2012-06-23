LUMINOSITY=3.54

NJETS=0

CHANNELS=" All SF OF MuMu EE EMu MuE "

SAMPLES="          \
ggWWto2L           \
WWTo2L2Nu          \
WZ                 \
ZZ                 \
TTbar              \
TW                 \
DY                 \
WgammaStar         \
WgammaNoStar       \
WJetsFakes\_Total  \
DataRun2012\_Total \
DYtautau           \
"


rm -rf rootfiles


# Loop
for CHANNEL in $CHANNELS; do

    for SAMPLE in $SAMPLES; do 
	
	root -l -b -q "runLatinosTreeScript.C($LUMINOSITY,$NJETS,\"$CHANNEL\",\"$SAMPLE\")"
  
    done

    OUTPATH=rootfiles/${NJETS}jet/${CHANNEL}

    # Merge some MC files
    hadd ${OUTPATH}/WW.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu.root 
    hadd ${OUTPATH}/Wgamma.root ${OUTPATH}/WgammaNoStar.root ${OUTPATH}/WgammaStar.root

done
