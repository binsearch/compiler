#!/bin/bash

# make -f Makefile.cfglp clean
# name1="$1.cs306.cfg"
# echo $name

# name="$1.c"
# cat test_files/$name
# make FILE=$name -f Makefile.cfg
make -f Makefile.cfglp
./cfglp -d -tokens $1 > out
./cfglp64 -d -tokens $1 > eout

diff -b out eout
