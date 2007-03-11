#!/usr/bin/python

import os
import sys
import string

# Modules are now in the modules.txt file in the script directory
# Read it from disk
templates = list();
DIR = os.path.join(os.path.dirname(sys.argv[0]), ".template", sys.argv[1])
for f in os.listdir(DIR):
	if f.endswith('pp'):
		path = os.path.join(DIR, f)
		templates.append(path);



(namespace, type) = sys.argv[2].split("::")
namespaceCap = ""
np = namespace.split("_")
for p in np:
	namespaceCap = namespaceCap + p.capitalize()
		

for t in templates:
	fin = open(t, "r")
	filename = "%s" % (os.path.basename(t).replace("_", type))
	fout = open(filename, "w");
	for line in fin.readlines():
		line = line.replace("%_", type.lower())
		line = line.replace("%s", type)
		line = line.replace("%S", type.upper())
		if(namespace == "se_core"):
			line = line.replace("se_core::", "")
			line = line.replace("using namespace se_core;", "")
		
		line = line.replace("%n", namespace)
		line = line.replace("%N", namespaceCap)
		fout.write(line)
		
	fin.close()
	fout.close()


