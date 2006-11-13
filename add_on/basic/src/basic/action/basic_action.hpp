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


#ifndef basic_action_hpp
#define basic_action_hpp

namespace se_basic {
	/**
	 * Action channels.
	 * Each action is performed in one of several channels.
	 * An se_core::Actor may have only one Action in each channel in
	 * the ActionQueue, but it may have actions that are performed
	 * in several channels.
	 */
	//enum ActionChannel {
	//	CHANNEL_DIRECTION, CHANNEL_MOVEMENT, CHANNEL_EXTRA, CHANNEL_COUNT
	//};

}

#endif