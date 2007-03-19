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
#include "comp/Composite.hpp"
#include "../sim.hpp"
#include "../pos/PosComponent.hpp"
#include "util/type/all.hpp"
#include "util/vecmath/util_vecmath.hpp"

namespace se_core {
	class _SeCoreExport SpawnComponent : public SimComponent {
	public:
		typedef Ptr<SpawnComponent, sct_SPAWN> Ptr;

		SpawnComponent(Composite* owner, PosComponent* pos);
		virtual ~SpawnComponent();

		static SpawnComponent* get(Composite& composite) {
			SpawnComponent* c = static_cast<SpawnComponent*>(composite.component(se_core::sct_SPAWN));
			return c;
		}

		static const SpawnComponent* get(const Composite& composite) {
			const SpawnComponent* c = static_cast<const SpawnComponent*>(composite.component(se_core::sct_SPAWN));
			return c;
		}

		static SpawnComponent* get(SimComponent& component) {
			SpawnComponent* c = static_cast<SpawnComponent*>(component.owner()->component(se_core::sct_SPAWN));
			return c;
		}

		static const SpawnComponent* get(const SimComponent& component) {
			const SpawnComponent* c = static_cast<const SpawnComponent*>(component.owner()->component(se_core::sct_SPAWN));
			return c;
		}


		void cleanup();

		/** Returns pointer to the spawner of a thing, or null for no spawner.
		 *
		 * A Thing cannot (by definition) collide with an Actor that spawned
		 * it. (TODO: Can a Thing be spawned? Will not all spawned things have
		 * the ability to perform Action?)
		 */
		Composite* spawner() const;

		Composite* spawn(const char* name, int spawnPointId, long deniedTsMask = 0);
		void incSpawnCount() { ++spawnCount_; }
		void decSpawnCount() { --spawnCount_; }
		int spawnCount() const { return spawnCount_; }

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
		int spawnPointCount() const {
			return spawnPointCount_;
		}

		void setSpawner(Composite* spawner);
		void resetSpawner();

		virtual void setDead();

	protected:
		PosComponent* pos_;

		/**
		 * The actor who spawned this thing.
		 * A bow shooter will for example spawn arrows when shooting. This
		 * variable is useful for making the shooter immune to shoots from
		 * his own arrows.
		 *
		 * This variable also makes it possible to maintain a spawn counter
		 * which indicates how many *living* children an actor has spawned.
		 * This enables monster spawner's to create new children whenever
		 * they fall below a certain count.
		 */
		RefPtr spawner_;

		int spawnCount_;


		/** The number of spawn points */
		int spawnPointCount_;

		/** List of spawn points associated with this SpawnComponent */
		const ViewPoint* const* spawnPoints_;
	};

}

#endif
