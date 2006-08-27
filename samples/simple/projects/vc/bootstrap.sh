#!/bin/sh

python ../../../../tools/python/createVCProjFile.py simple_ui ../../../.. library > simple_ui.vcproj
python ../../../../tools/python/createVCProjFile.py simple_logic ../../../.. library > simple_logic.vcproj
python ../../../../tools/python/createVCProjFile.py simple ../../../.. application > simple.vcproj

python ../../../../tools/python/createVCProjFile.py simple_static ../../../.. application > simple_static.vcproj

pushd .
cd ../../../../core/projects/vc && sh bootstrap.sh
cd ../../../add_on/client/projects/vc && sh bootstrap.sh
cd ../../../basic/projects/vc && sh bootstrap.sh
cd ../../../ogre/projects/vc && sh bootstrap.sh
popd

