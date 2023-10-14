if [ "$#" -lt 1 ]; then echo "please specify file to be built"; exit -1; fi
STD="c++20"
BUILD=$1
g++ -std=${STD} ${BUILD}.cpp -c -g -o ${BUILD}.o
g++ -std=${STD} ${BUILD}.o -o ${BUILD}_debug.out
gdb ${BUILD}_debug.out

