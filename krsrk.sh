#!/bin/bash

make -f Makefile.cfglp clean
# name1="$1.cs306.cfg"
# echo $name

# name="$1.c"
# cat test_files/$name
# make FILE=$name -f Makefile.cfg
make -f Makefile.cfglp
./cfglp -d -eval  $1 >out 
./cfglp32 -d -eval  $1 > eout

diff -bB out eout

# ./cfglp -d -ast $1 >out
# ./cfglp32 -d -ast $1 > eout

# diff -bB out eout
