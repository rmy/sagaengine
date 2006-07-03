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


#include "MessageCentral.hpp"
#include "MessageListener.hpp"
#include "../thing/Actor.hpp"
#include "../../util/error/Log.hpp"


namespace se_core {

	MessageCentral
	::MessageCentral() {
	}

	MessageCentral
	::~MessageCentral() {
	}


	void MessageCentral
	::addListener(MessageListener &l) {
		Assert(listenerCount < MAX_LISTENERS && "Added one listener too many");
		listeners[ listenerCount++ ] = &l;
	}


	void MessageCentral
	::removeListener(MessageListener &l) {
		for(int i = 0; i < listenerCount; ++i) {
			if(listeners[ i ] == &l) {
				listeners[ i ] = listeners[ --listenerCount ];
			}
		}
	}


	void MessageCentral
	::info(char* msg) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->infoEvent(msg);
		}
	}


	void MessageCentral
	::speech(Actor& speaker, const char* message) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->speechEvent(speaker, message);
		}
	}

}
