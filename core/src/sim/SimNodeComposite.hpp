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


#ifndef sim_SimNodeComposite_hpp
#define sim_SimNodeComposite_hpp

#include "./SimObject.hpp"
#include "./SimComposite.hpp"
#include "./SimCompositeOwner.hpp"
#include "./action/sim_action.hpp"
#include "./config/sim_config.hpp"
#include "./stat/Pos.hpp"
#include "./stat/MultiSimObject.hpp"
#include "./stat/sim_stat.hpp"
#include "./area/sim_area.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/ViewPoint.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_core {
	typedef SinglyLinkedList<class SimNodeComposite, MAX_GAME_OBJECTS, 27> Children;

	/** Container class for SimObjects that exists in the game world.
	 */
	class _SeCoreExport SimNodeComposite : public SimComposite {
	public:

		SimNodeComposite(Actor* owner);
		virtual ~SimNodeComposite();

		/**
		 * Does the Pos have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Get parent.
		 */
		SimNodeComposite* parent() { return parent_; }

		/**
		 * Get parent.
		 * const version.
		 */
		const SimNodeComposite* parent() const { return parent_; }

		/**
		 * Set parent to none.
		 * @param doKeepWorldCoor wether the local viewpoint should be updated
		 *   to maintain the world viewpoint
		 */
		void resetParent() { parent_ = 0; }

		/**
		 * Add a SimNodeComposite as a child.
		 */
		void addChild(SimNodeComposite& node);

		/**
		 * Add a SimNodeComposite from the child list.
		 */
		void removeChild(SimNodeComposite& node);

		/**
		 * Remove the SimNodeComposite from the child list of its parent.
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual void leaveCurrentParent();

		/**
		 * Move this SimNodeComposite from the child list of the old parent, to the child list of the new.
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual bool changeParent();

	protected:
		/** The parent of this position */
		SimNodeComposite* parent_;

		/** List of child SimNodeComposites  */
		Children children_;
		Children::iterator_type firstChild_;
	};

}

#endif
