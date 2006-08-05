#!/bin/sh

sh ../../../../tools/qmake/strap_app.sh ../../../../core core
sh ../../../../tools/qmake/strap_app.sh ../../ test

qmake -o makefile all.pro

