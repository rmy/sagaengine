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


#ifndef comp_NodeComponent_hpp
#define comp_NodeComponent_hpp

#include "comp/comp.hpp"
#include "comp/Component.hpp"
#include "comp/list/NodeComponentList.hpp"


namespace se_core {
	/** Container class for Objects that exists in the game world.
	 */
	class _SeCoreExport NodeComponent : public Component {
	public:
		NodeComponent(int type, Composite* owner, const ComponentFactory* factory = 0);
		virtual ~NodeComponent();

		/**
		 * Does the Pos have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Get parent.
		 */
		NodeComponent* parent() { return parent_; }

		/**
		 * Get parent.
		 * const version.
		 */
		const NodeComponent* parent() const { return parent_; }

		/**
		 * Set parent to none.
		 * @param doKeepWorldCoor wether the local viewpoint should be updated
		 *   to maintain the world viewpoint
		 */
		void resetParent();

		void setParent(NodeComponent& p);

		NodeComponentList& children() {
			return children_;
		}

	protected:
		void parentChanged(Composite* newParent, Composite* oldParent);

		void cleanup() {
			resetParent();
		}

		/**
		 * Add a NodeComponent as a child.
		 * Called by setParent of child.
		 */
		void addChild(NodeComponent& node);

		/**
		 * Add a NodeComponent from the child list.
		 * Called by setParent of child.
		 */
		void removeChild(NodeComponent& node);



		/** The parent of this position */
		NodeComponent* parent_;

		/** List of child NodeComponents  */
		NodeComponentList children_;
	};

}

#endif
