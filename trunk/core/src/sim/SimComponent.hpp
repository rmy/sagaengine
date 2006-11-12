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


#ifndef SimComponent_hpp
#define SimComponent_hpp

#include "sim.hpp"
#include "thing/sim_thing.hpp"
#include "area/sim_area.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"

namespace se_core {

	/**
	 * Base class for functionality composites.
	 */
	class _SeCoreExport SimComponent {
	public:
		/**
		 * Construct unnamed SimComponent.
		 *
		 * @param type The type of SimComponent (gct_ACTOR, gct_PHYSICS, etc)
		 */
		SimComponent(enum SimComponentType type, SimComposite* owner);

		/**
		 * Destructor.
		 */
		virtual ~SimComponent();


		inline Actor* owner() {
			return reinterpret_cast<Actor*>(owner_);
		}

		inline Actor* actor() {
			return reinterpret_cast<Actor*>(owner_);
		}
		inline const Actor* actor() const {
			return reinterpret_cast<Actor*>(owner_);
		}
		inline Area* toArea() {
			return reinterpret_cast<Area*>(owner_);
		}
		inline const Area* toArea() const {
			return reinterpret_cast<Area*>(owner_);
		}


		enum SimComponentType type() {
			return type_;
		}

		bool isActive();
		bool isDead();

	protected:
		friend class SimComposite;
		/** Called by SimComposite
		 */
		virtual void setActive(bool state) {}

		/** Called by SimComposite
		 */
		void setParent(SimComposite& p) {}


		/**
		 * The type of SimComponent.
		 */
		enum SimComponentType type_;
		SimComposite* owner_;
	};

}

#endif
