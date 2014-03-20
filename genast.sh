for file in test_files/*.c
	do 
		echo ${file/test_files\//}
		str=$(echo ${file/test_files\//})
#		echo $str
		make -f Makefile.cfg FILE=$str
done