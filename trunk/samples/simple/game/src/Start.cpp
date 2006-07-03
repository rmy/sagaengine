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


#include "game/app/Application.hpp"
#include <exception>
#include <stdexcept>
#include <iostream>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDOWS
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
	try {
		// Create application object
		game::Application app;
		if(!app.init()) {
			throw "Init failed";
		}
		app.go();
		app.cleanup();

	} catch( char* msg ) {
		std::cerr << "An exception has occured: " <<
			msg << std::endl;
	} catch( const char* msg ) {
		std::cerr << "An exception has occured: " <<
			msg << std::endl;
	}
	catch(std::runtime_error& e) {
		std::cerr << "Runtime error:" << e.what() << std::endl;
	}
	catch(std::logic_error& e) {
		std::cerr << "Logic error: " << e.what() << std::endl;
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	/*
	catch(...) {
		std::cerr << "Unknown exception. Exiting" << std::endl;
	}
	*/
	return 0;
}

#ifdef __cplusplus
}
#endif
