#!/bin/sh

mkdir bin
mkdir bin/debug
mkdir bin/release

cp /c/OgreSDK/bin/debug/*.dll bin/debug
cp /c/OgreSDK/bin/release/*.dll bin/release


mkdir dependencies
cd dependencies

mkdir lib
mkdir lib/debug
mkdir lib/release

mkdir include
mkdir include/ogre
mkdir include/ogre/samples


cp /c/OgreSDK/lib/* lib/release/
mv lib/release/*.pdb lib/debug
mv lib/release/*_d.* lib/debug
cp -R /c/OgreSDK/lib/opt/* lib

cp -R /c/OgreSDK/include/* include/ogre
cp /c/OgreSDK/samples/include/OgreCEGUI*.h include/ogre/samples
