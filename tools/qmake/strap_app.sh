#/bin/sh

qmake -project -t app `find $1/src -name "*.?pp"` -o $2.pro
