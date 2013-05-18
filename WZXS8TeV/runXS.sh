#!/bin/bash


rm -rf pdf
rm -rf png
rm -rf tex


### ./merge.sh >& merge.log
### grep Error merge.log


for ((level=0; level<6; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    root -l -b -q "XS.C+($level,$mode)"
  done
done


root -l -b -q drawScaleFactors.C+
root -l -b -q drawWeights.C+
root -l -b -q drawElectronScaleSystematic.C+


### ./copyFiguresAndTables.sh


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~

