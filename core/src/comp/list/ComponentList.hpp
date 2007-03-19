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


#ifndef ComponentList_hpp
#define ComponentList_hpp

#include "../comp.hpp"
#include "../schema/CompSchema.hpp"

namespace se_core {

	class _SeCoreExport ComponentList {
		friend class ComponentListIterator;

	public:
		typedef CompSchema::VoidList::iterator_type iterator_type;
		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(const ComponentList& mgo);
			void init(const ComponentList& mgo);
			void init(short firstNode);

			inline bool hasNext() {
				return it_ != CompSchema::VoidList::end();
			}

			inline Component& next() {
				return *static_cast<Component*>(CompSchema::voidList.next(it_));
			}

		private:
			CompSchema::VoidList::iterator_type it_;
		};


		ComponentList();
		virtual ~ComponentList();
		virtual void add(Component& value);
		void add(ComponentList& msc);
		virtual void remove(Component& value);
		void initIterator(CompSchema::VoidList::iterator_type& iterator) const;
		inline CompSchema::VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(Component& value) const;
		bool contains(ComponentList& msc) const;
		bool sharesAny(ComponentList& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		CompSchema::VoidList::iterator_type firstNode_;
	};
}

#endif
