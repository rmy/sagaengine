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


#ifndef engine_stat_Property_hpp
#define engine_stat_Property_hpp

#include "util/error/Log.hpp"
#include "util/type/util_type.hpp"
#include "sim/action/sim_action.hpp"
#include "sim/area/sim_area.hpp"
#include "sim/thing/sim_thing.hpp"


namespace se_core {
	class Property {
	public:
		enum Type {
			PT_NONE, PT_SHORT, PT_INT, PT_FLOAT, PT_ACTOR, PT_ACTION, PT_AREA, PT_STRING
		};

		Property() {}
		Property(Type type);

		void setType(Type type) {
			type_ = type;
		}

		inline short* shortValue() {
			Assert(type_ == PT_SHORT);
			return v.shortValue_;
		}

		inline int* intValue() {
			Assert(type_ == PT_INT);
			return v.intValue_;
		}

		inline float* floatValue() {
			Assert(type_ == PT_FLOAT);
			return v.floatValue_;
		}

		inline String* string() {
			Assert(type_ == PT_STRING);
			return v.string_;
		}

		inline Actor* actor() {
			Assert(type_ == PT_ACTOR);
			return v.actor_;
		}

		inline const Action* action() {
			Assert(type_ == PT_ACTION);
			return v.action_;
		}

		inline Area* area() {
			Assert(type_ == PT_AREA);
			return v.area_;
		}

	private:
		Type type_;

		union {
			void* value_;
			Actor* actor_;
			Area* area_;
			const Action* action_;
			String* string_;
			short* shortValue_;
			int* intValue_;
			float* floatValue_;
		} v;
	};
}

#endif
