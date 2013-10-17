#!/bin/bash


for ((cut=0; cut<10; cut++))
  do
  for ((mode=0; mode<2; mode++))
    do
    for ((wcharge=0; wcharge<3; wcharge++))
      do
      for ((njet=-1; njet<2; njet++))
	do
	root -l -b -q "XS.C+($cut,$mode,$wcharge,$njet)"
      done
    done
  done
done


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~

