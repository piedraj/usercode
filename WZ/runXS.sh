#!/bin/bash


for ((level=0; level<9; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    for ((njet=-1; njet<2; njet++))
      do
      root -l -b -q "XS.C+($level,$mode,0,$njet)"  # WInclusive
    done
  done
done


### root -l -b -q "XS.C+(4,0,1)"  # MET30, PPFmode, WPlus
### root -l -b -q "XS.C+(4,0,2)"  # MET30, PPFmode, WMinus


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~

