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


#ifndef o3d_io_O3dFileManager_hpp
#define o3d_io_O3dFileManager_hpp

#include "io/stream/FileManager.hpp"
#include "util/type/util_type.hpp"


namespace se_ogre {
	class O3dFileManager : public se_core::FileManager {
	public:
		O3dFileManager(const char* dataPath);
		~O3dFileManager();
		void init();

		se_core::InputStream* open(const char* filename);
		void close(se_core::InputStream*& is);

		int fileCount();
		const char* filename(int index);
		static bool isBinaryFile(const char* filename);
		static bool isTextFile(const char* filename);
		bool exists(const char* filename);

	private:
		void addFile(se_core::String* filename);

		const char* directory_;
		short fileCount_;
		static const short MAX_FILE_COUNT = 512;
		se_core::String* files_[ MAX_FILE_COUNT ];
	};
}

#endif