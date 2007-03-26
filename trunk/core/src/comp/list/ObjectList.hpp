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


#ifndef ObjectList_hpp
#define ObjectList_hpp

#include "comp/comp.hpp"
#include "comp/schema/CompSchema.hpp"

namespace se_core {

	class _SeCoreExport ObjectList {
		friend class ObjectListIterator;

	public:
		typedef CompSchema::VoidList::iterator_type iterator_type;
		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(const ObjectList& mgo);
			void init(const ObjectList& mgo);
			void init(short firstNode);

			inline bool hasNext() {
				return it_ != CompSchema::VoidList::end();
			}

			inline const Object& next() {
				return *static_cast<Object*>(CompSchema::voidList.next(it_));
			}

		private:
			CompSchema::VoidList::iterator_type it_;
		};


		ObjectList();
		virtual ~ObjectList();
		virtual void add(const Object& value);
		void add(ObjectList& msc);
		virtual void remove(const Object& value);
		void initIterator(CompSchema::VoidList::iterator_type& iterator) const;
		inline CompSchema::VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(const Object& value) const;
		bool contains(ObjectList& msc) const;
		bool sharesAny(ObjectList& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		CompSchema::VoidList::iterator_type firstNode_;
	};
}

#endif
