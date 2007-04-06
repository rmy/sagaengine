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


#ifndef o3d_thing_ThingMOList_hpp
#define o3d_thing_ThingMOList_hpp

#include "o3d_thing.hpp"
#include "comp/comp.hpp"
#include "comp/schema/CompSchema.hpp"

namespace se_ogre {

	class _SeCoreExport ThingMOList {
		friend class ThingMOListIterator;

	public:
		typedef se_core::CompSchema::VoidList::iterator_type iterator_type;
		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(const ThingMOList& mgo);
			void init(const ThingMOList& mgo);
			void init(short firstNode);

			inline bool hasNext() {
				return it_ != se_core::CompSchema::VoidList::end();
			}

			inline ThingMO& next() {
				return *static_cast<ThingMO*>(se_core::CompSchema::voidList.next(it_));
			}

		private:
			se_core::CompSchema::VoidList::iterator_type it_;
		};


		ThingMOList();
		virtual ~ThingMOList();
		void add(ThingMO& value);
		void add(ThingMOList& msc);
		void remove(ThingMO& value);
		ThingMO& pop();
		void initIterator(se_core::CompSchema::VoidList::iterator_type& iterator) const;
		inline se_core::CompSchema::VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(const ThingMO& value) const;
		bool contains(ThingMOList& msc) const;
		bool sharesAny(ThingMOList& msc) const;
		bool isEmpty() const;
		void clear();
		int size() const;

	protected:
		se_core::CompSchema::VoidList::iterator_type firstNode_;
	};
}

#endif
