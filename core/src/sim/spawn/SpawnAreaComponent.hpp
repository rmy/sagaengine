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


#ifndef SpawnAreaComponent_hpp
#define SpawnAreaComponent_hpp

#include "sim/sim.hpp"
#include "sim/pos/sim_pos.hpp"
#include "comp/Composite.hpp"
#include "comp/node/RootChildComponent.hpp"
#include "comp/list/CompositeList.hpp"
#include "util/vecmath/Vector3.hpp"
#include "util/vecmath/ViewPoint.hpp"


namespace se_core {
	class _SeCoreExport SpawnAreaComponent : public RootChildComponent {
	public:
		typedef ComponentPtr<SpawnAreaComponent, sct_SPAWN_AREA> Ptr;
		
		/** Constructor.
		 */
		SpawnAreaComponent(Composite* owner, const ComponentFactory* factory = 0);
		~SpawnAreaComponent();

		static SpawnAreaComponent* get(Composite& composite) {
			SpawnAreaComponent* c = static_cast<SpawnAreaComponent*>(composite.component(se_core::sct_SPAWN_AREA));
			return c;
		}

		static const SpawnAreaComponent* get(const Composite& composite) {
			const SpawnAreaComponent* c = static_cast<const SpawnAreaComponent*>(composite.component(se_core::sct_SPAWN_AREA));
			return c;
		}

		static SpawnAreaComponent* get(Component& component) {
			SpawnAreaComponent* c = static_cast<SpawnAreaComponent*>(component.owner()->component(se_core::sct_SPAWN_AREA));
			return c;
		}

		static const SpawnAreaComponent* get(const Component& component) {
			const SpawnAreaComponent* c = static_cast<const SpawnAreaComponent*>(component.owner()->component(se_core::sct_SPAWN_AREA));
			return c;
		}



		void setActive(bool state);
		void flipSpawns(void);
		Composite* spawn(const char* thingName, const ViewPoint& vp, PosComponent* parent = 0);

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

		bool hasSpawnPoint(short id) const {
			return id >= 0 && id < spawnPointCount_ && spawnPoint(id) != 0;
		}

		const char* name() const { return "SpawnArea"; }

	protected:
		friend class SpawnAreaComponentFactory;

		/** List of waiting to be spawned  */
		CompositeList newSpawns_;

		/** The number of spawn points */
		int spawnPointCount_;

		/** List of spawn points associated with this SpawnComponent */
		const ViewPoint* const* spawnPoints_;
	};
}

#endif
