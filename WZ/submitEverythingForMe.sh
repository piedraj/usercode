#!/bin/bash

rm -rf *.d
rm -rf *.so
rm -rf job_*.sh
rm -rf results

root -l -b -q loadAnalysisWZ.C

./submit.sh 0 0 -1 20 35
./submit.sh 0 0  0 20 35
./submit.sh 0 0  1 20 35

./submit.sh 8 1 -1 20 35
./submit.sh 8 1  0 20 35
./submit.sh 8 1  1 20 35


