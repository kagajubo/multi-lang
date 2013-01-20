FONT="arialuni"
SIZE="24"

print_usage()
{
echo "usage: make-all [-l][-b] [-m]"
echo "  '-l' use for little-endian, '-b' use for big-endian"
exit 0
}

if [ $# == 0 ];then
	print_usage
fi
if [ $# == 1 ];then
	if [ "$1" = "-b" ];then
		echo "make for big-endian...."
	elif [ "$1" = "-l" ];then
		echo "make for little-endian...."
	else
		print_usage
	fi
elif [ $# == 2 ];then
	if [ "$2" == "-m" ];then
		echo "rebuild all tools..."
		make clean
		make
	else
		print_usage
	fi
else
	print_usage
fi

f_get_unicode()
{
echo "make font table ..."
./GetUnicode strtable_eng.txt font_table_eng.txt font_table_eng.h font_table_eng
./GetUnicode strtable_sch.txt font_table_sch.txt font_table_sch.h font_table_sch
./GetUnicode strtable_rus.txt font_table_rus.txt font_table_rus.h font_table_rus
./GetUnicode strtable_kor.txt font_table_kor.txt font_table_kor.h font_table_kor
}

f_convert2utf8()
{
echo "convert to utf8 ..."
./ConvertToUTF8 strtable_eng.txt strtable_eng.c
./ConvertToUTF8 strtable_sch.txt strtable_sch.c
./ConvertToUTF8 strtable_rus.txt strtable_rus.c
./ConvertToUTF8 strtable_kor.txt strtable_kor.c
}

f_select_charcode()
{
echo "select char code..."
./SelectCharCode ${FONT}_U${SIZE}.bin ${FONT}_U${SIZE}_eng.bin 1 0x20 0x7e
./SelectCharCode ${FONT}_U${SIZE}.bin ${FONT}_U${SIZE}_sch.bin 2 ../language/font_table_sch.h
./SelectCharCode ${FONT}_U${SIZE}.bin ${FONT}_U${SIZE}_rus.bin 2 ../language/font_table_rus.h
./SelectCharCode ${FONT}_U${SIZE}.bin ${FONT}_U${SIZE}_kor.bin 2 ../language/font_table_kor.h
}

f_convert2big_endian()
{
echo "convert to big-endian..."
if [ $# == 0 ];then
	echo "must contain a parameter to mark little-endian or big-endian"
fi
ENDIAN=$1
./ConvertToBigEndian ${FONT}_U${SIZE}_eng.bin ${FONT}_U${SIZE}_eng${ENDIAN}.bin
./ConvertToBigEndian ${FONT}_U${SIZE}_sch.bin ${FONT}_U${SIZE}_sch${ENDIAN}.bin
./ConvertToBigEndian ${FONT}_U${SIZE}_rus.bin ${FONT}_U${SIZE}_rus${ENDIAN}.bin
./ConvertToBigEndian ${FONT}_U${SIZE}_kor.bin ${FONT}_U${SIZE}_kor${ENDIAN}.bin
echo $ENDIAN
}

f_convert2c_array()
{
echo "convert to carray..."
# MBCS encode type
#./ToCArray ${FONT}${SIZE}_CP1252.bin wordstock_eng.h wordstock_eng
#./ToCArray ${FONT}${SIZE}_CP936.bin wordstock_sch.h wordstock_sch
./ToCArray ${FONT}_U${SIZE}_eng${ENDIAN}.bin wordstock_eng.h wordstock_eng
./ToCArray ${FONT}_U${SIZE}_sch${ENDIAN}.bin wordstock_sch.h wordstock_sch
./ToCArray ${FONT}_U${SIZE}_rus${ENDIAN}.bin wordstock_rus.h wordstock_rus
./ToCArray ${FONT}_U${SIZE}_kor${ENDIAN}.bin wordstock_kor.h wordstock_kor
}


echo "build for little-endian ..."
echo "make font table..."
cp GetUnicode ConvertToUTF8 ../language/
cd ../language
f_get_unicode
f_convert2utf8
#exit clean
rm GetUnicode ConvertToUTF8

cd ../tools
cp SelectCharCode ToCArray ConvertToBigEndian ../wordstock/
cd ../wordstock
f_select_charcode
if [ "$1" == "-b" ];then
	f_convert2big_endian "_big"
fi
f_convert2c_array
#exit clean
rm SelectCharCode ToCArray ConvertToBigEndian
echo "all done."
