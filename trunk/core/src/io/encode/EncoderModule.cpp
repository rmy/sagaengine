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


#include "Encoder.hpp"
#include "EncoderModule.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>

namespace se_core {
	EncoderModule
	::EncoderModule(Encoder &encoder, unsigned char group, unsigned char code, int version)
			: moduleGroup_(group), moduleCode_(code), moduleVersion_(version) {
		encoder.add(*this);
	}


	EncoderModule
	::EncoderModule(unsigned char group, unsigned char code, int version)
		 : moduleGroup_(group), moduleCode_(code), moduleVersion_(version) {
	}

	EncoderModule
	::~EncoderModule() {
	}

	unsigned int EncoderModule
	::headerCode(char moduleGroup, char moduleCode, int moduleVersion) const {
		char buffer[5];
		sprintf(buffer, "%c%c%02d", moduleGroup, moduleCode, moduleVersion);
		int code = buffer[3] + buffer[2] * 256 + buffer[1] * 65536 + buffer[0] * 65536 * 256;
		LogDetail((sprintf(log_msg(), "HeaderCode: %x", code), log_msg()));
		return code;
	}

	int EncoderModule
	::headerCode() const {
		return headerCode(moduleGroup_, moduleCode_, moduleVersion_);
	}
}

