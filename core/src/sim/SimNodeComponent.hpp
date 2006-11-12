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


#ifndef sim_SimNodeComponent_hpp
#define sim_SimNodeComponent_hpp

#include "./SimObject.hpp"
#include "./SimComponent.hpp"
#include "./SimComposite.hpp"
#include "./action/sim_action.hpp"
#include "./config/sim_config.hpp"
#include "./pos/Pos.hpp"
#include "./stat/SimComponentList.hpp"
#include "./stat/sim_stat.hpp"
#include "./area/sim_area.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/ViewPoint.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_core {
	typedef SinglyLinkedList<class SimNodeComponent, MAX_GAME_OBJECTS, 27> Children;

	/** Container class for SimObjects that exists in the game world.
	 */
	class _SeCoreExport SimNodeComponent : public SimComponent {
	public:

		SimNodeComponent(SimComposite* owner);
		SimNodeComponent(enum SimComponentType type, SimComposite* owner);
		virtual ~SimNodeComponent();

		/**
		 * Does the Pos have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Get parent.
		 */
		SimNodeComponent* parent() { return parent_; }

		/**
		 * Get parent.
		 * const version.
		 */
		const SimNodeComponent* parent() const { return parent_; }

		/**
		 * Set parent to none.
		 * @param doKeepWorldCoor wether the local viewpoint should be updated
		 *   to maintain the world viewpoint
		 */
		void resetParent();

		/**
		 * Add a SimNodeComponent as a child.
		 */
		void addChild(SimNodeComponent& node);

		/**
		 * Add a SimNodeComponent from the child list.
		 */
		void removeChild(SimNodeComponent& node);


		void setParent(SimNodeComponent& p);

	protected:
		void parentChanged(SimComposite* oldParent, SimComposite* newParent);

		/** The parent of this position */
		SimNodeComponent* parent_;

		/** List of child SimNodeComponents  */
		MultiSimComponent children_;
	};

}

#endif
