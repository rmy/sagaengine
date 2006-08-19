#ifndef SimpleActorFactory_hpp
#define SimpleActorFactory_hpp

#include "BasicPre.hpp"

namespace se_basic {

	class _SeBasicExport SimpleActorFactory : public se_core::ThingFactory {
	public:
		SimpleActorFactory(se_core::String* name);
		~SimpleActorFactory();

		se_core::Thing* create() const;

		void setRadius(float r);
		void setScript(const char* name);
		void setPhysics(const char* name);
		void setCollide(const char* name);
		void setPickable(bool isPickable);
		void setCollideable(bool isCollideable);

		/**
		 * Copy spawn points.
		 */
		void setSpawnPoints(int count, se_core::ViewPoint* const* spawnPoints);

	protected:
		coor_t radius_;
		bool isPickable_;
		bool isCollideable_;
		const se_core::Script* script_;
		const se_core::Physics* physics_;
		const se_core::ThingCollide* collide_;

		int spawnPointCount_;
		se_core::ViewPoint** spawnPoints_;
	};

}

#endif
