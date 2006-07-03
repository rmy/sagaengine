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


#include "BaseUnitTest.hpp"
#include "util/error/Log.hpp"
#include "util/math/FixedPoint.hpp"
#include "util/template/all.hpp"
#include "util/template/SinglyLinkedList.hpp"
#include "util/template/QuadTreeNode.hpp"


namespace se_core {
	struct Element {
		int key;
		int value;
	};


	BaseUnitTest
	::BaseUnitTest() {
		DebugExec(go());
	}


	BaseUnitTest
	::~BaseUnitTest() {
	}


	void BaseUnitTest
	::go() {
		base_math_FixedPoint();
		base_template_HashTable();
	}


	void BaseUnitTest
	::base_math_FixedPoint() {

		FixedPoint v1, v2, v3, res1, res2;

		v1 = 0.0f;
		Assert(!v1); // Is zero
		v1 = 2.0f;
		Assert(v1); // Not zero
		v1 = 2.0f;
		v2 = 2.0f;
		v3 = 4.0f;

		Assert(FixedPoint::fromFloat(FixedPoint::toFloat(256)) == 256);

		res1 = v1 * v2;
		res2 = v1 + v2;
		Assert(v1 == v2);
		Assert(res1 == res2);
		Assert(res1 == v3);

		res1 /= v2;
		Assert(res1 == v1);
		res1 = (v1 * v2) / v1;
		Assert(res1 == v2);

		res1 = 0.0f;
		res2 = 0.0f;

		res1 = v1 + v2 + v3;
		res2 += v1;
		res2 += v2;
		res2 += v3;
		Assert(res1 == res2);

		res1 = v1 + v2 + v3;
		res1 -= v2;
		res1 -= v3;
		Assert(res1 == v1);

		float f1 = 2.0f;
		//float f2 = 0.0f;
		v1 = f1;
		//f2 = v1;
		//Assert(f1 == f2);

		LogMsg("UNIT TEST PASSED: base_math_FixedPoint");
	}


	void BaseUnitTest
	::base_template_HashTable() {
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

		LogMsg("UNIT TEST PASSED: base_template_HashTable");
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


	void BaseUnitTest
	::base_template_QuadTreeNode() {
		ElementQuadTreeNode rootNode;
		WasHere();
		rootNode.setDepth(8);
		WasHere();
		rootNode.setBounds(0, 0, 256, 256);
		WasHere();

		LogMsg("UNIT TEST PASSED: base_template_QuadTreeNode");
	}

}
