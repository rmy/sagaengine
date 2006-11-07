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


#ifndef SpawnComponent_hpp
#define SpawnComponent_hpp

#include "../SimComponent.hpp"
#include "../sim.hpp"
#include "../pos/PosComponent.hpp"
#include "util/type/all.hpp"
#include "util/vecmath/util_vecmath.hpp"

namespace se_core {
	class _SeCoreExport SpawnComponent : public SimComponent {
	public:
		SpawnComponent(SimComposite* owner, PosComponent* pos);
		virtual ~SpawnComponent();

		void cleanup();

		Thing* spawn(const char* name, int spawnPointId, long deniedTsMask = 0);
		void incSpawnCount() { ++spawnCount_; }
		void decSpawnCount() { --spawnCount_; }
		int spawnCount() { return spawnCount_; }

		/** Set the list of spawn points associated with this PosNode.
		 *
		 * @param count the number of spawn points
		 * @param spawnPoints the list of spawn points
		 */
		void setSpawnPoints(int count, const ViewPoint* const* spawnPoints);

		/** Get the reference to a spawn point
		 *
		 * @param id the id of the sapwn points
		 */
		const ViewPoint* spawnPoint(short id) const;

	protected:
		PosComponent* pos_;

		int spawnCount_;

		/** The number of spawn points */
		int spawnPointCount_;

		/** List of spawn points associated with this SpawnComponent */
		const ViewPoint* const* spawnPoints_;
	};

}

#endif
