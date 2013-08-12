#!/bin/bash

if [ $# -lt 1 ]; then

    echo "  "
    echo "  ./doEverythingForMe.sh NJET"
    echo "  "

    exit -1
fi

./run.sh 0 $1
./run.sh 1 $1
./run.sh 2 $1
./run.sh 3 $1
./run.sh 4 $1
./run.sh 5 $1
./run.sh 6 $1
./run.sh 7 $1
./run.sh 8 $1

# ./merge.sh

# ./runXS.sh

# ./copyFiguresAndTables.sh

