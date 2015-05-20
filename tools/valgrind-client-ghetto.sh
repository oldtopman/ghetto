#!/bin/bash

#Doesn't matter where we are.

#Code here!
nc -l -p 3333
echo
echo
read -n1 -r -p "Press any key to restart the client" key
clear
exec ~/src/codeblocks/wheatgrinder/scripts/valgrind-client.sh
