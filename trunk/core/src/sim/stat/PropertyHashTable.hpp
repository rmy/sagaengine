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


#ifndef sim_stat_PropertyHashTable_hpp
#define sim_stat_PropertyHashTable_hpp

#include "util/template/SinglyLinkedList.hpp"
#include "comp/schema/CompSchema.hpp"
#include "util/error/Log.hpp"
#include "Property.hpp"

namespace se_core {

	class PropertyHashTable {
	public:
		typedef CompSchema::VoidList::iterator_type iterator_type;
	protected:
		enum {
			DEFAULT_HASH_TABLE_SIZE = 16
		};

		iterator_type* hashTable_;
		int tableSize_;
		int count_;


	public:
		PropertyHashTable()
				: count_(0) {
			initialize();
		}

		PropertyHashTable(int size)
				: count_(0) {
			initialize(size);
		}

		virtual ~PropertyHashTable() {
			if(hashTable_) {
				// For all hash lists
				for(int i = 0; i < tableSize(); ++i) {
					// delete HashNode's
					iterator_type it;
					it = hashTable_[i];
					while(it != CompSchema::VoidList::end()) {
						Property* p = static_cast<Property*>(CompSchema::voidList.next(it));
						delete p;
					}
					it = hashTable_[i];
					// remove link chain from nodeList
					CompSchema::voidList.removeChain(it);
				}
				delete [] hashTable_;
			}
		}

		inline int tableIndex(int key) const {
			return key & (tableSize_ - 1);
		}

		void initialize(int size = DEFAULT_HASH_TABLE_SIZE) {
			hashTable_ = new iterator_type[size];
			tableSize_ = size;
			for(int i = 0; i < tableSize_; ++i) {
				hashTable_[i] = CompSchema::VoidList::end();
			}
		}


		// insert key into hash table.
		// returns pointer to old data with the key, if any, or
		// NULL if the key wasn't in the table previously.
		Property* add(Property& newNode) {
			if(!hashTable_)
				return 0;

			int key = newNode.key();
			unsigned int index = tableIndex(key);
			Property* old = lookup(key);
			if(old) {
				remove(old->key());
			}

			CompSchema::voidList.add(&newNode, hashTable_[ index ]);
			++count_;
			return old;
		}


		Property* lookup(int key) const {
			if(!hashTable_)
				return 0;

			Property* node = lookupNode(key);
			return node;
		}


		// returns the list that contains this hash key...
		// (for instance, if you have multiple matching keys)
		iterator_type lookupList(int key) {
			if(!hashTable_)
				return -1;
			unsigned int index = tableIndex(key);
			return hashTable_[index];
		}


		Property* remove(short key) {
			if(!hashTable_)
				return 0;

			unsigned int index = tableIndex(key);

			iterator_type walker = hashTable_[index];
			while( walker != CompSchema::VoidList::end() ) {
				Property* v = static_cast<Property*>(CompSchema::voidList.next(walker));
				if(v->key() == key) {
					CompSchema::voidList.remove(v, hashTable_[ index ]);
					--count_;
					return v;
				}
			}

			return 0;
		}


		iterator_type hashTable(int index) {
			return hashTable_[index];
		}



		int tableSize() {
			return tableSize_;
		}


		int count() {
			return count_;
		}


	protected:
		Property* lookupNode(short key) const {
			unsigned int index = tableIndex(key);
			Property* ret = 0;

			iterator_type walker;
			walker = hashTable_[index];

			while(walker != CompSchema::VoidList::end()) {
				Property* v = static_cast<Property*>(CompSchema::voidList.next(walker));
				if(v->key() == key) {
					ret = v;
					break;
				}
			}

			return ret;
		}


	private:
		// declared to prevent unintentional use...
		// (Don't forget to move to public access if you declare them!)
		PropertyHashTable& Copy(const PropertyHashTable& source_object) { return *this; }
		PropertyHashTable& operator= (const PropertyHashTable& source_object) { return *this; }
	};
}

#endif
