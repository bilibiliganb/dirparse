#!/bin/bash

if [ ! -f ./test.db ]; then
    dos2unix dir.txt
    ./bar
    echo ""
    ./searchall $1
else
    # echo "search $1"
    ./searchall $1
fi
