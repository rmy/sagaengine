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


#ifndef engine_action_SpawnChildren_hpp
#define engine_action_SpawnChildren_hpp

#include "sim/action/Action.hpp"

namespace se_basic {
	class _SeBasicExport SpawnChildren : public se_core::Action {
	public:
		SpawnChildren(const char* name, bool doSpawnChildren);

		short duration(se_core::ActionComponent& performer, se_core::Parameter& parameter) const {
			return 5;
		}

		bool isContinuing(se_core::ActionComponent &performer, se_core::Parameter& parameter) const {
			return false;
		}

		bool isRepeating(long when, se_core::ActionComponent &performer, se_core::Parameter& parameter) const {
			return false;
		}

		void perform(long when, se_core::ActionComponent& performer, se_core::Parameter& parameter) const;

		const SpawnChildren& param(const char* thingType, short SpawnChildrenPoint, se_core::Parameter& out) const;

	private:
		struct Param {
			Param(const char* thingType, short SpawnChildrenPoint)
				: thingType_(thingType), SpawnChildrenPoint_(SpawnChildrenPoint) {
			}
			const char* thingType_;
			short SpawnChildrenPoint_;
		};
		bool doSpawnChildren_;
		unsigned int PROPERTY_SPAWN;
		unsigned int PROPERTY_SPAWN_POINT;
	};


	extern _SeBasicExport const SpawnChildren actionSpawnChildren;
	extern _SeBasicExport const SpawnChildren actionSpawnSiblings;
}

#endif
