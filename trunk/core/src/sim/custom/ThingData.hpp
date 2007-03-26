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


#ifndef ThingData_hpp
#define ThingData_hpp

#include "../sim.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"
#include "util/type/String.hpp"

namespace se_core {
	class _SeCoreExport ThingData {
	public:
		ThingData();
		virtual ~ThingData() {}
		short singleValue(short type) const { return singleValues_[type]; }

		String& attribute(short type);
		const String& attribute(short type) const;

		void setSingleValue(short type, short value) { singleValues_[type] = value; }
		void incrementSingleValue(short type) { ++singleValues_[type]; }
		void decrementSingleValue(short type) { --singleValues_[type]; }

	protected:
		static const int MGO_COUNT = 20;
		static const int SV_COUNT = 20;
		static const int ATT_COUNT = 20;

		mutable short singleValues_[SV_COUNT];
		String attributes_[ATT_COUNT];

	};

}

#endif
