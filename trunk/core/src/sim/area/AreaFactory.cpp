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


#include "AreaFactory.hpp"
#include "Area.hpp"
#include "AreaManager.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"
#include "../schema/SimSchema.hpp"
#include "util/type/all.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>

namespace se_core {

	AreaFactory
	::AreaFactory(String* name)
			: name_(name) {
	}


	AreaFactory
	::~AreaFactory() {
		delete name_;
	}


	void AreaFactory
	::release(Area* a) {
		delete a;
	}


	const char* AreaFactory
	::name() const {
		return name_->get();
	}


}
