#!/bin/sh

python ../../../../tools/python/createVCProjFile.py navmesh_ui ../../../.. library > navmesh_ui.vcproj
python ../../../../tools/python/createVCProjFile.py navmesh_logic ../../../.. library > navmesh_logic.vcproj
python ../../../../tools/python/createVCProjFile.py navmesh ../../../.. application > navmesh.vcproj

python ../../../../tools/python/createVCProjFile.py navmesh_static ../../../.. application > navmesh_static.vcproj

pushd .
cd ../../../../core/projects/vc && sh bootstrap.sh
cd ../../../add_on/client/projects/vc && sh bootstrap.sh
cd ../../../basic/projects/vc && sh bootstrap.sh
cd ../../../ogre/projects/vc && sh bootstrap.sh
popd
