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


#ifndef Component_hpp
#define Component_hpp

#include "comp.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"

namespace se_core {
	/**
	 * Base class for functionality composites.
	 */
	class _SeCoreExport Component {
	public:
		/**
		 * Construct unnamed Component.
		 *
		 * @param type The type of Component.
		 */
		Component(int type, Composite* owner, const ComponentFactory* factory = 0);

		virtual const char* name() { return "Unkown"; }

		/**
		 * Destructor.
		 */
		virtual ~Component();


		inline Composite* owner() {
			return owner_;
		}


		inline const Composite* owner() const {
			return owner_;
		}


		int type() const {
			return type_;
		}


		const ComponentFactory* factory() {
			return factory_;
		}

		bool isActive();
		bool isDead();

	protected:
		friend class Composite;

		/** Called by Composite
		 */
		virtual void setActive(bool state) {}

		/** Called by Composite
		 */
		virtual void setDead() {}

		/** Called by Composite
		 */
		virtual void parentChanged(Composite* newParent, Composite* oldParent) {}

		/** Called by Composite
		 */
		virtual void areaChanged(Composite* newArea, Composite* oldArea);

		/** Called by Composite
		 */
		virtual void init();
		/** Called by Composite
		 */
		virtual void cleanup();

		/**
		 * The type of Component.
		 */
		int type_;
		Composite* owner_;

		const ComponentFactory* factory_;

	protected:
		/**
		 * Convenience pointer template with built in component conversion.
		 *
		 * Include the following in a the public part of the class declaration
		 * of XxxComponent to get a convenient Ptr method that automatically
		 * converts between owner compoisite and sibling components and XxxComponent.
		 * typedef se_core::Component::Ptr<XxxComponent, se_core::sct_XXX> Ptr;
		 */

		template <class T, int type> class Ptr {
		public:
			Ptr(T* c) {
				component_ = c;
			}
			Ptr(T& c) {
				component_ = &c;
			}
			Ptr(Component* c) {
				if(c)
					component_ = static_cast<T*>(c->owner()->component(type));
				else
					component_ = 0;
			}
			Ptr(Component& c) {
				component_ = static_cast<T*>(c.owner()->component(type));
			}
			const Ptr(const Component& c) {
				component_ = (T*)(c.owner()->component(type));
			}
			Ptr(Composite* c) {
				if(c)
					component_ = static_cast<T*>(c->component(type));
				else
					component_ = 0;
			}
			Ptr(Composite& c) {
				component_ = static_cast<T*>(c.component(type));
			}

			const Ptr(const Composite& c) {
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

			Ptr& operator=(Component* c) {
				if(c)
					component_ = static_cast<T*>(c->owner()->component(type));
				else
					component_ = 0;
				return *this;
			}

			Ptr& operator=(Component& c) {
				component_ = static_cast<T*>(c.owner()->component(type));
				return *this;
			}

			Ptr& operator=(Composite* c) {
				if(c)
					component_ = static_cast<T*>(c->component(type));
				else
					component_ = 0;
				return *this;
			}

			Ptr& operator=(Composite& c) {
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
