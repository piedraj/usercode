#!/bin/bash

rm -rf pdf
rm -rf png
rm -rf tex
rm -rf *.log

./merge.sh >& merge.log
grep Error merge.log

for ((level=0; level<5; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    root -l -b -q XS.C+\($level,$mode,0\)
  done
done

root -l -b -q XS.C+\(MET30,PPFmode,1\)
root -l -b -q XS.C+\(MET30,MCmode,1\)

root -l -b -q drawScaleFactors.C+
root -l -b -q drawWeights.C+


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *d
rm -rf *so
rm -rf *~

