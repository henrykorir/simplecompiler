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

###################################################################################
# c-compiler-test
echo 'build parser'
scons debug=1 parseg=1

echo 'generator c-compiler code'
build/debug/test/parsesyntax cprogramlanguage/syntax/cprogram.syntax  cprogramlanguage/syntax/cplgrammar.cpp


echo 'build c-compiler'
scons debug=1 cplc=1

echo 'check .c file'
build/debug/cpl/kcc test/input/easy.c

