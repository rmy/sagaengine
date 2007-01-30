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
#include "../../sim/thing/Actor.hpp"
#include "../stream/io_stream.hpp"
#include "../stream/OutputStream.hpp"

namespace se_core {


	Encoder
	::Encoder()
		: module_(0) {
	}


	Encoder
	::~Encoder() {
	}


	void Encoder
	::encode(OutputStream& out) {
		Assert(module_);
		module_->encode(out);
	}

	void Encoder
	::add(EncoderModule& m) {
		Assert(module_ == 0);
		module_ = &m;
	}

}

