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
		void setMass(float m);
		void setFriction(float linear, float angular, float bounceDecay);
		void setAbilities(short speed, short attack, short defence, short level);
		void setAnim(int id, int movementMode, float pos, float speed);

		/**
		 * Copy spawn points.
		 */
		void setSpawnPoints(int count, se_core::ViewPoint* const* spawnPoints);

	protected:
		// Bounding cylinder
		se_core::BoundingBox bounds_;
		bool isPickable_;
		bool isCollideable_;
		se_core::Move move_;
		const se_core::Script* script_;
		const se_core::Physics* physics_;
		const se_core::ThingCollide* collide_;
		se_core::ActionAndParameter* defaultAction_;
		se_core::Abilities abilities_;


		int spawnPointCount_;
		se_core::ViewPoint** spawnPoints_;

		se_core::Anim anim_[se_core::Pos::MAX_ANIMS];
	};

}

#endif
