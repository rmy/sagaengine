#!/usr/bin/python

import os
import sys
from FileWalker import FileWalker

# Modules are now in the modules.txt file in the script directory
# Read it from disk

basefile = sys.argv[1].strip(" /\\.")
template = sys.argv[2].strip("")

modules = list()
fd = open(basefile + "_modules.txt", "r")
for line in fd.readlines():
	modules.append( line.strip() )
fd.close()

includes = list()
fd = open(basefile + "_includes.txt", "r")
for line in fd.readlines():
	includes.append( line.strip() )
fd.close()

defines = ""
fd = open(basefile + "_defines.txt", "r")
for line in fd.readlines():
	defines += line.strip() + ";"
fd.close()

libraries = ""
fd = open(basefile + "_libraries.txt", "r")
for line in fd.readlines():
	libraries += line.strip() + ".lib "
fd.close()

# The root path which all other directories are relative to
SRC_DIR = os.curdir

def dumpChildren(dirs, children, name):
	# Result starts as empty
	res = ""

	# If name is empty, then this is the root node -
	# don't create subdirectory for that
	# Uncomment to enable subdirectories.
	if(name != ""):
		res = """<Filter Name="%s" Filter="cpp;hpp">""" % (name.replace("/", "%").replace("\\", "%").split("%")[-1])
	res += dirs.get(name, "")

	# Sort children directories
	sortedDirs = children.keys()
	sortedDirs.sort()

	# iterate through all children directories in sorted dirs
	for d in sortedDirs:
		# Avoid doing root directory twice
		if(d == ""):
			continue
		if(name != ""):
			res += dumpChildren(dirs, children[d], name + "/" + d)
		else:
			res += dumpChildren(dirs, children[d], d)
	
	# If this is not the root node, then close it's folder
	# Uncomment to enable subdirectories.
	if(name != ""):
		res += """</Filter>"""

	# Return the result
	return res

if __name__ == '__main__':
	# Read template file
	basefile = sys.argv[1].strip(" /\\.");
	base = open(template + ".vctmpl", 'r').read();

	# Include paths implied by module paths
	inc = ""
	for m in modules:
		inc += m + ";"
	for m in includes:
		inc += m + ";"


	# Read all files in directory structure in modules
	walker = FileWalker(SRC_DIR, "")
	# Iterate through modules
	for m in modules:
		walker.setPrefix(SRC_DIR, m)
		# Read all cpp files in this module
		os.path.walk(walker.src_dir_, FileWalker.Cpp, walker)
		# Read all hpp files in this module
		os.path.walk(walker.src_dir_, FileWalker.Hpp, walker)
	
	# Get all files from all modules in a single dict
	# Key in files are full path with dir and filename
	# Value is CppFile() object from saga.FileWalker
	files = walker.files_

	# Sort files
	fs = files.keys()
	fs.sort()

	# Create dictionary for holding directories
	dirs = dict()
	# Create dictionary for holding files belonging to a directory
	children = dict()
	
	for f in fs:
		# Get and xml format a file in vcproj format
		n = """<File RelativePath="%s"> </File>""" % files[f].getPrefixedName().replace("/", "\\")

		# Start at root children array
		c = children
		# Iterate throug all directories in the path
		for part in ("/" + files[f].dirname_).replace("\\", "/").split("/"):
			# Get or create the dictionary for this subdirectory
			tmp = c.get( part, dict() )
			# Add it to children
			c[part] = tmp
			# This subdirectory is the parent directory
			# for the next level of the path
			c = tmp

		# Append the file to the directory where it belongs
		dirname = files[f].dirname_.replace("\\", "/") #.replace("/", "_")
		dirs[ dirname ] = dirs.get( dirname, "" ) + n + "\n"

	res = dumpChildren(dirs, children.get("", dict()), "")
	# If you get an error here, it is probably because
	# there is something wrong with the vctmpl file.
	# There must be 5 places with %s in it
	h = int(hash(basefile))
	if(h < 0):
		h = -h

	print base % (basefile, h, basefile, libraries, inc, defines, libraries, inc, defines, res)
	
