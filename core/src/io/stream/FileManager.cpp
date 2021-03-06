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


#include "FileManager.hpp"
#include "../encode/EncodeManager.hpp"
#include "../schema/IoSchema.hpp"
#include "util/error/Log.hpp"
#include <cstdio>
#include <cstring>



namespace se_core {

	FileManager
	::FileManager() {
	}


	FileManager
	::~FileManager() {
	}


	void FileManager
	::load(const char* filename) {
		InputStream* is = 0;
		if((is = IoSchema::fileManager->open(filename)) != 0) {
			bool success = IoSchema::parser().parse(*is);
			AssertWarning(success, "Failed loading " << filename);
			close(is);
		}
	}

	void FileManager
	::save(const char* filename) {
		OutputStream* os = 0;
		if((os = IoSchema::fileManager->openOutput(filename)) != 0) {
			IoSchema::encoder().encode(*os);
			closeOutput(os);
			// Save thumb as well

			static const int MAX_THUMB_LENGTH = 255;
			char thumbname[MAX_THUMB_LENGTH + 1];
			AssertFatal(strlen(filename) < (MAX_THUMB_LENGTH - sizeof(".thumb.txt")), "Thumb name too long for filename %s" << filename);
			sprintf(thumbname, "%s.thumb.txt", filename);
			if((os = IoSchema::fileManager->openOutput(thumbname)) != 0) {
				IoSchema::encoder().encodeThumb(*os);
				closeOutput(os);
			}
		}
	}

	void FileManager
	::loadDirectory(const char* directory) {
		int len = static_cast<int>(strlen(directory));
		for(int i = 0; i < IoSchema::fileManager->fileCount(); ++i) {
			const char* filename = IoSchema::fileManager->filename(i);
			if(strncmp(filename, directory, len) == 0) {
				InputStream* is = 0;
				if((is = IoSchema::fileManager->open(filename)) != 0) {
					bool success = IoSchema::parser().parse(*is);
					AssertWarning(success, "Failed parsing " << filename);
					close(is);
				}
			}
		}
	}


	void FileManager
	::loadBatch(const char* batchFilename) {
		InputStream* initIs = IoSchema::fileManager->open(batchFilename);
		char filename[256];

		while(true) {
			initIs->readLine(filename, 256);
			if(filename[0] == 0 || filename[0] == 'Q') {
				break;
			}
			if(filename[0] == 0 || filename[0] == '\n' || filename[0] == ' ') {
				continue;
			}

			InputStream* is = 0;
			if((is = IoSchema::fileManager->open(filename)) != 0) {
				bool success = IoSchema::parser().parse(*is);
				AssertWarning(success, "Failed parsing " << filename);
				IoSchema::fileManager->close(is);
				LogDetail("Finished loading batch: " << filename);
				continue;
			}

			LogFatal("Couldn't open file: " << filename);
		}
		close(initIs);
	}


}
