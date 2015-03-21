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


#include "Property.hpp"

namespace se_core {

	Property
	::Property(const char* name, short value)
			: name_(name, 0), key_(hash(name)), type_(PT_SHORT) {
		v.shortValue_ = value;
	}

	Property
	::Property(const char* name, int value)
			: name_(name, 0), key_(hash(name)), type_(PT_INT) {
		v.intValue_ = value;
	}

	Property
	::Property(const char* name, float value)
			: name_(name, 0), key_(hash(name)), type_(PT_FLOAT) {
		v.floatValue_ = value;
	}

	Property
	::Property(const char* name, String* value)
			: name_(name, 0), key_(hash(name)), type_(PT_STRING) {
		v.string_ = value;
		if(value)
			valueHash_ = hash(value->get());
		else
			valueHash_ = 0;
	}

	Property
	::Property(const char* name, unsigned int count, String* values)
			: name_(name, 0), key_(hash(name)), type_(PT_STRING_LIST) {
		v.string_ = values;
		valueHash_ = count;
	}

	Property
	::Property(const char* name, const Action* value)
			: name_(name, 0), key_(hash(name)), type_(PT_ACTION) {
		v.action_ = value;
	}
	
	Property
	::Property(const char* name, const Script* value)
			: name_(name, 0), key_(hash(name)), type_(PT_SCRIPT) {
		v.script_ = value;
	}
	
	Property
	::Property(const char* name, Area* value)
			: name_(name, 0), key_(hash(name)), type_(PT_AREA) {
		v.area_ = value;
	}

	Property
	::~Property() {
		if(type_ == PT_STRING) {
			delete v.string_;
		}
		if(type_ == PT_STRING_LIST) {
			delete[] v.string_;
		}
	}

	unsigned int Property
	::hash(const char* name) {
		unsigned int h = 2166136261;
		for(const char* n = name; *n != 0; ++n) {
			h *= 16777619 * h;
			h ^= *n;
		}
		h ^= h >> 16;
		return (h & 0xffff);
	}

}
