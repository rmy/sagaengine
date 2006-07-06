The following must be installed:
 - Ogre 1.2 (Dagon)
 - CEGUI 0.4
 - Angelscript (optional if you don't want angelscripting, but needed for some samples)

The tools may have its on requirements.
    python (for python scripts)
    FBX 2005.12 (for fbx scripts)


To build:

On linux:
   make sure you have qmake on your system. It comes with qt-devel on Fedora.


 cd to samples/simple/project/qmake
 Do: 
  sh bootstrap.sh
  make
  ./simple

 Or:
  sh bootstrap_lib.sh
  make
  ./simple

On Windows:
   Build architecture for Visual C++ is not yet complete, although the library will compile if you create your own..

