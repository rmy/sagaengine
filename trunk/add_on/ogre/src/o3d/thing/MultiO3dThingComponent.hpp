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


#ifndef MultiO3dThingComponent_hpp
#define MultiO3dThingComponent_hpp

#include <se_core.hpp>
#include "O3dThingComponent.hpp"
#include "comp/schema/CompSchema.hpp"

namespace se_ogre {

	class _SeCoreExport MultiO3dThingComponent {
		friend class MultiO3dThingComponentIterator;

	public:
		typedef se_core::CompSchema::VoidList::iterator_type iterator_type;
		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(MultiO3dThingComponent& mgo);
			void init(MultiO3dThingComponent& mgo);
			void init(short firstMode);

			inline bool hasNext() {
				return it_ != se_core::CompSchema::VoidList::end();
			}

			inline O3dThingComponent& next() {
				return *static_cast<O3dThingComponent*>(se_core::CompSchema::voidList.next(it_));
			}

		private:
			se_core::CompSchema::VoidList::iterator_type it_;
		};


		MultiO3dThingComponent();
		virtual ~MultiO3dThingComponent();
		virtual void add(O3dThingComponent& value);
		void add(MultiO3dThingComponent& msc);
		virtual void remove(O3dThingComponent& value);
		void initIterator(se_core::CompSchema::VoidList::iterator_type& iterator) const;
		inline se_core::CompSchema::VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(O3dThingComponent& value) const;
		bool contains(MultiO3dThingComponent& msc) const;
		bool sharesAny(MultiO3dThingComponent& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		se_core::CompSchema::VoidList::iterator_type firstNode_;
	};
}

#endif
