#!/bin/bash


for file in *.c
do
echo "$file"
cd ..
# ls
make FILE="$file" -f ../Makefile.cfg
cd test_files/
# ls
# ./cfglp -d -tokens "$file" > out
done