#ifndef SimpleActorFactory_hpp
#define SimpleActorFactory_hpp

#include "BasicPre.hpp"
#include "sim/custom/Abilities.hpp"
#include "sim/physics/Move.hpp"
#include "comp/CompositeFactory.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "sim/pos/Anim.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/thing/ActorFactory.hpp"
#include "sim/react/CollisionComponentFactory.hpp"

namespace se_basic {

	class _SeBasicExport SimpleActorFactory : public se_core::CompositeFactory {
	public:
		SimpleActorFactory(se_core::String* name);
		~SimpleActorFactory();

		se_core::Composite* create() const;

		void setBounds(const se_core::BoundingBox& b);
		void setPhysics(const char* name);
		void setCollide(const char* name);
		void setPickable(bool isPickable);
		void setCollideable(bool isCollideable);
		void setDefaultAction(const char* name);
		void setMass(scale_t m);
		void setFriction(float linear, float angular, float bounceDecay);
		void setAnim(int id, int movementMode, float pos, float speed);
		void setSubstance(int s);
		void setDebugLevel(int level);
		void setDoObstructView(bool flag);
		void setDoObstructViewDefault();

		/**
		 * Copy spawn points.
		 */
		void setSpawnPoints(int count, se_core::ViewPoint* const* spawnPoints);

	protected:
		// Bounding cylinder
		se_core::ActorFactory actorFactory_;
		se_core::CollisionComponentFactory collisionFactory_;

		se_core::BoundingBox bounds_;
		bool isPickable_;
		bool isCollideable_;
		se_core::Move move_;
		const se_core::Physics* physics_;
		const se_core::ThingCollide* collide_;
		se_core::ActionAndParameter* defaultAction_;
		se_core::Abilities abilities_;


		int spawnPointCount_;
		se_core::ViewPoint** spawnPoints_;
		int substance_;
		int debugLevel_;
		bool doObstructView_;

		se_core::Anim anim_[se_core::Pos::MAX_ANIMS];
	};

}

#endif
