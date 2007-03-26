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


#include "Object.hpp"
#include "comp/list/ObjectRepository.hpp"
#include "comp/schema/CompSchema.hpp"

namespace se_core {
	Object
	::Object(int type, const char* name)
			: type_(type), name_(name) {
		Assert(name);
		id_ = hash(type, name);
		CompSchema::objectRepository().add(this);
	}

	int Object
	::hash(int type, const char* name) {
		int h = 2166136261;
		h *= 16777619 * type;
		h ^= type;
		for(const char* n = name; *n != 0; ++n) {
			h *= 16777619;
			h ^= *n;
		}
		//h ^= h >> 16;
		return (h & 0xffffffff);
	}


	se_err::Log& operator<< (se_err::Log& log, const Object& o) {
		log << o.name() << "(" << o.type() << ", " << o.id() << ")";
		return log;
	}
}
