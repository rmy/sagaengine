#/bin/sh

sh ../../../tools/qmake/strap_lib.sh ../.. core
sh ../../../tools/qmake/strap_lib.sh ../../../add_on/platform_pc pc
qmake -o makefile lib.pro
