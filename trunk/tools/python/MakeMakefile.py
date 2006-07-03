import os
import sys
from FileWalker import FileWalker


def MakeMakefile(modules, SRC_DIR, target, extraLinks):
    print "#"
    print "# Headers"
    print "#"
    hppWalker = FileWalker(SRC_DIR, "")
    for m in modules:
        hppWalker.setPrefix(SRC_DIR, os.path.normpath(m));
        os.path.walk(hppWalker.src_dir_, FileWalker.Hpp, hppWalker)

    hppFiles = hppWalker.files_

    for f in hppFiles.keys():
        hppFiles[ f ].calcHppDependencies(hppFiles)
        hppDef = f.split("/")[-1].replace(".", "_") + " ="
        for h in hppFiles[f].dependencies_.values():
            hppDef += " " + h
        print hppDef.replace("\\", "/"), "\n"

    cppWalker = FileWalker(SRC_DIR, "")
    for m in modules:
        cppWalker.setPrefix(SRC_DIR, m)
        os.path.walk(cppWalker.src_dir_, FileWalker.Cpp, cppWalker)

    cppFiles = cppWalker.files_

    print "#"
    print "# Compile"
    print "#"
    for f in cppFiles.keys():
        n = cppFiles[f].getPrefixedName()
        o = n.replace(".cpp", ".o")
        cppDef = o + ": " + n
        for hpp in cppFiles[f].headers_:
            h = hpp.split("/")[-1]
            h = h.replace(".", "_")
            cppDef += " $(" + h + ")"
        print cppDef.replace("\\", "/")

        if(f.find(".text.iwram") != -1):
            # Special handling for special GBA file
            print "\t$(COMPILE_ARM)"
        else:
            print "\t$(COMPILE)"
        print "\n"

    print "#"
    print "# Link"
    print "#"

    link = target + ": " + extraLinks
    for f in cppFiles.keys():
        n = cppFiles[f].getPrefixedName()
        o = n.replace(".cpp", ".o")
        link += " " + o

    print link.replace("\\", "/")
    print "\t$(LINK)"
    print "\n"

    print """
#
# Clean
#

clean:"""
    for m in modules:
        print "\tpython $(SE_SCRIPTS)/clean.py %s" % (m)
    print "\trm -f $(LINK_TARGET) $(TARGET_DIR)/$(LINK_TARGET)"
    print ""

