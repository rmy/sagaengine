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


#ifndef io_encode_EncoderModule_hpp
#define io_encode_EncoderModule_hpp

#include "../stream/io_stream.hpp"

namespace se_core {
	class EncoderModule {
	public:
		/*
		EncoderModule();
		virtual ~EncoderModule();
		virtual void encode(OutputStream& out) = 0;
		*/
		unsigned int headerCode(char moduleGroup, char moduleCode, int moduleVersion);

	private:
	};
}

#endif