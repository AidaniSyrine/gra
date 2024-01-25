#!/bin/bash

make clean 
make 
cd build
echo -e "===========================================
Enter arguments:"
read -r -p "->  " args 
echo -e "===========================================
Command:"
set -x
./tonwertkorrektur.out $args
#set +x
#cd .. 
#make clean 