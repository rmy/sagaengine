#!/usr/bin/python

import os
import sys
import string

xSize = int(sys.argv[1])
ySize = int(sys.argv[2])

x = 0
while(x < xSize):
	y = 0
	while(y < ySize):
		for s in ("%d.%d_things.tmp.txt", "%d.%d_things.txt"):
			filename = s % (y, x)
			print filename
			fout = open(filename, "w")
			fout.write("XB01\n")
			fout.write("N %d.%d\n" % (y, x))
			fout.close()
		y = y + 1
		
	x = x + 1

	
