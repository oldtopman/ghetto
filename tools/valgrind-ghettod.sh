#!/bin/bash

#Get current directory (tools)
BASEDIR=$(dirname $0)
if [ "$BASEDIR" == "." ]
  then
    BASEDIR=$(pwd)
fi

#Cd to root of project.
cd "${BASEDIR}/.."

#cd to ghettod binary location
cd src

#Code here!
make
valgrind -v --leak-check=full  --track-origins=yes ghettod
sleep 1
echo
echo Complete!
