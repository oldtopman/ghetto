#!/bin/bash

#Get current directory (tools)
BASEDIR=$(dirname $0)
if [ "$BASEDIR" == "." ]
  then
    BASEDIR=$(pwd)
fi

#Cd to root of project.
cd "${BASEDIR}/.."

#cd to ghetto binary location
cd src

#Code here!
make
valgrind -v --leak-check=full  --track-origins=yes --log-socket=127.0.0.1:3333 ghetto
sleep 1
echo
echo Complete!
