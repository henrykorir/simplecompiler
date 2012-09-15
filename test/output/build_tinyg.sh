../../build/debug/test/tinycompiler ../input/tinyg.x tinyg.s
echo 'movl $0, %eax' >>tinyg.s
echo -e "\tleave" >>tinyg.s
echo -e '\tret' >>tinyg.s
gcc -o ttinyg tinyg.s
