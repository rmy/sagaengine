#!/bin/sh

python ../../../../tools/python/createVCProjFile.py fly_ui ../../../.. library > fly_ui.vcproj
python ../../../../tools/python/createVCProjFile.py fly_logic ../../../.. library > fly_logic.vcproj
python ../../../../tools/python/createVCProjFile.py fly ../../../.. application > fly.vcproj

python ../../../../tools/python/createVCProjFile.py fly_static ../../../.. application > fly_static.vcproj

pushd .
cd ../../../../core/projects/vc && sh bootstrap.sh
cd ../../../add_on/client/projects/vc && sh bootstrap.sh
cd ../../../basic/projects/vc && sh bootstrap.sh
cd ../../../ogre/projects/vc && sh bootstrap.sh
popd
