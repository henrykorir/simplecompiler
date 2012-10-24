# run all test

#build/debug/test/parsesyntax vs2008/Debug/cexpression.txt test/output/cexptest1.txt

#echo 'build parser'
#scons debug=1 parseg=1
#
#echo 'generator c-program language compiler code'
#build/debug/test/parsesyntax test/input/cexpression.syntax test/output/cexpression_test_out.cpp

echo 'build c-compiler'
scons debug=1 cexp=1

echo 'run test'
build/debug/test/cexptest test/input/cexptest.input
