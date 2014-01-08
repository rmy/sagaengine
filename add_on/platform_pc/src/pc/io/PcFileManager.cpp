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


#include "PcFileManager.hpp"
#include "all.hpp"
#include "io/stream/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/error/Log.hpp"
#include "util/type/TmpString.hpp"
#include "util/type/String.hpp"
#include <cstdio>
#include <cstring>
#include <cctype>

#ifdef _WIN32
 #include <shlobj.h>
 #include <windows.h>
#endif
#ifdef linux
 #include <unistd.h>
 #include <pwd.h>
 #include <sys/stat.h>
#endif


using namespace se_core;


namespace se_pc {
	PcFileManager
	::PcFileManager(const char* dataPathFile)
			: fileCount_(0) {
		initSavePath();
		loadDataPath(dataPathFile);
	}


	PcFileManager
	::~PcFileManager() {
		clear();
	}

  
#ifdef _WIN32
	void PcFileManager
	::initSavePath() {
		// Windows 7 doesn't allow applications to save in program folder.
		char appDir[ MAX_PATH_LENGTH];
		SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, appDir);
		sprintf(saveDirectory_, "%s\\%s", appDir, "Tootinis");
		CreateDirectory(saveDirectory_, NULL);
		char subDir[ MAX_PATH_LENGTH];
		sprintf(subDir, "%s\\%s", saveDirectory_, "save");
		CreateDirectory(subDir, NULL);
	}

	const char* PcFileManager
	::savePath(char* dest, const char* filename) {
		sprintf(dest, "%s\\%s", saveDirectory_, filename);
		return dest;
	}

#endif

#ifdef linux
	void PcFileManager
	::initSavePath() {
		struct passwd *pw = getpwuid(getuid());
		const char *homeDir = pw->pw_dir;
		sprintf(saveDirectory_, "%s/%s", homeDir, ".tootinis");
		mkdir(saveDirectory_, 0777);

		char subDir[ MAX_PATH_LENGTH];
		sprintf(subDir, "%s/%s", saveDirectory_, "save");
		mkdir(subDir, 0777);
	}


	const char* PcFileManager
	::savePath(char* dest, const char* filename) {
		sprintf(dest, "%s/%s", saveDirectory_, filename);
		return dest;
	}


#endif


	void PcFileManager
	::loadDataPath(const char* dataPathFile) {
		FILE* in = fopen(dataPathFile, "r");
		if(!in) {
			LogFatal("Couldn't open: " << dataPathFile);
		}
		static char buffer[256];
		fgets(buffer, 256, in);
		size_t len = strlen(buffer);
		while(len > 0 && isspace( buffer[ len - 1 ] )) {
			--len;
			buffer[ len ] = 0;
		}
		dataDirectory_ = buffer;
		IoSchema::dataPath = buffer;
		fclose(in);
	}


	void PcFileManager
	::init(const char** files) {
		clear();

		InputStream* is;
		String* s = new String();

		for(const char** f = files; *f != 0; ++f) {
			is = open(*f);
			if(!is) {
				LogWarning("Couldn't open " << *f);
			}
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


	void PcFileManager
	::clear() {
		while(fileCount_ > 0) {
			delete files_[ --fileCount_ ];
		}
	}


	bool PcFileManager
	::isBinaryFile(const char* filename) {
		const char* s = filename;
		short len = static_cast<short>(strlen(s));
		if(len < 4)
			return false;

		if(s[ len - 4 ] != '.')
			return false;
		if(s[ len - 3 ] != 'b')
			return false;
		if(s[ len - 2 ] != 'i')
			return false;
		if(s[ len - 1 ] != 'n')
			return false;
		return true;
	}



	bool PcFileManager
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



	se_core::InputStream* PcFileManager
	::open(const char* filename) {
		if(isBinaryFile(filename)) {
			return new PcBinaryInputStream(directory(filename), filename);
		}
		else if(isTextFile(filename)) {
			return new PcTextInputStream(directory(filename), filename);
		}
		else {
			char f[256];
			sprintf(f, "%s.bin", filename);
			if(exists(f)) {
				return new PcBinaryInputStream(directory(f), f);
			}
			sprintf(f, "%s.txt", filename);
			if(exists(f)) {
				return new PcTextInputStream(directory(f), f);
			}
		}
		return 0;
	}


	void PcFileManager
	::close(InputStream*& is) {
		delete is;
		is = 0;
	}


	se_core::OutputStream* PcFileManager
	::openOutput(const char* filename) {
		return new PcTextOutputStream(directory(filename), filename);
	}


	void PcFileManager
	::closeOutput(OutputStream*& os) {
		delete os;
		os = 0;
	}


	bool PcFileManager
	::exists(const char* filename) const {
		for(int i = 0; i < fileCount_; ++i) {
			if(strcmp(filename, files_[i]->get()) == 0) return true;
		}
		return false;
	}


	int PcFileManager
	::findFile(const char* filename) const {
		for(int i = 0; i < fileCount_; ++i) {
			if(strcmp(filename, files_[i]->get()) == 0) return i;
		}
		return -1;
	}


	int PcFileManager
	::fileCount() const {
		return fileCount_;
	}


	const char* PcFileManager
	::filename(int index) const {
		//printf("%d %s\n", index, files_[ index ]->get());
		return files_[ index ]->get();
	}


	bool PcFileManager
	::addFileIfExists(const char* filename) {
		char buffer[MAX_PATH_LENGTH];
		sprintf(buffer, "%s/%s", directory(filename), filename);
		FILE* in = fopen(buffer, "r");
		if(in) {
			String* s = new String();
			s->copy(filename);
			addFile(s);
			fclose(in);
			return true;
		}

		return false;
	}

	void PcFileManager
	::addFile(String* filename) {
		Assert(fileCount_ < MAX_FILE_COUNT - 1);
		files_[ fileCount_++ ] = filename;
	}


	void PcFileManager
	::removeFile(const char* filename) {
		Assert(fileCount_ > 0);
		int id = findFile(filename);
		if(id < 0)
			return;

		char src[MAX_PATH_LENGTH];
		sprintf(src, "%s/%s", directory(filename), filename);
		char dest[MAX_PATH_LENGTH];
		sprintf(dest, "%s/%s.bak", directory(filename), filename);
		remove(dest);
		rename(src, dest);

		files_[ id ] = files_[ --fileCount_ ];		
	}



	const char* PcFileManager
	::directory(const char* filename) const {
		if(strncmp("save/", filename, strlen("save/")) == 0) {
			return saveDirectory_;
		}
		return dataDirectory_;
	}
}
