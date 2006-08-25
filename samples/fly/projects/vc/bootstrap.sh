#!/bin/sh

python ../../../../tools/python/createVCProjFile.py se_core ../../../.. library > se_core.vcproj
python ../../../../tools/python/createVCProjFile.py se_client ../../../.. library > se_client.vcproj
python ../../../../tools/python/createVCProjFile.py se_basic ../../../.. library > se_basic.vcproj
python ../../../../tools/python/createVCProjFile.py se_ogre ../../../.. library > se_ogre.vcproj
python ../../../../tools/python/createVCProjFile.py ui ../../../.. library > ui.vcproj
python ../../../../tools/python/createVCProjFile.py logic ../../../.. library > logic.vcproj
python ../../../../tools/python/createVCProjFile.py fly ../../../.. application > fly.vcproj

python ../../../../tools/python/createVCProjFile.py all ../../../.. application > all.vcproj
