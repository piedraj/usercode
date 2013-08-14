#!/bin/bash

rm -rf *.d
rm -rf *.so
rm -rf results

root -l -b -q loadAnalysisWZ.C

./Submit.sh 0 0 -1 20 35
./Submit.sh 0 0  0 20 35
./Submit.sh 0 0  1 20 35

./Submit.sh 8 1 -1 20 35
./Submit.sh 8 1  0 20 35
./Submit.sh 8 1  1 20 35

rm -rf job_*.sh

