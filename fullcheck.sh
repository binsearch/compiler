make -f Makefile.cfglp

# run for all files.

for file in test_files/*.cs306.cfg
do
echo "$file"
./cfglp -d -ast "$file" > out
./cfglp32 -d -ast "$file" > eout

diff -bB out eout
done
