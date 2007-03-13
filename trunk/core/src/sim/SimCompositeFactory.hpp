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


#ifndef SimCompositeFactory_hpp
#define SimCompositeFactory_hpp

#include "sim.hpp"
#include "util/type/util_type.hpp"
#include "./config/sim_config.hpp"
#include "./script/sim_script.hpp"
#include "./stat/sim_stat.hpp"
#include "SimComponentFactory.hpp"

namespace se_core {

	class _SeCoreExport SimCompositeFactory {
	public:
		enum SubType { st_AREA, st_THING, st_SPECIAL, st_SUBTYPE_COUNT };

		SimCompositeFactory(short type, SubType subtype, String* name);
		virtual ~SimCompositeFactory();
		const char* name() const;
		virtual SimComposite* create() const;
		virtual void release(SimComposite* t) const;

		void addComponent(SimComponentFactory* f);

		static void addGenericComponent(SubType type, const SimComponentFactory* f) {
			gen().addComponent(type, f);
		}

		void setTag(int t) { tag_ = t; }
		int tag() { return tag_; }

	protected:
		String* name_;
		short type_;
		SubType subtype_;
		int tag_;
		
		void createComponents(SimComposite* owner) const;
		void createGenericComponents(SimComposite* owner) const;		

		static const int MAX_COMPONENTS = 32;
		int componentCount_;
		SimComponentFactory* components_[ MAX_COMPONENTS ];

		class Generic {
		public:
			Generic() {
				for(int i = 0; i < MAX_COMPONENT_TYPES; ++i) {
					componentCount_[i] = 0;
				}
			}

			void addComponent(SubType subtype, const SimComponentFactory* f) {
				components_[subtype][ componentCount_[subtype]++ ] = f;
			}

			void createComponents(SubType subtype, SimComposite* owner) const {
				for(int i = 0; i < componentCount_[subtype]; ++i) {
					components_[subtype][i]->create(owner);
				}
			}

			static const int MAX_COMPONENT_TYPES = st_SUBTYPE_COUNT;
			static const int MAX_COMPONENTS = 16;
			int componentCount_[MAX_COMPONENT_TYPES];
			const SimComponentFactory* components_[MAX_COMPONENT_TYPES][MAX_COMPONENTS];
		};

		static Generic& gen();

	};

}

#endif
