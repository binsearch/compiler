#!/bin/bash

make -f Makefile.cfglp clean
echo Please, enter your file name ....
read name
name1="$name.cs306.cfg"
# echo $name

name="$name.c"
echo $name
echo $name1
# cat test_files/$name
make FILE=$name -f Makefile.cfg
make -f Makefile.cfglp
./cfglp -d -tokens test_files/$name1 > out
./cfglp64 -d -tokens test_files/$name1 > eout

diff -b out eout
