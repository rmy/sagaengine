#/bin/sh

qmake -project -t app `find ../../src -name "*.?pp"` -o src.pro
qmake -o makefile lib.pro
