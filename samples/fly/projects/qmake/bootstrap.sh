#!/bin/sh

sh ../../../../tools/qmake/strap_app.sh ../../../../core core
sh ../../../../tools/qmake/strap_app.sh ../../../../add_on/client client
sh ../../../../tools/qmake/strap_app.sh ../../../../add_on/ogre ogre
sh ../../../../tools/qmake/strap_app.sh ../../ogre_ui ogre_ui
sh ../../../../tools/qmake/strap_app.sh ../../logic logic
sh ../../../../tools/qmake/strap_app.sh ../../start start

qmake -o makefile all.pro

