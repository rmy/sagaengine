#!/usr/local/bin/python
from Tkinter import *

FROM_LANG = "EN"
TO_LANG = "NO"
OUT_FILE = "phrases." + TO_LANG + ".txt"
INPUT_FILES = ( "phrases.txt", OUT_FILE )

class ScrolledFrame(Frame):

	def cget(self,item):
		return Frame.cget(self,item) if not hasattr(self,item) else getattr(self,item)

	__getitem__ = cget

	def configure(self,cnf=None,**kw):
		if kw:cnf=Tkinter._cnfmerge((cnf,kw))
		for key in cnf.keys():
			Frame.configure(self,cnf) if not hasattr(self,key) else setattr(self,key,cnf[key])
	def __setitem__(self,item,value):self.configure({item:value})

	def __call__(self):
		return self.client

	def __init__(self,master=None,stretch=True,cnf={},**kw):
		Frame.__init__(self,master,cnf,**kw)
		self.grid(sticky=N+S+E+W)
		self.client=Frame(self,border=0)
		self.client.grid(sticky=N+S+E+W)

		top = self.winfo_toplevel()
		top.rowconfigure(0, weight=1)
		top.columnconfigure(0, weight=1)
		self.rowconfigure(0, weight=1)
		self.columnconfigure(0, weight=1)
		self.client.rowconfigure(0, weight=1)
		self.client.columnconfigure(0, weight=1)
		
		# width and height of Scrolledframe
		self.W=1.0
		self.H=1.0
		# top left corner coordinates of client frame
		self.client_x=0
		self.client_y=0
		# width and height of client frame
		self.client_w=1.0
		self.client_h=1.0
		# scrollcommands (default)
		self.xscrollcommand=lambda *args:None
		self.yscrollcommand=lambda *args:None
		# scrollincrements
		self.xscrollincrement=5
		self.yscrollincrement=5
		# stretches
		self.stretch=stretch
		self.stretch_x=stretch
		self.stretch_y=stretch

		self.bind("<Expose>",self.update_scrollregion)

	def xview(self,event,value,units='pages'):
		if event == "moveto":
			fraction=float(value)
			if fraction <= 0.0:self.client_x=0
			elif fraction >= float(self.client_w-self.W)/self.client_w:self.client_x=self.W-self.client_w
			else:self.client_x=int(-self.client_w*fraction)
		elif event == "scroll":
			amount=int(value)
			if self.client_x == 0 and amount < 0:return
			if self.client_x == self.W-self.client_w and amount > 0:return
			self.client_x=self.client_x-(amount*self.xscrollincrement if units == "units" else amount*self.W*0.99)
		else:return

		self.update_scrollx()
		self.client.place_configure(x=self.client_x)

	def yview(self,event,value,units='pages'):
		if event == "moveto":
			fraction=float(value)
			if fraction <= 0.0:self.client_y=0
			elif fraction >= float(self.client_h-self.H)/self.client_h:self.client_y=self.H-self.client_h
			else:self.client_y=int(-self.client_h*fraction)
		elif event == "scroll":
			amount=int(value)
			if self.client_y == 0 and amount < 0:return
			if self.client_x == self.H-self.client_h and amount > 0:return
			self.client_y=self.client_y-(amount*self.yscrollincrement if units == "units" else amount*self.H)
		else:return

		self.update_scrolly()
		self.client.place_configure(y=self.client_y)

	def update_scrollx(self,*args):
		low=0.0 if self.client_x >= 0 else -float(self.client_x)/self.client_w
		high=1.0 if self.client_x+self.client_w <= self.W else low+float(self.W)/self.client_w
		if low <= 0.0:
			self.client_x=0
		elif high >= 1.0:
			self.client_x=self.W-self.client_w
			low=-float(self.client_x)/self.client_w
		self.stretch_x=self.stretch if (self.client_w < self.W) else False
		self.xscrollcommand(low,high)

	def update_scrolly(self,*args):
		low=0.0 if self.client_y >= 0 else -float(self.client_y)/self.client_h
		high=1.0 if self.client_y+self.client_h <= self.H else low+float(self.H)/self.client_h
		if low <= 0.0:
			self.client_y=0
		elif high >= 1.0:
			self.client_y=self.H-self.client_h
			low=-float(self.client_y)/self.client_h
		self.stretch_y=self.stretch if (self.client_h < self.H) else False
		self.yscrollcommand(low,high)

	def update_scrollregion(self,*args):
		if len(self.client.children):
			self.client_w=self.client.winfo_reqwidth()
			self.client_h=self.client.winfo_reqheight()
			self.W=self.winfo_width()
			self.H=self.winfo_height()

			self.update_scrolly()
			self.update_scrollx()

			self.client.place_configure(
				anchor="nw",
				y=self.client_y,height=self.H if self.stretch_y else self.client_h,
				x=self.client_x,width=self.W if self.stretch_x else self.client_w
			)
		else:
			self.xscrollcommand(0.0,1.0)
			self.yscrollcommand(0.0,1.0)
			self.client.place_forget()		

