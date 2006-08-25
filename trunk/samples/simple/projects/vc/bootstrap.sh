#!/bin/sh

python ../../../../tools/python/createVCProjFile.py ui ../../../.. library > ui.vcproj
python ../../../../tools/python/createVCProjFile.py logic ../../../.. library > logic.vcproj
python ../../../../tools/python/createVCProjFile.py simple ../../../.. application > simple.vcproj

python ../../../../tools/python/createVCProjFile.py all ../../../.. application > all.vcproj

pushd .
cd ../../../../core/projects/vc && sh bootstrap.sh
cd ../../../add_on/client/projects/vc && sh bootstrap.sh
cd ../../../basic/projects/vc && sh bootstrap.sh
cd ../../../ogre/projects/vc && sh bootstrap.sh
popd

