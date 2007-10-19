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
#include "sim/script/sim_script.hpp"
#include "sim/thing/sim_thing.hpp"
#include "comp/comp.hpp"


namespace se_core {
	class _SeCoreExport Property {
	public:
		enum Type {
			PT_NONE, PT_SHORT, PT_INT, PT_HASH, PT_FLOAT, PT_SCRIPT, PT_ACTION, PT_AREA, PT_STRING, PT_STRING_LIST, PT_END_OF_TYPES
		};
		static unsigned int hash(const char* name);

	private:
		/** Hash of name. */
		unsigned int key_;
		/** Hash of value. */
		unsigned int valueHash_; // 
		Type type_;

		union {
			void* value_;
			const Action* action_;
			const Script* script_;
			Area* area_;
			String* string_;
			short shortValue_;
			int intValue_;
			float  floatValue_;
		} v;

	public:
		Property(const char* name, short value);
		Property(const char* name, int value);
		Property(const char* name, float value);
		Property(const char* name, String* value);
		Property(const char* name, unsigned int count, String* values);
		Property(const char* name, const Action* value);
		Property(const char* name, const Script* value);
		Property(const char* name, Area* value);
		~Property();

		inline Type type() const { 
			return type_; 
		}
		unsigned int key() const {
			return key_;
		}

		inline short shortValue() const {
			Assert(type_ == PT_SHORT);
			return v.shortValue_;
		}

		inline int intValue() const {
			Assert(type_ == PT_INT);
			return v.intValue_;
		}

		inline unsigned int hashValue() const {
			Assert(type_ == PT_STRING);
			return valueHash_;
		}

		inline float floatValue() const {
			Assert(type_ == PT_FLOAT);
			return v.floatValue_;
		}

		inline const char* string() const {
			Assert(type_ == PT_STRING);
			return v.string_->get();
		}

		inline unsigned int valueCount() const {
			if(type_ == PT_STRING) {
				return 1;
			}
			return valueHash_;
		}

		inline const char* string(unsigned int index) const {
			Assert(type_ == PT_STRING_LIST || type_ == PT_STRING);
			Assert(index < valueCount());
			return v.string_[index].get();
		}

		inline const Action* action() const {
			Assert(type_ == PT_ACTION);
			return v.action_;
		}

		inline const Script* script() const {
			Assert(type_ == PT_SCRIPT);
			return v.script_;
		}

		inline Area* area() const {
			Assert(type_ == PT_AREA);
			return v.area_;
		}

	};
}

#endif
