#ifndef SimpleActorFactory_hpp
#define SimpleActorFactory_hpp

#include "BasicPre.hpp"

namespace se_basic {

	class _SeBasicExport SimpleActorFactory : public se_core::SimCompositeFactory {
	public:
		SimpleActorFactory(se_core::String* name);
		~SimpleActorFactory();

		se_core::SimComposite* create() const;

		void setBounds(const se_core::BoundingBox& b);
		void setScript(const char* name);
		void setPhysics(const char* name);
		void setCollide(const char* name);
		void setPickable(bool isPickable);
		void setCollideable(bool isCollideable);
		void setDefaultAction(const char* name);

		/**
		 * Copy spawn points.
		 */
		void setSpawnPoints(int count, se_core::ViewPoint* const* spawnPoints);

	protected:
		// Bounding cylinder
		se_core::BoundingBox bounds_;
		bool isPickable_;
		bool isCollideable_;
		const se_core::Script* script_;
		const se_core::Physics* physics_;
		const se_core::ThingCollide* collide_;
		se_core::ActionAndParameter* defaultAction_;

		int spawnPointCount_;
		se_core::ViewPoint** spawnPoints_;
	};

}

#endif
