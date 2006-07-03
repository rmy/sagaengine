#!/usr/bin/python

import os
import os.path
import sys

class AllHppCreator:
	def __init__(self):
		self.dirs_ = dict()
		self.src_dir = "."


	def walkerCallback(self, dirname, nameList):
		relpath = dirname.replace(self.src_dir_, "", 1)
		for entry in nameList:
			if entry.endswith(".hpp"):
				if(entry == "all.hpp"):
				   continue
				if(entry == "use.hpp"):
				   continue
				d = dirname
				n = entry

				if(not self.dirs_.has_key(d)):
					self.dirs_[d] = "#ifndef " + relpath.replace("/", "_").replace("\\", "_").lstrip("_") + "_all_hpp\n"
					self.dirs_[d] += "#define " + relpath.replace("/", "_").replace("\\", "_").lstrip("_") + "_all_hpp\n\n"
				self.dirs_[d] += "#include \"" + n + "\"\n"


	def walk(self, dir):
		self.src_dir_ = dir
		os.path.walk(dir, AllHppCreator.walkerCallback, self)

	def dump(self):
		for dir in self.dirs_.keys():
			name = os.path.join(dir, "all.hpp")
			f = open(name, "w")
			f.write(self.dirs_[ dir ])
			f.write("\n#endif\n")
			f.close()

if(len(sys.argv) < 2):
	dirname = "./"
else:
	dirname = sys.argv[1]

w = AllHppCreator()

w.walk(dirname)
w.dump()
