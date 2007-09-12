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


#ifndef SinglyLinkedList_hpp
#define SinglyLinkedList_hpp

#include "util/error/Log.hpp"


namespace se_core {

	/**
	 * Singly linked list manager.
	 * Stores multiple singly linked lists in a single fixed size array. The
	 * approach makes memory usage efficient for MultiSimObject
	 * and similar classes without introducing the overhead of allocation,
	 * resizing and deallocation of memory buffers on creation or destruction
	 * of the list or when deleting or adding members to the list.
	 */
	template <class ElementType, int maxElements, int xid> class SinglyLinkedList {
	public:
		typedef int iterator_type;


		/**
		 * Constructor.
		 * @param name The name of the singly linked list (for debug purposes)
		 */
		SinglyLinkedList(const char* name)
			: nodes_(new ElementType*[ MAX_ELEMENTS ])
			, nextNodes_(new iterator_type[ MAX_ELEMENTS ])
			, name_(name) {
			clear();
			DebugExec(count_ = 0);
		}


		/**
		 * Destructor.
		 */
		virtual ~SinglyLinkedList() {
			delete[] nodes_;
			delete[] nextNodes_;
		}


		/**
		 * Add member to a list.
		 * The member will be added to the beginning of the list
		 * for efficiency reasons.
		 * @param element the element to add
		 * @param firstNode a reference to a variable containint the
		 *    index of the first node in the list (or end() if it
		 *    is a new or empty list)
		 */
		void add(ElementType* element, iterator_type &firstNode) {
			// Check that there are free nodes left
			DebugExec(if(firstFreeNode_ == end())) LogDetail(name_ << ": " << MAX_ELEMENTS << " - " << count_);
			DebugExec(if(firstFreeNode_ == end())) LogDetail("Size: " << size(firstNode));

			AssertFatal(firstFreeNode_ < MAX_ELEMENTS && firstFreeNode_ >= 0, name_ << " Max size: " << MAX_ELEMENTS << " Chain size: " << size(firstNode));
			AssertFatal((element) != 0, name_);
			//DbgAssert(!isFree(firstNode));

			// Store pointer to the second node in free node chain
			iterator_type tmp = nextNodes_[ firstFreeNode_ ];

			// Make the first free node part of the node chain
			nextNodes_[ firstFreeNode_ ] = firstNode;
			firstNode = firstFreeNode_;

			// Remove used node from free node chain
			firstFreeNode_ = tmp;

			// Store actor as first node of the chain
			nodes_[ firstNode ] = element;
			DebugExec(++count_);

			DebugExec(if(nodes_[0] == 0) LogDetail(name_));
		}


		/**
		 * Remove a member from the the list.
		 * @TODO: This should be protected.
		 */
		ElementType* remove(iterator_type &iterator, const iterator_type &previousElement) {
			//Assert(nodes_[0] != 0);
			// Store the reference to the element after the iterator
			iterator_type tmp = nextNodes_[ iterator ];

			// Put the iterator node into the free node chain
			nextNodes_[ iterator ] = firstFreeNode_;
			firstFreeNode_ = iterator;

			// Make the iterator point to the next node in the chain
			if(previousElement != end()) {
				nextNodes_[ previousElement ] = tmp;
			}
			iterator = tmp;
			Assert(iterator >= -1);
			Assert(iterator < MAX_ELEMENTS);

			DebugExec(if(nodes_[0] == 0) LogDetail(name_));

			// Return deleted node
			return nodes_[ firstFreeNode_ ];
		}


		/**
		 * Remove and return topmost element in list.
		 */
		ElementType* pop(iterator_type &iterator) {
			// Store the reference to the element after the iterator
			iterator_type tmp = nextNodes_[ iterator ];
			AssertFatal(tmp < MAX_ELEMENTS && tmp >= -1, name_ << " Max size: " << MAX_ELEMENTS);

			// Put the iterator node into the free node chain
			nextNodes_[ iterator ] = firstFreeNode_;
			firstFreeNode_ = iterator;

			// Make the iterator point to the next node in the chain
			iterator = tmp;

			DebugExec(--count_);

			// Return deleted node
			return nodes_[ firstFreeNode_ ];
		}


		/**
		 * Remove an element from the list.
		 * Will only remove the element from the list once. If
		 * the member may exist in the list multiple times,
		 * the method must be called once for each instance
		 * to remove.
		 * @param element The elemnt to remove
		 * @param firstNode a reference to a variable containint the
		 *    index of the first node in the list (or end() if it
		 *    is a new or empty list)
		 */
		bool remove(ElementType* element, iterator_type &firstNode) {
			//DbgAssert(!isFree(firstNode));
			iterator_type iterator = firstNode;
			iterator_type prev = end();
			//Assert(nodes_[0] != 0 || firstNode == end());

			while(iterator != end()) {
				if(nodes_[ iterator ] == element) {
					if(iterator == firstNode) {
						remove(iterator, end());
						firstNode = iterator;
						return true;
					}
					else {
						// BUG: If two are removed in a row...
						remove(iterator, prev);
						return true;
					}
				}
				else {
					prev = iterator;
					iterator = nextNodes_[ iterator ];
				}
			}
			DebugExec(if(nodes_[0] == 0) LogDetail(name_));
			DebugExec(--count_);
			return false;
		}


		/**
		 * Remove the chain
		 * Remove all members from the singly linked list
		 * @param firstNode a reference to a variable containint the
		 *    index of the first node in the list (or end() if it
		 *    is a new or empty list)
		 */
		void removeChain(iterator_type &firstNode) {
			// Store beginning of chain
			//DbgAssert(!isFree(firstNode));
			if(firstNode == end())
				return;
			iterator_type tmp = firstNode;

			// Find last node
			while(nextNodes_[ firstNode ] != end()) {
				firstNode = nextNodes_[ firstNode ];
			}
			// Make last node point to free nodes
			nextNodes_[ firstNode ] = firstFreeNode_;

			// Make list of free nodes point to first node
			firstFreeNode_ = tmp;

			// Reset firstNode of cleared chain
			firstNode = end();
		}


		/**
		 * Check if an element exists in a singly linked list
		 * @param element element to look for
		 * @param firstNode a reference to a variable containing the
		 *    index of the first node in the list (or end() if it
		 *    is a new or empty list)
		 */
		bool hasElement(ElementType* element, iterator_type& firstNode) {
			//DbgAssert(!isFree(firstNode));
			iterator_type iterator = firstNode;
			while(iterator != end()) {
				if(nodes_[ iterator ] == element)
					return true;

				// Make iterator point to the next node in the chain
				iterator = nextNodes_[ iterator ];
			}
			return false;
		}


		/**
		 * Iterate to the next member of a singly linked list
		 * The iterator is a iterator_type value that should
		 * initially have the index of the first node in the
		 * list as its value. Each time this method is called,
		 * the iterator is changed to point to the next member.
		 * @param iterator a reference to a variable pointing
		 *       to the current member in the list. After the call
		 *       it will contain the index of the next member,
		 *       or end() if no more members exist.
		 */
		ElementType* next(iterator_type& iterator) {
			//DbgAssert(!isFree(iterator));
			//DbgAssert(iterator != end());
			//DbgAssert(iterator >= 0);
			DbgAssert(iterator >= 0 && iterator < MAX_ELEMENTS);

			// Store actor
			ElementType* tmp = nodes_[ iterator ];

			// Make iterator point to the next node in the chain
			iterator = nextNodes_[ iterator ];

			// Return the element the iterator pointed to previously
			return tmp;
		}


		/**
		 * Returns the value that terminates the loop.
		 */
		static const iterator_type end() {
			return -1;
		}


		/**
		 * The number of members in a list.
		 * @param firstNode a reference to a variable containing the
		 *    index of the first node in the list (or end() if it
		 *    is a new or empty list)
		 * @return the number of members
		 */
		int size(const iterator_type& firstNode) {
			//DbgAssert(!isFree(firstNode));
			int s = 0;
			iterator_type iterator = firstNode;
			while(iterator != end()) {
				++s;
				iterator = nextNodes_[ iterator ];
			}
			return s;
		}


		int capacity() {
			return size(firstFreeNode_);
		}

		bool isFree(const iterator_type &it) {
			iterator_type iterator = firstFreeNode_;
			while(iterator != end()) {
				if(it == iterator) return true;
				iterator = nextNodes_[ iterator ];
			}
			return false;
		}



		/**
		 * Remove all singly linked lists.
		 * Removes all singly linked lists contained in the
		 * structure.
		 */
		void clear() {
			firstFreeNode_ = 0;
			for(iterator_type i = 0; i < MAX_ELEMENTS; ++i) {
				nextNodes_[ i ] = i + 1;
			}
			nextNodes_[ MAX_ELEMENTS - 1 ] = end();
			//LogDetail("Free node list size (" << name << "): " << size(firstFreeNode_));
		}


		/**
		 * End of list marker.
		 * The value that when used as the firstNode signifies
		 * that the list is empty, or if in an iterator,
		 * that the iterator has reached the end of the list.
		 */
		//static const iterator_type end() = -1;
		// Must be primitive type
		//static const int end() = -1;


	private:
		static const iterator_type MAX_ELEMENTS = maxElements;
		/**
		 * First free member.
		 * Free members are contained in its own singly linked list.
		 * This is the index of the first member of that list.
		 */
		iterator_type firstFreeNode_;

		/**
		 * The list of elements.
		 */
		ElementType** nodes_;

		/**
		 * The next node for any element in nodes_ with
		 * the same array index. A value of end() means
		 * that the node is the last in the list.
		 */
		iterator_type* nextNodes_;

	protected:
		/**
		 * The name of this container instance (included to ease debugging).
		 */
		const char* name_;
		DebugExec(int count_);
	};

}

#endif
