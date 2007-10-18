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


#ifndef CollisionManager_hpp
#define CollisionManager_hpp

#include "sim_react.hpp"
#include "CollisionGrid.hpp"
#include "../sim.hpp"
#include "comp/node/RootComponent.hpp"


namespace se_core {

	/**
	 * Base class for functionality component managers.
	 */
	class _SeCoreExport CollisionManager
		: public RootComponent {
	public:
		/**
		 * Construct CollisionManager.
		 */
		CollisionManager();


		/**
		 * Destructor.
		 */
		virtual ~CollisionManager();


		void step(long when);

		static CollisionManager& singleton();

		CollisionGrid* grabCollisionGrid();
		void releaseCollisionGrid(CollisionGrid* g);

		void resolveContacts();
		void getContactList();

		void flipGodMode() {
			isGodModeOn_ = !isGodModeOn_;
		}

		void resetGodMode() {
			isGodModeOn_ = false;
		}

	private:
		static const int MAX_ACTIVE = 3 * (7 * 7 * 7);
		int gridCount_, gridPoolCount_;
		bool isGodModeOn_;
		CollisionGrid** collisionGrids_;
		CollisionGrid** gridPool_;

		static const int MAX_CONTACTS = 1024;
		short contactCount_;
		Contact* contactList_;

	};

}


#endif

