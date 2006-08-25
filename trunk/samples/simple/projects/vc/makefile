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
	vcbuild ../../../../core/projects/vc/se_core.vcproj Release
	vcbuild ../../../../add_on/client/projects/vc/se_client.vcproj Release
	vcbuild ../../../../add_on/basic/projects/vc/se_basic.vcproj Release
	vcbuild ../../../../add_on/ogre/projects/vc/se_ogre.vcproj Release
	vcbuild logic Release
	vcbuild ui Release
	vcbuild fly Release

dbg:
	vcbuild ../../../../core/projects/vc/se_core.vcproj Debug
	vcbuild ../../../../add_on/client/projects/vc/se_client.vcproj Debug
	vcbuild ../../../../add_on/basic/projects/vc/se_basic.vcproj Debug
	vcbuild ../../../../add_on/ogre/projects/vc/se_ogre.vcproj Debug
	vcbuild logic Debug
	vcbuild ui Debug
	vcbuild fly Debug

clean:
	vcbuild -c ../../../../core/projects/vc/se_core.vcproj
	vcbuild -c ../../../../add_on/client/projects/vc/se_client.vcproj
	vcbuild -c ../../../../add_on/basic/projects/vc/se_basic.vcproj
	vcbuild -c ../../../../add_on/ogre/projects/vc/se_ogre.vcproj
	vcbuild -c logic
	vcbuild -c ui
	vcbuild -c fly

static_debug:
	vcbuild all Debug

static_release:
	vcbuild all Release
