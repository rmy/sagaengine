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


#ifndef io_encode_Encoder_hpp
#define io_encode_Encoder_hpp

#include "../stream/io_stream.hpp"
#include "../encode/io_encode.hpp"

namespace se_core {
	/** Base class for encoder modules. */
	class _SeCoreExport Encoder {
	public:
		enum Type { et_NORMAL, et_LAST, et_THUMB, et_STANDALONE };

		Encoder(se_core::EncodeManager &encoder, unsigned char group, unsigned char code, int version, Type type = et_NORMAL);
		Encoder(unsigned char group, unsigned char code, int version);
		virtual ~Encoder();

		int headerCode() const;
		unsigned int headerCode(char moduleGroup, char moduleCode, int moduleVersion) const;
		virtual void encode(OutputStream& out) = 0;
		Type type() { return type_; }


	private:
		int moduleGroup_;
		int moduleCode_;
		int moduleVersion_;
		Type type_;
	};
}

#endif
