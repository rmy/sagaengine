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


#ifndef io_fs_FileManager_hpp
#define io_fs_FileManager_hpp

#include "io_stream.hpp"

namespace se_core {
	/** File system base class.
	 * This class should be subclassed in a platform dependent
	 * layer. This enables SagaEngine's data file architecture to both
	 * handle real file systems, network transfers, and file system
	 * emulation on ROMs.
	 *
	 * @see TextInputStream
	 * @see BinaryInputStream
	 */
	class _SeCoreExport FileManager {
	public:
		FileManager();
		virtual ~FileManager();
		virtual void init() {}
		virtual void cleanup() {}

		virtual InputStream* open(const char* filename) = 0;
		virtual void close(InputStream*& is) = 0;

		void load(const char* filename);

		/**
		 * Load and parse all the files in this
		 * directory and all subdirectories.
		 */
		void loadDirectory(const char* directory);

		/**
		 * Load and parse all the files in the
		 * batch file. The batch should contain
		 * one filename per line.
		 */
		void loadBatch(const char* batchFilename);

		virtual int fileCount() = 0;
		virtual const char* filename(int index) = 0;

	private:
	};
}

#endif
