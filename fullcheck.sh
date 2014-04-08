make -f Makefile.cfglp

# run for all files.

for file in test_files/*.cs306.cfg
do
echo "$file"
./cfglp64ref -lra -compile "$file"
mv "$file.spim" "$file.spim.ref"
./cfglp64 -lra -compile "$file"

diff -bB "$file.spim" "$file.spim.ref"
done

