#!/bin/bash


rm -rf pdf
rm -rf png
rm -rf tex


for ((level=0; level<9; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    root -l -b -q "XS.C+($level,$mode,0)"  # WInclusive
  done
done

root -l -b -q "XS.C+(4,0,1)"  # MET30, PPFmode, WPlus
root -l -b -q "XS.C+(4,0,2)"  # MET30, PPFmode, WMinus


root -l -b -q drawScaleFactors.C+
root -l -b -q drawWeights.C+
root -l -b -q drawElectronScaleSystematic.C+


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~

