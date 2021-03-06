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


#ifndef base_system_hpp
#define base_system_hpp

#ifdef IS_GBA
 #include "gba/system/gba_system.hpp"
#else
#define OBJECT_IN_EWRAM
#define VAR_IN_EWRAM
#endif

#define SE_AUTO_INIT __attribute__((constructor)) 
#define SE_AUTO_CLEANUP __attribute__((destructor)) 

namespace se_core {
	class RealClock;
}

#endif
