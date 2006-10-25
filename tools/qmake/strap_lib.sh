#/bin/sh

qmake-qt4 -project -t lib `find $1/src -name "*.?pp"` -o $2.pro
qmake -project -nopwd -recursive -t lib $1/src -o $2.pro
