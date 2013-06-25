#!/bin/bash


rm -rf pdf
rm -rf png
rm -rf tex


for ((level=0; level<9; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    for ((wcharge=0; wcharge<3; wcharge++))
      do
      root -l -b -q "XS.C+($level,$mode,$wcharge)"
    done
  done
done


root -l -b -q drawScaleFactors.C+
root -l -b -q drawWeights.C+
root -l -b -q drawElectronScaleSystematic.C+


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~

