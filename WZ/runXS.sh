#!/bin/bash


rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~
rm -rf *_C.*
rm -rf *_C_*
rm -rf  job_AnalysisWZ_*
rm -rf  job_XS_*
rm -rf  pdf
rm -rf  png
rm -rf  tex


source /opt/root/bin/thisroot.sh
root -l -b -q loadXS.C


for ((cut=0; cut<7; cut++))
  do
  for ((mode=0; mode<2; mode++))
    do
    for ((wcharge=0; wcharge<1; wcharge++))
      do
      for ((njet=-1; njet<0; njet++))
	do
	./submitXS.sh $cut $mode $wcharge $njet
      done
    done
  done
done


root -l -b -q drawScaleFactors.C+
root -l -b -q drawWeights.C+
root -l -b -q drawElectronScaleSystematic.C+
