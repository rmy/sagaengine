import os
import sys


class CppFile:
    def __init__(self):
        self.dirlist_ = list()
        self.filename_ = "_.cpp"
        self.fullname_ = ""
        self.headers_ = dict()
        self.dependencies_ = dict()
        self.prefix_ = ""
        self.dirname_ = ""

    def getPrefixedName(self) :
        return os.path.join(self.prefix_, self.fullname_);

    def setPath(self, dirname, filename, prefix):
        """ set the directory and filename of this header file """
        #dirname = dirname.replace("/", "\\")
        self.dirlist_ = dirname.replace("\\", "/").split("/")
        if(self.dirlist_[0] == ""):
            self.dirlist_.pop(0)
        self.filename_ = filename
        for p in self.dirlist_:
            self.dirname_ = os.path.join(self.dirname_, p)
        self.fullname_ = os.path.join(self.dirname_, self.filename_)
        #sys.stderr.write(self.fullname_ + "\n")
        self.prefix_ = os.path.normpath(prefix);


    def addHeader(self, include):
        """ add include line to this header file """
        try:
            h = include.split("\"")[1]
            #p = h.split("/")
            #name = list(self.dirlist_)
            #while(p[0] == ".."):
            #    p.pop(0)
            #    name.pop()

            #name.extend(p)
            #n = name.pop()
            #fullname = ""
            #for p in name:
            #    fullname += p + "/"
            #fullname += n
            fullname = os.path.normpath(os.path.join(self.dirname_, h)).lstrip("/").lstrip("\\")
            self.headers_[ fullname ] = os.path.normpath(h)
        except IndexError:
            pass


    def getNestedDependencies(self, dependencies, hppFiles):
        """ get a list of dependencies """
        dependencies[ self.fullname_ ] = os.path.join(self.prefix_, self.fullname_)
        for h in self.headers_.keys():
            if(dependencies.has_key(h)):
                continue

            if(hppFiles.has_key(h)):
                hppFiles[ h ].getNestedDependencies(dependencies, hppFiles)
            else:
                h2 = self.headers_[ h ]
                if(hppFiles.has_key(h2)):
                    hppFiles[ h2 ].getNestedDependencies(dependencies, hppFiles)
                else:
                    sys.stderr.write ("Warning: Illegal include to %s in %s.\n" % (h2, self.dirname_))


    def calcHppDependencies(self, hppFiles):
        """ calc dependencies for Hpp files """
        self.getNestedDependencies(self.dependencies_, hppFiles)



class FileWalker:
    """
    Usage:
    cppWalker = FileWalker(SRC_DIR)
    os.path.walk(SRC_DIR, FileWalker.Cpp, cppWalker)
    cppFiles = cppWalker.files_
    """
    def __init__(self, src_dir, prefix = ""):
        self.src_dir_ = os.path.normpath(os.path.join(src_dir, prefix))
        self.files_ = dict()
        self.prefix_ = prefix


    def setPrefix(self, src_dir, prefix):
        self.src_dir_ = os.path.normpath(os.path.join(src_dir, prefix))
        self.prefix_ = prefix


    def Cpp(self, dirname, nameList):
        if(not dirname.startswith(self.src_dir_)):
            print dirname
            sys.exit()
        else:
            relpath = dirname.replace(self.src_dir_, "", 1)

        for entry in nameList:
            if entry.endswith(".cpp"):
                cpp = CppFile()
                cpp.setPath(relpath, entry, self.prefix_)
                f = open(os.path.join(dirname, entry), 'r', -1)
                for line in f.readlines():
                    if(line.startswith("#include")):
                        line = line.rstrip()
                        cpp.addHeader(line)
                f.close()
                self.files_[ cpp.fullname_ ] = cpp


    def Hpp(self, dirname, nameList):
        if(not dirname.startswith(self.src_dir_)):
            print dirname
            sys.exit()
        else:
            relpath = dirname.replace(self.src_dir_, "", 1)

        for entry in nameList:
            if entry.endswith(".hpp"):
                hpp = CppFile()
                hpp.setPath(relpath, entry, self.prefix_)
                f = open(os.path.join(dirname, entry), 'r', -1)
                for line in f.readlines():
                    if(line.startswith("#include")):
                        line = line.rstrip()
                        hpp.addHeader(line)
                f.close()
                self.files_[ hpp.fullname_ ] = hpp

