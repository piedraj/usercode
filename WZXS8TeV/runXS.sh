#!/bin/bash

rm -rf pdf
rm -rf png
rm -rf tex

for ((level=0; level<6; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    root -l -b -q XS.C+\($level,$mode,0\)
  done
done

root -l -b -q XS.C+\(MET30,PPFmode,1\)
root -l -b -q XS.C+\(MET30,MCmode,1\)

root -l -b -q drawScaleFactors.C+


# PDF tables
#-------------------------------------------------------------------------------
pdflatex mc_samples_wz8tev.tex
pdflatex mc_samples_wz8tev.tex

pdflatex yields.tex
pdflatex yields.tex

mkdir -p pdf/tables

cp index.php pdf/tables/.

mv mc_samples_wz8tev.pdf pdf/tables/.
mv yields.pdf            pdf/tables/.


# Cleanup
#-------------------------------------------------------------------------------
rm -rf *aux
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~

