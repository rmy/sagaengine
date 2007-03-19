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


#ifndef CompositeFactory_hpp
#define CompositeFactory_hpp

#include "comp.hpp"
#include "util/type/util_type.hpp"
#include "ComponentFactory.hpp"

namespace se_core {

	class _SeCoreExport CompositeFactory {
	public:
		CompositeFactory(CompositeType subtype, String* name);
		virtual ~CompositeFactory();
		const char* name() const;
		int type() const { return type_; }

		virtual Composite* create() const;
		virtual void release(Composite* t) const;

		void addComponent(ComponentFactory* f);

		static void addGenericComponent(CompositeType type, const ComponentFactory* f) {
			gen().addComponent(type, f);
		}

		void setTag(int t) { tag_ = t; }
		int tag() { return tag_; }

	protected:
		String* name_;
		CompositeType type_;
		int tag_;
		
		void createComponents(Composite* owner) const;
		void createGenericComponents(Composite* owner) const;		

		static const int MAX_COMPONENTS = 32;
		int componentCount_;
		ComponentFactory* components_[ MAX_COMPONENTS ];

		class Generic {
		public:
			Generic() {
				for(int i = 0; i < MAX_COMPONENT_TYPES; ++i) {
					componentCount_[i] = 0;
				}
			}

			void addComponent(CompositeType type, const ComponentFactory* f) {
				components_[type][ componentCount_[type]++ ] = f;
			}

			void createComponents(CompositeType type, Composite* owner) const {
				for(int i = 0; i < componentCount_[type]; ++i) {
					components_[type][i]->create(owner);
				}
			}

			static const int MAX_COMPONENT_TYPES = st_SUBTYPE_COUNT;
			static const int MAX_COMPONENTS = 16;
			int componentCount_[MAX_COMPONENT_TYPES];
			const ComponentFactory* components_[MAX_COMPONENT_TYPES][MAX_COMPONENTS];
		};

		static Generic& gen();
	};

}

#endif
