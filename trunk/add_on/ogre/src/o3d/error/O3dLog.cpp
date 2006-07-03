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


#include "O3dLog.hpp"
#ifdef IS_OGRE
#include "o3d/schema/O3dSchema.hpp"
#  include <OgreException.h>
#  include <OgreLogManager.h>
#endif
#ifdef _WINDOWS
#  include <Windows.h>
#endif
#include <iostream>
#include <ctime>
#include <cstdio>

namespace se_err {
	/** Must be defined in platform dependent module */
	void scream3(const char* file, int line, const char* msg) {
		// Fatal
		char* buffer = new char[512];
		sprintf(buffer, "Fat: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);

#	ifdef _WINDOWS
		// Unhandled exception throws to debugger in Visual C++.
		throw 0.0f;
#   else
		// Throws gdb into debugger
		assert(false);
#	endif

		throw buffer;
		//throw( Ogre::Exception( Ogre::Exception::ERR_RT_ASSERTION_FAILED, msg, "LogFatal", const_cast<char*>(file), line) );
	}


	void say3(const char* file, int line, const char* msg) {
		// Msg
		char buffer[512];
		sprintf(buffer, "Msg: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);
	}


	void dump(const char *msg) {
#	ifdef _WINDOWS
		//OutputDebugString(msg);
		//OutputDebugString("\n");
#	endif
		if(se_ogre::O3dSchema::sceneManager)
			Ogre::LogManager::getSingleton().logMessage(msg);
		else
			std::cerr << msg << std::endl;
	}
}
