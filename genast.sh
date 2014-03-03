
# run for all files.

for file in test_files/*.c
do
echo "$file"
make FILE="$file" -f Makefile.cfg 
done