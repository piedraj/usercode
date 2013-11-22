#!/bin/bash


rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~
rm -rf *_C.*
rm -rf *_C_*
rm -rf  job_XS_*.sh
rm -rf  pdf
rm -rf  png
rm -rf  tex


root -l -b -q loadXS.C


for ((cut=0; cut<10; cut++))
  do
  for ((mode=0; mode<2; mode++))
    do
    for ((wcharge=0; wcharge<3; wcharge++))
      do
      for ((njet=-1; njet<2; njet++))
	do
	./submitXS.sh $cut $mode $wcharge $njet
      done
    done
  done
done


root -l -b -q drawScaleFactors.C+
root -l -b -q drawWeights.C+
root -l -b -q drawElectronScaleSystematic.C+

