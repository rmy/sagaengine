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


#ifndef SimComposite_hpp
#define SimComposite_hpp

#include "sim.hpp"
#include "thing/sim_thing.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"

namespace se_core {

	/**
	 * Base class for functionality composites.
	 */
	class _SeCoreExport SimComposite {
	public:
		enum SimCompositeType { sct_UNDEFINED, sct_ACTOR, sct_SCRIPT, sct_PHYSICS, sct_NODE, sct_POS };

		/**
		 * Construct unnamed SimComposite.
		 *
		 * @param type The type of SimComposite (gct_ACTOR, gct_PHYSICS, etc)
		 */
		SimComposite(enum SimCompositeType type, Actor* owner)
			: type_(type), owner_(owner) {
		}


		/**
		 * Destructor.
		 */
		virtual ~SimComposite() {
		}


		inline Actor* owner() {
			return owner_;
		}

		bool isActive();
		bool isDead();

	protected:
		friend class SimCompositeOwner;
		/** Called by SimCompositeOwner
		 */
		virtual void setActive(bool state) {}

		/**
		 * The type of SimComposite.
		 */
		enum SimCompositeType type_;
		Actor* owner_;
	};

}

#endif