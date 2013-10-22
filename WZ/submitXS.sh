#!/bin/bash


if [ $# -lt 4 ]; then

    echo "  "
    echo "  ./submitXS.sh  CUT  MODE  WCHARGE  NJET"
    echo "  "
    echo "  CUT"
    echo "     0 = Exactly3Leptons"
    echo "     1 = InvMass3Lep100"
    echo "     2 = HasZ"
    echo "     3 = HasW"
    echo "     4 = MET30"
    echo "     5 = MtW40"
    echo "     6 = TighterCuts"
    echo "     7 = ZJetsRegion"
    echo "     8 = TopRegion"
    echo "     9 = VBFSelection"
    echo "  "
    echo "  MODE"
    echo "     0 = PPFmode"
    echo "     1 = MCmode"
    echo "  "
    echo "  WCHARGE"
    echo "     0 = WInclusive"
    echo "     1 = WPlus"
    echo "     2 = WMinus"
    echo "  "
    echo "  NJET"
    echo "    -1"
    echo "     0"
    echo "     1"
    echo "  "

    exit -1
fi


CUT=$1
MODE=$2
WCHARGE=$3
NJET=$4


# Working directory. From where root should be invoked
WORKINGDIR=`pwd`


create_and_submit_job() {
    local par1=$1
    local par2=$2
    local par3=$3
    local par4=$4
    local filename=job_XS_${par1}_${par2}_${par3}_${par4}.sh
    cat > $filename <<EOF
#!/bin/bash
source /nfs/fanae/root_releases/root.5.34.09/bin/thisroot.sh
cd $WORKINGDIR
root -l -b -q "runXS.C(${par1},${par2},${par3},${par4})";
EOF
    chmod u+x $filename
    qsub      $filename
}


create_and_submit_job $CUT $MODE $WCHARGE $NJET

