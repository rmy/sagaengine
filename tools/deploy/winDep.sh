#!/bin/sh

mkdir bin
mkdir bin/debug
mkdir bin/release

cp /c/OgreSDK/bin/debug/*.dll bin/debug
cp /c/OgreSDK/bin/release/*.dll bin/release
cp /c/Programfiler/FMOD\ SoundSystem/FMOD\ Programmers\ API/api/fmodex.dll bin/release
cp /c/Programfiler/FMOD\ SoundSystem/FMOD\ Programmers\ API/api/fmodex.dll bin/debug

mkdir dependencies
cd dependencies

mkdir lib
mkdir lib/debug
mkdir lib/release

mkdir include
mkdir include/ogre
mkdir include/ogre/samples
mkdir include/fmod

cp /c/OgreSDK/lib/* lib/release/
mv lib/release/*.pdb lib/debug
mv lib/release/*_d.* lib/debug
cp lib/release/ode.* lib/debug
cp -R /c/OgreSDK/lib/opt/* lib
cp /c/Programfiler/FMOD\ SoundSystem/FMOD\ Programmers\ API/api/lib/fmodex_vc.lib lib/release/fmodex.lib
cp /c/Programfiler/FMOD\ SoundSystem/FMOD\ Programmers\ API/api/lib/fmodex_vc.lib lib/debug/fmodex.lib

cp -R /c/OgreSDK/include/* include/ogre
cp /c/OgreSDK/samples/include/OgreCEGUI*.h include/ogre/samples
cp /c/Programfiler/FMOD\ SoundSystem/FMOD\ Programmers\ API/api/inc/* include/fmod/