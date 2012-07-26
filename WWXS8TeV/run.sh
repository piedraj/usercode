if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./run.sh NJETS"
    echo "  "
    exit -1
fi


LUMINOSITY=5.064

NJETS=$1

CHANNELS=" All SF OF EE MuMu EMu MuE "

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
HWW125             \
"


rm -rf rootfiles/${NJETS}jet


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
