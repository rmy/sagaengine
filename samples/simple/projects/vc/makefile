# Do 'nmake' on the command prompt to run this makefile
# Only tested with Visual C++ 2003.
# You must have vcbuild.exe installed:
#   http://www.gotdotnet.com/team/cplusplus/samples/vcbuild.zip
#

# Needed to make 'nmake release' work, because of 
# Release subdir
release: rel

# Needed to make 'nmake debug' work, because of
# Debug subdir
debug: dbg

rel:
	vcbuild se_core Release
	vcbuild se_client Release
	vcbuild se_basic Release
	vcbuild se_ogre Release
	vcbuild logic Release
	vcbuild ui Release
	vcbuild simple Release

dbg:
	vcbuild se_core Debug
	vcbuild se_client Debug
	vcbuild se_basic Debug
	vcbuild se_ogre Debug
	vcbuild logic Debug
	vcbuild ui Debug
	vcbuild simple Debug

clean:
	vcbuild -c se_core
	vcbuild -c se_client
	vcbuild -c se_basic
	vcbuild -c se_ogre
	vcbuild -c logic
	vcbuild -c ui
	vcbuild -c simple

static_debug:
	vcbuild all Debug

static_release:
	vcbuild all Release
