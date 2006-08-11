#!/bin/sh

python ../../../../tools/python/createVCProjFile.py se_core ../../../../env/vc/library > se_core.vcproj
python ../../../../tools/python/createVCProjFile.py se_client ../../../../env/vc/library > se_client.vcproj
python ../../../../tools/python/createVCProjFile.py se_ogre ../../../../env/vc/library > se_ogre.vcproj
python ../../../../tools/python/createVCProjFile.py ui ../../../../env/vc/library > ui.vcproj

python ../../../../tools/python/createVCProjFile.py simple ../../../../env/vc/application > simple.vcproj

mkdir Debug
mkdir Release
cp /c/ogre-win32-v1-2-1/ogrenew/Samples/Common/bin/Release/*.dll ./Debug
cp /c/ogre-win32-v1-2-1/ogrenew/Samples/Common/bin/Release/*.dll ./Release

cp /c/ogre-win32-v1-2-1/ogrenew/Samples/Common/CEGUIRenderer/lib/*.lib ./Debug
cp /c/ogre-win32-v1-2-1/ogrenew/Samples/Common/CEGUIRenderer/lib/*.lib ./Release

cp /c/ogre-win32-v1-2-1/ogrenew/Dependencies/lib/Release/*.lib ./Debug
cp /c/ogre-win32-v1-2-1/ogrenew/Dependencies/lib/Release/*.lib ./Release
cp /c/ogre-win32-v1-2-1/ogrenew/OgreMain/lib/Release/*.lib ./Debug
cp /c/ogre-win32-v1-2-1/ogrenew/OgreMain/lib/Release/*.lib ./Release
