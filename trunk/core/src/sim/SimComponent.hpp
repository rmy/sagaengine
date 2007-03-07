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
		SimComponent(enum SimComponentType type, SimComposite* owner, const SimComponentFactory* factory = 0);

		virtual const char* name() { return "Unkown"; }

		/**
		 * Destructor.
		 */
		virtual ~SimComponent();


		inline SimComposite* owner() {
			return owner_;
		}

		inline const SimComposite* owner() const {
			return owner_;
		}

		inline Actor* toActor() {
			return reinterpret_cast<Actor*>(owner_);
		}
		inline const Actor* toActor() const {
			return reinterpret_cast<Actor*>(owner_);
		}
		inline Area* toArea() {
			return reinterpret_cast<Area*>(owner_);
		}
		inline const Area* toArea() const {
			return reinterpret_cast<Area*>(owner_);
		}


		enum SimComponentType type() const {
			return type_;
		}


		const SimComponentFactory* factory() {
			return factory_;
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
		virtual void parentChanged(SimComposite* newParent, SimComposite* oldParent) {}

		/** Called by SimComposite
		 */
		virtual void areaChanged(SimComposite* newArea, SimComposite* oldArea);

		/** Called by SimComposite
		 */
		virtual void init();
		/** Called by SimComposite
		 */
		virtual void cleanup();

		/**
		 * The type of SimComponent.
		 */
		enum SimComponentType type_;
		SimComposite* owner_;

		friend class SimCompositeFactory_;
		const SimComponentFactory* factory_;


	protected:
		/**
		 * Convenience pointer template with built in component conversion.
		 *
		 * Include the following in a the public part of the class declaration
		 * of XxxComponent to get a convenient Ptr method that automatically
		 * converts between owner compoisite and sibling components and XxxComponent.
		 * typedef se_core::SimComponent::Ptr<XxxComponent, se_core::sct_XXX> Ptr;
		 */

		template <class T, int type> class Ptr {
		public:
			Ptr(T* c) {
				component_ = c;
			}
			Ptr(T& c) {
				component_ = &c;
			}
			Ptr(SimComponent* c) {
				if(c)
					component_ = static_cast<T*>(c->owner()->component(type));
				else
					component_ = 0;
			}
			Ptr(SimComponent& c) {
				component_ = static_cast<T*>(c.owner()->component(type));
			}
			const Ptr(const SimComponent& c) {
				component_ = (T*)(c.owner()->component(type));
			}
			Ptr(SimComposite* c) {
				if(c)
					component_ = static_cast<T*>(c->component(type));
				else
					component_ = 0;
			}
			Ptr(SimComposite& c) {
				component_ = static_cast<T*>(c.component(type));
			}

			const Ptr(const SimComposite& c) {
				component_ = (T*)(c.component(type));
			}

			inline T* operator->() {
				Assert(component_);
				return component_;
			}

			inline T& operator*() {
				Assert(component_);
				return *component_;
			}

			inline const T* operator->() const {
				Assert(component_);
				return component_;
			}

			inline const T& operator*() const {
				Assert(component_);
				return *component_;
			}

			Ptr& operator=(T& c) {
				component_ = &c;
				return *this;
			}

			Ptr& operator=(T* c) {
				component_ = c;
				return *this;
			}

			Ptr& operator=(SimComponent* c) {
				if(c)
					component_ = static_cast<T*>(c->owner()->component(type));
				else
					component_ = 0;
				return *this;
			}

			Ptr& operator=(SimComponent& c) {
				component_ = static_cast<T*>(c.owner()->component(type));
				return *this;
			}

			Ptr& operator=(SimComposite* c) {
				if(c)
					component_ = static_cast<T*>(c->component(type));
				else
					component_ = 0;
				return *this;
			}

			Ptr& operator=(SimComposite& c) {
				component_ = static_cast<T*>(c.component(type));
				return *this;
			}

			operator T*() {
				return component_;
			}

			operator const T*() const {
				return component_;
			}

			bool isNull() {
				return component_ == 0;
			}

			bool operator==(const T* c) const {
				return component_ == c;
			}

		private:
			T* component_;
		};
	};


}

#endif
