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


#include "ThingData.hpp"
#include "../sim.hpp"


namespace se_core {
	ThingData
	::ThingData() {
		for(int i = 0; i < SV_COUNT; ++i)
			singleValues_[i] = 0;
	}

	String& ThingData
	::attribute(short type) {
		return attributes_[type];
	}


	const String& ThingData
	::attribute(short type) const {
		return attributes_[type];
	}

}
