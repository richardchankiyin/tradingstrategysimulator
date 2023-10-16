if [ "$#" -lt 1 ]; then echo "please specify file to be built"; exit -1; fi
BUILD=$1
./build.sh ${BUILD}
./${BUILD}.out 2>/dev/null
