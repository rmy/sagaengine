#/bin/sh

qmake -project -t lib `find ../../src -name "*.?pp"` -o core.pro
qmake -o makefile lib.pro



