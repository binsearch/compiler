# make -f Makefile.cfglp clean
make -f Makefile.cfglp

echo "$1"
./cfglp32ref -d -compile "$1" > out
# mv "$1.ic" "$1.ic.ref"
./cfglp32 -d -compile "$1" > eout

diff -bB out eout
