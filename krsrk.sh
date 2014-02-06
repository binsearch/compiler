#!/bin/bash

make -f Makefile.cfglp clean
name1="$1cs306.cfg"
# echo $name

name="$1c"
# cat test_files/$name
#make FILE=$name -f Makefile.cfg
make -f Makefile.cfglp
echo "krish"
echo $name1
./cfglp -d -eval $name1 > out
./cfglp32 -d -eval $name1 > eout

diff -b -B out eout
