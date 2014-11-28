#!/bin/bash


rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~
rm -rf *_C.*
rm -rf *_C_*
rm -rf  job_AnalysisWZ_*.sh
rm -rf  results-2014


source ~/.bashrc
root -l -b -q loadAnalysisWZ.C


################################################################################
#
# ./submit.sh  SYSTEMATIC  MODE  JET_CHANNEL  MUON_JET_ET  ELEC_JET_ET
#
################################################################################


#for NJET in -1 0 1 2; do
for NJET in -1; do

  # Nominal analysis
  #-----------------------------------------------------------------------------
  ./submit.sh 0 0 $NJET 20 35


  # Systematics
  #-----------------------------------------------------------------------------
  for SYSTEMATIC in 1 2 3 4 5 6 7; do

    ./submit.sh $SYSTEMATIC 0 $NJET 20 35

  done


  # Full data-driven estimation of the Z+jets and top backgrounds
  #-----------------------------------------------------------------------------
  #for MUON_JET_PT in 05 10 15 20 25 30 35 40 45 50; do
  for MUON_JET_PT in 10 20 30; do

      for ELEC_JET_PT in 15 35 50; do

	  ./submit.sh 8 1 $NJET ${MUON_JET_PT} ${ELEC_JET_PT}

      done

  done

done

