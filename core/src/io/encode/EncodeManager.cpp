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


#include "EncodeManager.hpp"
#include "Encoder.hpp"
#include "../parse/Parser.hpp"
#include "../../sim/thing/Actor.hpp"
#include "../stream/io_stream.hpp"
#include "../stream/OutputStream.hpp"

namespace se_core {


	EncodeManager
	::EncodeManager()
		: Encoder(Parser::ENGINE, Parser::EMBEDDED, 1), moduleCount_(0), lastModule_(0), thumbModule_(0) {
	}


	EncodeManager
	::~EncodeManager() {
	}


	void EncodeManager
	::encode(OutputStream& out) {
		out.writeHeaderCode(headerCode());
		for(int i = 0; i < moduleCount_; ++i) {
			Assert(modules_[i]->type() == et_NORMAL);
			modules_[i]->encode(out);
		}
		if(lastModule_) {
			lastModule_->encode(out);
		}
	}

	void EncodeManager
	::encodeThumb(OutputStream& out) {
		AssertFatal(thumbModule_ != 0, "No thumb encoder module");
		thumbModule_->encode(out);
	}

	void EncodeManager
	::encode(OutputStream& out, Encoder& m) {
		m.encode(out);
	}


	void EncodeManager
	::add(Encoder& m) {
		Assert(MAX_MODULES > moduleCount_ + 1);
		switch(m.type()) {
		case Encoder::et_NORMAL:
			LogWarning("Added module: " << moduleCount_);
			modules_[ moduleCount_++ ] = &m;
			break;

		case Encoder::et_LAST:
			LogWarning("Added last module");
			lastModule_ = &m;
			break;

		case Encoder::et_THUMB:
			LogWarning("Added thumb module");
			thumbModule_ = &m;
			break;
		}
	}

}

