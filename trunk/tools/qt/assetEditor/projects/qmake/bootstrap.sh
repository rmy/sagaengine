#!/bin/sh

sh ../../../../../tools/qmake/strap_app.sh ../../../../../core core
sh ../../../../../tools/qmake/strap_app.sh ../../../../../add_on/platform_pc platform_pc
sh ../../../../../tools/qmake/strap_app.sh ../.. start

qmake -o makefile all.pro
