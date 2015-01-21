Get ROOT
====

At `fanaeui.geol.uniovi.es`

    source /opt/root/bin/thisroot.sh

At `lorienmaster.irb.hr`

    export SCRAM_ARCH=slc5_amd64_gcc481
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    scram project CMSSW_7_0_0
    cd CMSSW_7_0_0/src
    cmsenv


Get the code
====

Read only

    git clone git://github.com/piedraj/usercode.git

Read and write

    git clone https://github.com/piedraj/usercode.git


Produce histograms
====

    cd usercode/WZ

Test

    root -l -b -q runAnalysisWZ.C

Produce all the output histograms, including systematics

    ./submitEverythingForMe.sh

Merge the output

    ./merge.sh results
    ./merge.sh results/0jet
    ./merge.sh results/1jet
    ./merge.sh results/2jet


Extract the WZ cross section
====

    cd usercode/WZ

Test

    root -l -b -q runXS.C

Produce all the plots and tables

    ./runXS.sh


GitHub (if you want to commit)
====

    mkdir GitHub
    git clone https://github.com/piedraj/usercode.git
    cd usercode/WZ
    git add *
    git commit -m 'My changes'
    git push origin master

