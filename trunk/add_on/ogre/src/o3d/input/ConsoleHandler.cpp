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


#include "ConsoleHandler.hpp"
#include "Console.hpp"
#include <clocale>
#include <cctype>

namespace se_ogre {

	/**
	 * Constructors and Destructor
	 */
	ConsoleHandler
	::ConsoleHandler() : console_(0) {
	}

	ConsoleHandler
	::~ConsoleHandler()
	{
	}

	/**
	 * Output
	 */
	bool ConsoleHandler
	::output(const char* msg) {
		if(!console_) return false;

		console_->output(msg);
		return true;
	}



	bool ConsoleHandler
	::hideConsole() {
		if(!console_)
			return false;

		if(console_->isFocused())
			console_->flipFocus();
		return true;
	}

}

