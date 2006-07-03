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


#ifndef base_template_HashTable_hpp
#define base_template_HashTable_hpp

#include "SinglyLinkedList.hpp"
#include "util/error/Log.hpp"

namespace se_core {

	template <class T, int poolSize> class HashTable {
	protected:
		enum {
			DEFAULT_HASH_TABLE_SIZE = 16
		};

	public:
		class HashTableNode {
		public:
			short key_;
			T* data_;

			HashTableNode(short key, T* new_data)
				: key_(key), data_(new_data) {
			}
		};

		struct NodeList
			: public SinglyLinkedList<HashTableNode, poolSize, 97> {
			NodeList(const char* name) : SinglyLinkedList<HashTableNode, poolSize, 97>(name) {
			}
		};
		typedef int iterator_type;



		HashTable()
			: count_(0) {
			initialize();
		}

		HashTable(int size)
			: count_(0) {
			initialize(size);
		}

		HashTable(const HashTable& source) {
			copy(source);
		}


		virtual ~HashTable() {
			if(hashTable_) {
				// For all hash lists
				for(int i = 0; i < tableSize(); ++i) {
					// delete HashNode's
					iterator_type it;
					it = hashTable_[i];
					while(it != NodeList::end()) {
						delete nodeList().next(it);
					}
					it = hashTable_[i];
					// remove link chain from nodeList
					nodeList().removeChain(it);
				}
				delete [] hashTable_;
			}
		}


		void initialize(int size = DEFAULT_HASH_TABLE_SIZE) {
			hashTable_ = new iterator_type[size];
			tableSize_ = size;
			for(int i = 0; i < tableSize_; ++i) {
				hashTable_[i] = NodeList::end();
			}
		}


		unsigned int hash(short key) {
			return key;
		}


		// insert key into hash table.
		// returns pointer to old data with the key, if any, or
		// NULL if the key wasn't in the table previously.
		T* add(short key, T* data) {
			if(!hashTable_)
				return 0;

			unsigned int index = hash(key) % tableSize();
			HashTableNode* newNode = new HashTableNode(key, data);
			nodeList().add(*newNode, hashTable_[ index ]);
			++count_;
			return newNode->data_;
		}


		T* lookup(short key) {
			if(!hashTable_)
				return 0;

			HashTableNode* newNode = lookupNode(key);

			if(newNode)
				return newNode->data_;

			return 0;
		}


		// returns the list that contains this hash key...
		// (for instance, if you have multiple matching keys)
		//HashTable::NodeList::iterator_type
		short lookupList(short key) {
			if(!hashTable_)
				return 0;
			unsigned int index = hash(key) % tableSize();
			return hashTable_[index];
		}


		T* remove(short key) {
			if(!hashTable_)
				return 0;

			unsigned int index = hash(key) % tableSize_;

			iterator_type walker = hashTable_[index];
			while( walker != NodeList::end() ) {
				HashTableNode* v = nodeList().next(walker);
				if(v->key_ == key) {
					T* return_value = v->data_;
					nodeList().remove(*v, hashTable_[ index ]);
					delete v;
					--count_;
					return return_value;
				}
			}

			return 0;
		}


		iterator_type hashTable() {
			return hashTable_;
		}

		NodeList& nodeList() {
			static NodeList singleton(__FILE__);
			return singleton;
		}

		int tableSize() {
			return tableSize_;
		}


		int count() {
			return count_;
		}


	protected:
		HashTableNode* lookupNode(short key) {
			unsigned int index = hash(key) % tableSize();
			HashTableNode* ret = 0;

			iterator_type walker;
			walker = hashTable_[index];

			while(walker != NodeList::end()) {
				HashTableNode* v = nodeList().next(walker);
				if(v->key_ == key) {
					ret = v;
					break;
				}
			}

			return ret;
		}

		iterator_type* hashTable_;
		int tableSize_;
		int	count_;

	private:
		// declared to prevent unintentional use...
		// (Don't forget to move to public access if you declare them!)
		HashTable& Copy(const HashTable& source_object);
		HashTable& operator= (const HashTable& source_object);

	};
}

#endif
