if [ "$#" -lt 1 ]; then echo "please specify file to be compiled"; exit -1; fi
STD="c++20"
BUILD=$1
g++ -std=${STD} ${BUILD}.h -c 
