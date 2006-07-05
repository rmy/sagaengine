#/bin/sh

qmake -project -t lib `find ../../src -name "*.?pp"` -o src.pro
qmake -o makefile lib.pro
