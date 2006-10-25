#/bin/sh

#qmake -project -norecursive -t app `find $1/src -name "*.cpp"; find $1/src -name "*.ui"` -o $2.pro
#qmake -project -nopwd -recursive -t app $1/src -o $2.pro
qmake -project -nopwd -recursive -t app $1/src -o /tmp/tmp.pro
mv /tmp/tmp.pro $2.pro
