# run all test

## c-expression-test
##build/debug/test/parsesyntax vs2008/Debug/cexpression.txt test/output/cexptest1.txt
#
##echo 'build parser'
##scons debug=1 parseg=1
##
##echo 'generator c-program language expression compiler code'
##build/debug/test/parsesyntax test/input/cexpression.syntax test/output/cexpression_test_out_tmp.cpp
#
#echo 'build c-compiler-expression'
#scons debug=1 cexp=1
#
#echo 'run test'
#ln -f build/debug/test/cexptest curtest
#build/debug/test/cexptest test/input/cexptest.input

#while getopts "r" arg
#do
#	case $arg in
#		r)
#			

###################################################################################

DOPARSE=0
NOTCALLREUCE=0
ISDEBUG=0

while getopts "dpn" arg
do
	case $arg in
		d)
			ISDEBUG=1
			;;
		p)
			DOPARSE=1
			;;
		n)
			NOTCALLREUCE=1
			;;
		?)
			echo 'unknown params'
			exit 1
			;;
	esac
done

if [ ${ISDEBUG} == 0 ]; then
	binfolder=release
else
	binfolder=debug
fi

if [ ${DOPARSE} == 1 ]; then
# c-compiler-test
	echo 'build parser'
	scons debug=${ISDEBUG} parseg=1 noreduce=${NOTCALLREUCE} -j4

	if [ $? != 0 ]; then exit 1; fi

	echo 'generator c-expression code'
	build/${binfolder}/test/parsesyntax cprogramlanguage/syntax/cexptest.syntax cexpressiontest

	if [ $? != 0 ]; then exit 1; fi
fi

echo 'build c-expression-compiler'
scons debug=${ISDEBUG} cexp=1 noreduce=${NOTCALLREUCE} -j4
if [ $? != 0 ]; then exit 1; fi

echo 'check .c file'
build/${binfolder}/cexp/kcexp cexpressiontest/test/test3.c
if [ $? != 0 ]; then exit 1; fi

#echo 'generator c-compiler code'
#build/debug/test/parsesyntax cprogramlanguage/syntax/cprogram.syntax  cprogramlanguage/syntax

#echo 'build c-compiler'
#scons debug=1 cplc=1
#
#echo 'check .c file'
#build/debug/cpl/kcc test/input/easy.c

