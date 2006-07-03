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


#ifndef base_template_QuadTreeNode_hpp
#define base_template_QuadTreeNode_hpp

#include "SinglyLinkedList.hpp"
#include "../error/Log.hpp"
#include "util/type/util_type.hpp"
#include "../bounds/BoundingSquare.hpp"

namespace se_core {

	/**
	 * QuadTreeNode template.
	 * The template should be subclassed to implement data containers.
	 */
	template <class SubClass> class QuadTreeNode {
	public:
		/**
		 * Contructor.
		 * Create an empty QuadTreeNode with no parent_ and no children.
		 */
		QuadTreeNode()
			: parent_(0), children_(0), childLevels_(0) {
		}


		virtual ~QuadTreeNode() {
			delete[] children_;
		}


		/**
		 * Create depth levels of child nodes.
		 * @params depth The number of child nodes to create.
		 */
		void setDepth(short depth) {
			childLevels_ = depth;
			if(depth <= 0) return;
			Assert(children_ == 0);
			children_ = new SubClass[ 4 ];
			for(short i = 0; i < 4; ++i) {
				children_[i].setDepth(reinterpret_cast<SubClass*>(this), depth - 1);
			}
		}


		/**
		 * Set the bounds of this node and its child
		 * nodes. The parameters sent in are the
		 * tight bounds of the root node.
		 */
		void setBounds(coor_t minX, coor_t minY,
					   coor_t maxX, coor_t maxY) {
			looseBounds_.setMax(minX, minY);
			tightBounds_.setMax(minX, minY);

			// Halfsizes rounded up
			coor_t xSize = CoorT::half(maxX - minX + 1);
			coor_t ySize = CoorT::half(maxY - minY + 1);

			looseBounds_.setMin(
							   minX - xSize,
							   minY - ySize
							   );
			looseBounds_.setMax(
							   maxX + xSize,
							   maxY + ySize
							   );

			if(children_) {
				children_[0].setBounds
					(
					 minX, minY,
					 maxX - xSize, maxY - ySize
					 );

				children_[1].setBounds
					(
					 minX + xSize, minY,
					 maxX, maxY - ySize
					 );

				children_[2].setBounds
					(
					 minX, minY + ySize,
					 maxX - xSize, maxY
					 );

				children_[3].setBounds
					(
					 minX + xSize, minY + ySize,
					 maxX, maxY
					 );
			}
		}


		/**
		 * Returns the correct node for x, y coordinates. If no other nodes
		 * fit, the root node will be returned. It is your responsibility
		 * to not send in coordinates that are not inside the this QuadTreeNode
		 * at all.
		 *
		 * This method will usually be called one the root node of the quad tree.
		 */
		SubClass* findNode(coor_t x, coor_t y, coor_t radius) {
			Assert(tightBounds_.hasInside(x, y));
			Assert(radius < tightBounds_.radius());

			SubClass* node = this;
			while(node->tightBounds_.radius() > radius && node->children_) {
				for(short i = 0; i < 4; ++i) {
					if(children_[i].tightBounds_.hasInside(x, y)) {
						node = &children_[i];
						break;
					}
				}
			}

			return node;
		}


	protected:
		/**
		 * Sets parent_ and creates another depth
		 * level of children.
		 */
		void setDepth(SubClass* parent, short depth) {
			parent_ = parent;
			setDepth(depth);
		}

		SubClass* parent_;
		SubClass* children_;
		BoundingSquare tightBounds_;
		BoundingSquare looseBounds_;
		int childLevels_;
	};
}

#endif
