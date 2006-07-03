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


#include "PCFileManager.hpp"
#include "all.hpp"
#include "io/stream/all.hpp"
#include "util/error/Log.hpp"
#include "util/type/TmpString.hpp"
#include "util/type/String.hpp"
#include <cstdio>
#include <cstring>

using namespace se_core;


namespace se_ncurs {
	PCFileManager
	::PCFileManager() : directory_("../../data"), fileCount_(0) {
		InputStream* is;
		String* s = new String();

		const char* files[] = { "core/files.txt", "client/files.txt", "ogre/files.txt", 0 };

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


	PCFileManager
	::~PCFileManager() {
		while(fileCount_ > 0) {
			delete files_[ --fileCount_ ];
		}
	}


	bool PCFileManager
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



	bool PCFileManager
	::isTextFile(const char* filename) {
		const char* s = filename;
		short len = static_cast<short>(strlen(s));

		if(s[ len - 3 ] != 't')
			return false;
		if(s[ len - 2 ] != 'x')
			return false;
		if(s[ len - 1 ] != 't')
			return false;
		return true;
	}



	se_core::InputStream* PCFileManager
	::open(const char* filename) {
		if(isBinaryFile(filename)) {
			return new PCBinaryInputStream(directory_, filename);
		}
		else if(isTextFile(filename)) {
			return new PCTextInputStream(directory_, filename);
		}
		else {
			char f[256];
			sprintf(f, "%s.bin", filename);
			if(exists(f)) {
				return new PCBinaryInputStream(directory_, f);
			}
			sprintf(f, "%s.txt", filename);
			if(exists(f)) {
				return new PCTextInputStream(directory_, f);
			}
		}
		return 0;
	}


	void PCFileManager
	::close(InputStream*& is) {
		delete is;
		is = 0;
	}


	bool PCFileManager
	::exists(const char* filename) {
		for(int i = 0; i < fileCount_; ++i) {
			if(strcmp(filename, files_[i]->get()) == 0) return true;
		}
		return false;
	}


	int PCFileManager
	::fileCount() {
		return fileCount_;
	}


	const char* PCFileManager
	::filename(int index) {
		//printf("%d %s\n", index, files_[ index ]->get());
		return files_[ index ]->get();
	}


	void PCFileManager
	::addFile(String* filename) {
		Assert(fileCount_ < MAX_FILE_COUNT - 1);
		files_[ fileCount_++ ] = filename;
	}

}
