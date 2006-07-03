#!/usr/bin/python

import os
import sys


SRC_DIR = os.curdir
if(len(sys.argv) > 1):
    v = sys.argv[1]
    SRC_DIR = os.path.join(os.curdir, v)

def CleanWalker(unused, dirname, nameList):
    if(not dirname.startswith(SRC_DIR)):
        print dirname
        sys.exit()

    for entry in nameList:
        if(entry.endswith(".o") or entry.endswith(".obj") or entry.endswith(".gch")):
            f = os.path.join(dirname, entry)
            print "Removing:", os.path.normpath(f)
            os.remove(f)
 

if __name__ == '__main__':
    os.path.walk(SRC_DIR, CleanWalker, None)

    deletes = ("se_core.so", "run.elf", "merged.elf", "merged.gba")
    for entry in deletes:
        f = os.path.join(SRC_DIR, entry)
        if(os.path.exists(f)):
            print "Removing:", os.path.normpath(f)
            os.remove(f)
