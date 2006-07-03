#!/usr/bin/python

import os
import sys
from MakeMakefile import MakeMakefile

modules = list()
fd = open("modules.txt", "r")
for line in fd.readlines():
	modules.append( line.strip() )
fd.close()



SRC_DIR = os.curdir

if __name__ == '__main__':
    print """
-include rel_type
include module_config
"""

    MakeMakefile(modules, SRC_DIR, "$(LINK_TARGET)", " ")

    print """

#
# Auto generate all.hpp files
#

alls:
"""
    for m in modules:
        print "	python $(SE_SCRIPTS)/createAllHpp.py " + m + "/"
    print """

#
# Makefile
#

mf:
	python $(SE_SCRIPTS)/createGccMakefile.py > makefile
    """
