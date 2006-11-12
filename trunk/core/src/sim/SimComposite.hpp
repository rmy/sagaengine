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
#include "SimPtr.hpp"


namespace se_core {
	typedef HashTable<class SimComposite, 200> SCOHT;
	class _SeCoreExport Composites : public SCOHT  {
	};


	class _SeCoreExport SimComposite : public SimObject {
	protected:
		SimComposite(enum SimObjectType type, const char* name);
		SimPtr& ptr() { return ptr_; }

	public:
		SimComposite(const char* name);

		SimComponent* component(int type);
		void addComponent();


		/**
		 * Does the Pos have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Get parent.
		 */
		SimComposite* parent() {
			return parent_;
		}

		/**
		 *
		 */
		void setParent(SimComposite& p);
		void resetParent();


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

		/**
		 * Add a SimComposite as a child.
		 * Called by the childs setParent.
		 */
		void addChild(SimComposite& node);

		/**
		 * Add a SimComposite as a child.
		 * Called by the childs setParent.
		 */
		void removeChild(SimComposite& node);

		SimPtr ptr_;

		int tag_;
		friend class ThingManager;
		SimCompositeFactory* factory_;
		SimComposite* parent_;
		MultiSimObject children_;

		MultiSimComponent components_;

		bool isActive_;
		bool isDead_;

	};

}


#endif
