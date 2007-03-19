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
#include "SimPtr.hpp"
#include "stat/MultiSimComponent.hpp"
#include "stat/MultiSimComposite.hpp"


namespace se_core {
	class _SeCoreExport SimComposite : public SimObject {
	protected:
		SimComposite(enum SimObjectType type, const char* name);

	public:
		SimPtr& ptr() { return ptr_; }
		SimComposite(const char* name);
		virtual ~SimComposite();

		void init(bool doTraverseChildren = true);
		void cleanup(bool doTraverseChildren = true);

		SimComponent* component(int type);
		const SimComponent* component(int type) const;

		const SimCompositeFactory* factory() { return factory_; }
		void setFactory(const SimCompositeFactory* f) { factory_ = f; }

		/**
		 * Does the composite have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Get parent.
		 */
		SimComposite* parent() {
			return parent_;
		}

		/**
		 * Set the parent composite.
		 */
		void setParent(SimComposite& p);

		/**
		 * Set parent composite to none.
		 */
		void resetParent();

		/**
		 * List of child composites.
		 */
		const MultiSimComposite& children() const { return children_; }


		/**
		 * Set composite state to active.
		 * Typically used for paging in active game zones.
		 */
		void setActive(bool state, bool doTraverseChildren = true);


		/**
		 * Is the composite active.
		 */
		bool isActive() { return isActive_; }

		void setTag(int t) { tag_ = t; }
		int tag() const { return tag_; }

		/** Schedule the Composite for destruction.
		 *
		 * The destruction will be delayed one or two AI steps before it
		 * is actually destroyed, to make sure that any reference to
		 * and Action upon the Thing in the world has a chance to be resolved
		 * first.
		 */
		virtual void scheduleForDestruction();

		/** Returns true if Thing is scheduled for destruction.
		 *
		 * @see scheduleForDestruction
		 */
		bool isDead() { return isDead_; }

		/**
		 * Called during flip is area is changed from pos to nextPos
		 */
		void areaChanged(SimComposite* newArea, SimComposite* oldArea);

		/**
		 * Destroy components. Called by factory or destructor.
		 */
		void releaseComponents();

	private:
		friend class SimComponent;
		// The SimComponent adds and removes itself
		void addComponent(SimComponent& c);
		void removeComponent(SimComponent& c);


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

		/**
		 * Reference counted pointer.
		 */
		SimPtr ptr_;

		int tag_;
		friend class SpawnManager;
		const SimCompositeFactory* factory_;
		SimComposite* parent_;
		MultiSimComposite children_;
		MultiSimComponent components_;
		enum { FAST_COMPONENT_COUNT = 16 };
		SimComponent* fastComponents_[16];

		bool isActive_;
		bool isDead_;
	};

}


#endif
