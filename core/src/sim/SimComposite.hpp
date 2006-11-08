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

#include "SimObject.hpp"
#include "sim.hpp"
#include "util/template/HashTable.hpp"
#include "stat/MultiSimComponent.hpp"
#include "stat/MultiSimObject.hpp"


namespace se_core {
	typedef HashTable<class SimComposite, 200> SCOHT;
	class _SeCoreExport Composites : public SCOHT  {
	};


	class _SeCoreExport SimComposite : public SimObject {
	protected:
		SimComposite(enum SimObjectType type, const char* name);

	public:
		SimComposite(const char* name);
		SimComponent* component(int type);
		void addComponent();

		bool isActive() { return isActive_; }

		/** Returns true if Thing is scheduled for destruction.
		 *
		 * @see scheduleForDestruction
		 */
		bool isDead() { return isDead_; }
		void setActive(bool state);

		void setTag(int t) { tag_ = t; }
		int tag() { return tag_; }

	private:
		friend class SimComponent;
		// The SimComponent adds and removes itself
		void removeComponent(SimComponent& c);
		void addComponent(SimComponent& c);

	protected:
		int tag_;
		friend class ThingManager;
		SimCompositeFactory* factory_;
		SimComposite* parent_;
		MultiSimObject childComposites_;

		MultiSimComponent components_;

		bool isActive_;
		bool isDead_;
	};

}


#endif
