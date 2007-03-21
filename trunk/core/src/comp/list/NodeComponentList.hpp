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


#ifndef NodeComponentList_hpp
#define NodeComponentList_hpp

#include "comp/comp.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/node/comp_node.hpp"

namespace se_core {

	class _SeCoreExport NodeComponentList {
		friend class NodeComponentListIterator;

	public:
		typedef CompSchema::VoidList::iterator_type iterator_type;

		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(NodeComponentList& mgo);
			void init(NodeComponentList& mgo);
			void init(short firstNode);

			inline bool hasNext() {
				return it_ != CompSchema::VoidList::end();
			}

			inline NodeComponent& next() {
				return *static_cast<NodeComponent*>(CompSchema::voidList.next(it_));
			}

		private:
			CompSchema::VoidList::iterator_type it_;
		};


		class _SeCoreExport TreeIterator {
		public:
			TreeIterator();
			TreeIterator(NodeComponentList& mgo);
			void init(NodeComponentList& mgo);

			bool hasNext();
			NodeComponent& next();

		private:
			static const int MAX_STACK_DEPTH = 10;
			NodeComponentList::Iterator itStack_[ MAX_STACK_DEPTH ];
			short stackPointer_;
		};


		NodeComponentList();
		virtual ~NodeComponentList();
		virtual void add(NodeComponent& value);
		void add(NodeComponentList& msc);
		virtual void remove(NodeComponent& value);
		void initIterator(CompSchema::VoidList::iterator_type& iterator) const;
		inline CompSchema::VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(NodeComponent& value) const;
		bool contains(NodeComponentList& msc) const;
		bool sharesAny(NodeComponentList& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		CompSchema::VoidList::iterator_type firstNode_;
	};
}

#endif
