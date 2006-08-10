#/bin/sh

qmake -project -t lib `find $1/src -name "*.?pp"` -o $2.pro
