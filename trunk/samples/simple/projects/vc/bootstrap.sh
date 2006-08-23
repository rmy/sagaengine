#!/bin/sh

python ../../../../tools/python/createVCProjFile.py se_core ../../../../env/vc/library > se_core.vcproj
python ../../../../tools/python/createVCProjFile.py se_client ../../../../env/vc/library > se_client.vcproj
python ../../../../tools/python/createVCProjFile.py se_basic ../../../../env/vc/library > se_basic.vcproj
python ../../../../tools/python/createVCProjFile.py se_ogre ../../../../env/vc/library > se_ogre.vcproj
python ../../../../tools/python/createVCProjFile.py ui ../../../../env/vc/library > ui.vcproj
python ../../../../tools/python/createVCProjFile.py logic ../../../../env/vc/library > logic.vcproj
python ../../../../tools/python/createVCProjFile.py simple ../../../../env/vc/application > simple.vcproj

python ../../../../tools/python/createVCProjFile.py all ../../../../env/vc/application > all.vcproj

cp ../../../../dependencies/bin/release/* Release/
cp ../../../../dependencies/bin/debug/* Debug/
