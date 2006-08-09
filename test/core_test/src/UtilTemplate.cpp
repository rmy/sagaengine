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


#include "UtilTemplate.hpp"
#include "util/error/Log.hpp"
#include "util/template/all.hpp"
#include "util/template/SinglyLinkedList.hpp"
#include "util/template/QuadTreeNode.hpp"

using namespace se_core;

namespace test {
	struct Element {
		int key;
		int value;
	};


	UtilTemplate
	::UtilTemplate() {
	}


	UtilTemplate
	::~UtilTemplate() {
	}


	void UtilTemplate
	::run() {
		util_template_QuadTreeNode();
		util_template_HashTable();
	}


	void UtilTemplate
	::util_template_HashTable() {
		/*
		WasHere();
		HashTable<struct Element, 512> elementHash;
		Element elements[256];
		Element* e = 0;

		for(int i = 0; i < 256; ++i) {
			elements[i].key = i;
			elements[i].value = i * i;
			e = elementHash.add(i, &elements[i]);
			Assert(e == &elements[i]);
		}

		for(int i = 0; i < 256; ++i) {
			e = elementHash.lookup(i);
			Assert(e == &elements[i]);
		}

		e = elementHash.lookup(256);
		Assert(e == 0);

		e = elementHash.remove(50);
		Assert(e == &elements[50]);

		e = elementHash.lookup(50);
		Assert(e == 0);

		LogMsg("UNIT TEST PASSED: util_template_HashTable");
		*/
	}



	class ElementQuadTreeNode : public QuadTreeNode<ElementQuadTreeNode> {
	public:
		struct ElementList : public SinglyLinkedList<Element, 256, 253> {
			ElementList(const char* name)
				: SinglyLinkedList<Element, 256, 253>(name) {
			}
		};
		typedef ElementList::iterator_type iterator_type;

		ElementQuadTreeNode() : firstElement_(ElementList::NULL_NODE) {
		}


		Element& add(Element& element) {
			elementList().add(element, firstElement_);
			return element;
		}


		Element& remove(Element& element) {
			elementList().remove(element, firstElement_);
			return element;
		}


		static ElementList& elementList() {
			static ElementList singleton(__FILE__);
			return singleton;
		}

	private:
		iterator_type firstElement_;
	};


	void UtilTemplate
	::util_template_QuadTreeNode() {
		ElementQuadTreeNode rootNode;
		rootNode.setDepth(8);
		rootNode.setBounds(0, 0, 256, 256);

		LogMsg("UNIT TEST PASSED: util_template_QuadTreeNode");
	}

}
