make -f Makefile.cfglp

# run for all files.

for file in test_files/*.cs306.cfg
do
echo "$file"
./cfglp32ref -icode "$file"
mv "$file.ic" "$file.ic.ref"
./cfglp32 -icode "$file"

diff -bB "$file.ic" "$file.ic.ref"
done

