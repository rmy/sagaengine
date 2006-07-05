#!/bin/sh

sh ../../../../tools/qmake/strap_app.sh ../../../../core core
sh ../../../../tools/qmake/strap_app.sh ../../../../add_on/client client
sh ../../../../tools/qmake/strap_app.sh ../../../../add_on/ogre ogre
sh ../../../../tools/qmake/strap_app.sh ../../ogre_ui ogre_ui
sh ../../../../tools/qmake/strap_app.sh ../../game game

rm core.pro client.pro ogre.pro ogre_ui.pro game.pro

qmake -o makefile all.pro