class Grid(Frame):
	def __init__(self, master=None):
		Frame.__init__(self, master)
		self.grid(sticky=N+S+E+W)
		self.createWidgets()
		self.master = master
	
	def addRow(self, text="Dummy"):
		if(not text.startswith('L ')):
			return

		#print text
		tokens = text.split()
		name = tokens[2] + ": " + tokens[3]
		labels = list()
		col = 0
	
		if(not self.rows.has_key(name)):
			row = dict()
			self.rows[ name ] = row

			# Label for lang
			row[ "TOKENS" ] = tokens
			row[ "LANG" ] = StringVar()
			label = Label ( self.master, text=tokens[1], justify = LEFT, textvariable = row[ "LANG" ] )
			label.grid(row = self.r, column=col, sticky=W)
			col = col + 1
			row[ "LANG" ].set(">> XX " + name)
			labels.append( label )

			# Label for name
			#label = Label ( self.master, text=name, justify = LEFT )
			#label.grid(row = self.r, column=col, sticky=W)
			#col = col + 1
			#labels.append( label )

			# Edit box for test
			row[ "VAR" ] = StringVar()
			textEntry = Entry ( self.master, textvariable = row[ "VAR" ] )
			textEntry.grid(row = self.r, column=col, sticky=N+S+E+W)
			col = col + 1

			row[ "LABELS" ] = labels

		else:
			row = self.rows[ name ]

		row[ tokens[1] ] = tokens[4];
		value = row[ "VAR" ]
		lang = row[ "LANG" ]
		if(row.has_key( TO_LANG )):
			value.set(row[ TO_LANG ].replace("_", " "))
			lang.set("     " + TO_LANG + " " + name)
			row[ "TOKENS" ][1] = TO_LANG
			row[ "TOKENS" ][4] = row[ TO_LANG ]
		elif(row.has_key( FROM_LANG )):
			value.set(row[ FROM_LANG ].replace("_", " "))
			lang.set(">> " + FROM_LANG + " " + name)
			row[ "TOKENS" ][1] = FROM_LANG
			row[ "TOKENS" ][4] = row[ FROM_LANG ]

		self.r = self.r + 1


	def load(self, files):
		for filename in files:
			try:
				f = open( filename )
				data = f.read()

				lines = data.split("\n")
				for n in lines:
					self.addRow( text=n )

			except:
				print "Couldn't open %s" % (filename)
			finally:
				print "Closing %s" % (filename)
				f.close
			
			
	def save(self, filename):
		OUT = open( filename, "w" )
		OUT.write( "CL01\n" )
		for n in self.rows.keys():
			row = self.rows[ n ]
			tokens = row[ "TOKENS" ]
			newText = row[ "VAR" ].get().replace(" ", "_")
			#r = cmp(newText, tokens[4])
			#if(r != 0):
			#	print "<%s> != <%s> %d" % (newText, tokens[4], r)
			if(cmp(tokens[ 1 ], TO_LANG) != 0 and cmp(newText, tokens[4]) == 0):
				continue
			if(len(newText) == 0):
				continue
			line = "L " + TO_LANG + " " + tokens[ 2 ] + " " + tokens[ 3 ] + " " + newText + "\n"
			#print line
			OUT.write( line )
		OUT.write( "Q\n" )
		OUT.close

	def createWidgets(self):
		top=self.master.winfo_toplevel()
		top.rowconfigure(0, weight=1)
		top.columnconfigure(0, weight=1)

		self.master.rowconfigure(0, weight=1)
		self.master.columnconfigure(0, weight=1)
		self.master.columnconfigure(1, weight=12)
		#self.master.columnconfigure(2, weight=12)

		self.r = 0
		self.rows = dict()
		self.load(INPUT_FILES)




class Application(Frame):   
	def __init__(self, master=None, root=None):
		Frame.__init__(self, master)
		self.root = root
		self.grid(sticky=N+S+E+W)
		self.createWidgets()


	def createWidgets(self):
		top=self.winfo_toplevel()
		top.rowconfigure(0, weight=1)
		top.columnconfigure(0, weight=1)

		self.grid = Grid( self )
		self.grid.grid(row=0, column=0, sticky=N+S+E+W)


	def save(self):
		print "Saving"
		self.grid.save(OUT_FILE)
		quit()

root = Tk()
scrollframe = ScrolledFrame(root, stretch=True)
app = Application(scrollframe());

scrollx=Scrollbar(root,orient="horizontal",command=scrollframe.xview)
scrollx.grid(row=1,column=0,sticky="nwse")
scrolly=Scrollbar(root,orient="vertical",command=scrollframe.yview)
scrolly.grid(row=0,column=1,sticky="nwse")
scrollframe["xscrollcommand"]=scrollx.set
scrollframe["yscrollcommand"]=scrolly.set

#scrollframe.grid(row=0, column=0, sticky=N+S+E+W)

checkvar=BooleanVar(root,value=scrollframe["stretch"])
def set_stretch():
	scrollframe["stretch"]=True if checkvar.get() else False
	scrollframe.update_scrollregion()


quitButton = Button ( root, text="Save and exit", command=app.save )
quitButton.grid(sticky=N+S+E+W)

root.title("Translation tool")
root.columnconfigure(0,weight=1)
root.rowconfigure(0,weight=1)

root.mainloop()

app.save()
print "Exiting"


#app = ScrolledFrame(root, stretch=True)
#app = Application(root)
#app.master.title("Translation tool")
#app.mainloop()
