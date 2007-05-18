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


#ifndef pc_io_PcFileManager_hpp
#define pc_io_PcFileManager_hpp

#include "io/stream/FileManager.hpp"
#include "util/type/util_type.hpp"


namespace se_pc {
	class _SePcExport PcFileManager : public se_core::FileManager {
	public:
		PcFileManager(const char* dataPathFile);
		~PcFileManager();
		void init(const char** files);
		bool addFileIfExists(const char* filename);
		void clear();

		void loadDatapath(const char* dataPathFile);

		se_core::InputStream* open(const char* filename);
		void close(se_core::InputStream*& is);

		se_core::OutputStream* openOutput(const char* filename);
		void closeOutput(se_core::OutputStream*& is);

		int fileCount() const;
		const char* filename(int index) const;
		static bool isBinaryFile(const char* filename);
		static bool isTextFile(const char* filename);
		bool exists(const char* filename) const;

	private:
		void addFile(se_core::String* filename);

		const char* directory_;
		short fileCount_;
		static const short MAX_FILE_COUNT = 2048;
		se_core::String* files_[ MAX_FILE_COUNT ];
	};
}

#endif
