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


#ifndef CompositeList_hpp
#define CompositeList_hpp

#include "../comp.hpp"
#include "../schema/CompSchema.hpp"

namespace se_core {

	class _SeCoreExport CompositeList {
	public:
		typedef CompSchema::VoidList::iterator_type iterator_type;
		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(const CompositeList& mgo);
			void init(const CompositeList& mgo);
			void init(short firstNode);

			inline bool hasNext() {
				return it_ != CompSchema::VoidList::end();
			}

			inline Composite& next() {
				return *static_cast<Composite*>(CompSchema::voidList.next(it_));
			}

		private:
			CompSchema::VoidList::iterator_type it_;
		};


		class _SeCoreExport TreeIterator {
		public:
			TreeIterator();
			TreeIterator(const CompositeList& mgo);
			void init(const CompositeList& mgo);

			bool hasNext();
			Composite& next();

		private:
			static const int MAX_STACK_DEPTH = 10;
			CompositeList::Iterator itStack_[ MAX_STACK_DEPTH ];
			short stackPointer_;
		};

		CompositeList();
		virtual ~CompositeList();
		virtual void add(Composite& value);
		void add(CompositeList& msc);
		virtual void remove(Composite& value);
		void initIterator(CompSchema::VoidList::iterator_type& iterator) const;
		inline CompSchema::VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(Composite& value) const;
		bool contains(CompositeList& msc) const;
		bool sharesAny(CompositeList& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		CompSchema::VoidList::iterator_type firstNode_;
	};
}

#endif
