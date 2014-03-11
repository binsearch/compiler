#!/bin/bash

# make -f Makefile.cfglp clean
# name1="$1.cs306.cfg"
# echo $name

# name="$1.c"
# cat test_files/$name
# make FILE=$name -f Makefile.cfg
make -f Makefile.cfglp


# ./cfglp -d -eval $1 >out
# ./cfglp32 -d -eval $1 > eout

# diff -bB out eout


dir=./test_files/*

for f in $dir
do
	if [[ ${f: -10} == ".cs306.cfg" ]]
	then
		echo ${f:-10}
		./cfglp -d -eval $f > out
		./cfglp32 -d -eval $f > eout
		diff -bB out eout
	fi

  # echo "Processing $f file..."
  # take action on each file. $f store current file name
  # cat $f
done