#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./runXS.sh RUN_AT_OVIEDO"
    echo "  "
    exit -1
fi

RUN_AT_OVIEDO=$1

rm -rf pdf
rm -rf png
rm -rf tex

for ((level=0; level<6; level++))
  do
  for ((mode=0; mode<2; mode++))
    do
    root -l -b -q XS.C+\($level,$mode,0,${RUN_AT_OVIEDO}\)
  done
done

root -l -b -q XS.C+\(MET,PPFmode,1,${RUN_AT_OVIEDO}\)
root -l -b -q XS.C+\(MET,PPFmode,1,${RUN_AT_OVIEDO}\)

root -l -b -q drawScaleFactors.C+\(${RUN_AT_OVIEDO}\)

pdflatex mc_samples_wz8tev.tex
pdflatex mc_samples_wz8tev.tex

pdflatex yields.tex
pdflatex yields.tex

mv mc_samples_wz8tev.pdf pdf/.
mv yields.pdf            pdf/.

rm -rf *aux
rm -rf *d
rm -rf *log
rm -rf *so
rm -rf *~
