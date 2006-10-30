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


#ifndef SimComponentManager_hpp
#define SimComponentManager_hpp

#include "sim.hpp"
#include "thing/sim_thing.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"

namespace se_core {

	/**
	 * Base class for functionality component managers.
	 */
	class _SeCoreExport SimComponentManager {
	public:
		/**
		 * Construct SimComponentManager.
		 *
		 * @param type The type of SimComponentManager (gct_ACTOR, gct_PHYSICS, etc)
		 */
		SimComponentManager(enum SimComponentType type)
			: type_(type) {
		}


		/**
		 * Destructor.
		 */
		virtual ~SimComponentManager() {
		}


		virtual void step(long when) = 0;

	private:
		/**
		 * The type of SimComponent.
		 */
		enum SimComponentType type_;

	};

}

#endif
