/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#include "O3dFileManager.hpp"
#include "all.hpp"
#include "io/stream/all.hpp"
#include "util/error/Log.hpp"
#include "util/type/TmpString.hpp"
#include "util/type/String.hpp"
#include <cstdio>
#include <cstring>
#include <cctype>

using namespace se_core;


namespace se_ogre {
	O3dFileManager
	::O3dFileManager(const char* dataPath)
			: fileCount_(0) {
		FILE* in = fopen(dataPath, "r");
		if(!in) {
			LogFatal("Couldn't open: " << dataPath);
		}
		static char buffer[256];
		fgets(buffer, 256, in);
		int len = strlen(buffer);
		while(len > 0 && isspace( buffer[ len - 1 ] )) {
			--len;
			buffer[ len ] = 0;
		}
		directory_ = buffer;
		fclose(in);
		// TODO: Throw exception if illegal datapath?
	}


	O3dFileManager
	::~O3dFileManager() {
		while(fileCount_ > 0) {
			delete files_[ --fileCount_ ];
		}
	}


	void O3dFileManager
	::init() {
		InputStream* is;
		String* s = new String();

		const char* files[] = { "game/files.txt", "ogre/files.txt", 0 };

		for(const char** f = files; *f != 0; ++f) {
			is = open(*f);
			while(!is->eof()) {
				is->readLine(*s);
				if(s->isEmpty()) {
					continue;
				}
				if(s->get()[0] == '#') {
					continue;
				}

				addFile(s);
				s = new String();
			}
			close(is);
		}
		delete s;
	}


	bool O3dFileManager
	::isBinaryFile(const char* filename) {
		const char* s = filename;
		short len = static_cast<short>(strlen(s));

		if(s[ len - 3 ] != 'b')
			return false;
		if(s[ len - 2 ] != 'i')
			return false;
		if(s[ len - 1 ] != 'n')
			return false;
		return true;
	}



	bool O3dFileManager
	::isTextFile(const char* filename) {
		return (!isBinaryFile(filename));
		/*
		const char* s = filename;
		short len = static_cast<short>(strlen(s));

		if(s[ len - 1 ] != 't')
			return false;
		if(s[ len - 2 ] != 'x')
			return false;
		if(s[ len - 3 ] != 't')
			return false;
			return true;
		*/
	}



	se_core::InputStream* O3dFileManager
	::open(const char* filename) {
		if(isBinaryFile(filename)) {
			return new O3dBinaryInputStream(directory_, filename);
		}
		else if(isTextFile(filename)) {
			return new O3dTextInputStream(directory_, filename);
		}
		else {
			char f[256];
			sprintf(f, "%s.bin", filename);
			if(exists(f)) {
				return new O3dBinaryInputStream(directory_, f);
			}
			sprintf(f, "%s.txt", filename);
			if(exists(f)) {
				return new O3dTextInputStream(directory_, f);
			}
		}
		return 0;
	}


	void O3dFileManager
	::close(InputStream*& is) {
		delete is;
		is = 0;
	}


	bool O3dFileManager
	::exists(const char* filename) {
		for(int i = 0; i < fileCount_; ++i) {
			if(strcmp(filename, files_[i]->get()) == 0) return true;
		}
		return false;
	}


	int O3dFileManager
	::fileCount() {
		return fileCount_;
	}


	const char* O3dFileManager
	::filename(int index) {
		//printf("%d %s\n", index, files_[ index ]->get());
		return files_[ index ]->get();
	}


	void O3dFileManager
	::addFile(String* filename) {
		Assert(fileCount_ < MAX_FILE_COUNT - 1);
		files_[ fileCount_++ ] = filename;
	}

}
