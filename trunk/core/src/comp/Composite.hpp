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


#ifndef Composite_hpp
#define Composite_hpp

#include "comp.hpp"
#include "Object.hpp"
#include "RefPtr.hpp"
#include "comp/list/ComponentList.hpp"
#include "comp/list/CompositeList.hpp"


namespace se_core {
	class _SeCoreExport Composite {
	public:
		typedef Composite* id_type;

		Composite(const CompositeFactory* factory);
		Composite(const CompositeFactory* factory, const char* name);
		virtual ~Composite();

		RefPtr& ref() { return ptr_; }

		const char* name() const;
		int type() const;

		id_type id() const { return (id_type)this; }

		void init(bool doTraverseChildren = true);
		void cleanup(bool doTraverseChildren = true);

		Component* component(int type);
		const Component* component(int type) const;

		const CompositeFactory* factory() { return factory_; }
		void setFactory(const CompositeFactory* f) { factory_ = f; }

		/**
		 * Does the composite have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Get parent.
		 */
		Composite* parent() {
			return parent_;
		}

		/**
		 * Set the parent composite.
		 */
		void setParent(Composite& p);

		/**
		 * Set parent composite to none.
		 */
		void resetParent();

		/**
		 * List of child composites.
		 */
		const CompositeList& children() const { return children_; }


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
		void zoneChanged(int type, Composite* newZone, Composite* oldZone);

		/**
		 * Destroy components. Called by factory or destructor.
		 */
		void releaseComponents();

	private:
		friend class Component;
		// The Component adds and removes itself
		void addComponent(Component& c);
		void removeComponent(Component& c);


	protected:

		/**
		 * Add a Composite as a child.
		 * Called by the childs setParent.
		 */
		void addChild(Composite& node);

		/**
		 * Add a Composite as a child.
		 * Called by the childs setParent.
		 */
		void removeChild(Composite& node);

		const char* name_;
		const CompositeFactory* factory_;
		int tag_;
		bool isActive_;
		bool isDead_;

		ComponentList components_;

		Composite* parent_;
		CompositeList children_;
		enum { FAST_COMPONENT_COUNT = 16 };
		Component* fastComponents_[16];


		/**
		 * Reference counted pointer.
		 */
		RefPtr ptr_;

	};

}


#endif
