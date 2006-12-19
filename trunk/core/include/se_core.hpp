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


#ifndef se_core_hpp
#define se_core_hpp

#include "../src/sim/all.hpp"
#include "../src/sim/area/all.hpp"
#include "../src/sim/schema/all.hpp"
#include "../src/sim/thing/all.hpp"
#include "../src/sim/config/all.hpp"
#include "../src/sim/action/all.hpp"
#include "../src/sim/script/all.hpp"
#include "../src/sim/react/all.hpp"
#include "../src/sim/message/all.hpp"
#include "../src/sim/physics/all.hpp"
#include "../src/sim/pos/all.hpp"
#include "../src/sim/stat/all.hpp"
#include "../src/io/parse/all.hpp"
#include "../src/io/schema/all.hpp"
#include "../src/io/stream/all.hpp"
#include "../src/io/encode/all.hpp"
#include "../src/util/all.hpp"
#include "../src/util/config/all.hpp"
#include "../src/util/system/all.hpp"
#include "../src/util/type/all.hpp"
#include "../src/util/math/all.hpp"
#include "../src/util/task/all.hpp"
#include "../src/util/template/all.hpp"
#include "../src/util/vecmath/all.hpp"
#include "../src/util/error/all.hpp"
#include "../src/util/noise/all.hpp"

namespace se_core {

	inline bool initSagaEngine(const char* appName) {
		if(!SimSchema::init(appName)) {
			return false;
		}
		if(!IoSchema::init()) {
			return false;
		}
		return true;
	}


	inline void cleanupSagaEngine() {
		IoSchema::cleanup();
		SimSchema::cleanup();
	}
}


#endif
