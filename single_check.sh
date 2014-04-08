make -f Makefile.cfglp

echo "$1"
./cfglp32ref  -icode -lra "$1"
mv "$1.ic" "$1.ic.ref"
./cfglp32 -icode -lra "$1"

diff -bB "$1.ic" "$1.ic.ref"
