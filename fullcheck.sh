make -f Makefile.cfglp

# run for all files.

for file in test_files/*.cs306.cfg
do
echo "$file"
./cfglp32ref -compile "$file"
mv "$file.spim" "$file.spim.ref"
./cfglp32 -compile "$file"

diff -bB "$file.spim" "$file.spim.ref"
done

