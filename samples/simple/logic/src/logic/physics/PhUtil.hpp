#ifndef logic_physics_PhUtil_hpp
#define logic_physics_PhUtil_hpp

#include "LogicPre.hpp"
#include "sim/physics/all.hpp"
#include "sim/physics/Move.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/pos/PosComponent.hpp"

namespace logic {

	class _SimpleLogicExport PhUtil : public se_core::Physics {
	public:
		PhUtil(const char* name) : se_core::Physics(name) {}
		void calcNext(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		bool isBlocked(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				) const;

		virtual void blocked(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;
		void affect(se_core::PhysicsComponent& physics) const {}

	protected:
		void rebound(const se_core::Move& move, se_core::Move& nextMove, const se_core::Point3& p1, const se_core::Point3& p2) const;
		void blendAnims(const se_core::Pos& pos, se_core::Pos& nextPos) const;
		void revertXZ(const se_core::Pos& pos, se_core::Pos& nextPos) const;
		void updateForces(const se_core::PhysicsComponent& physics, se_core::Move& nextMove) const;
		void applyFriction(se_core::Move& nextMove) const;
		void applyForces(const se_core::Pos& pos, se_core::Pos& nextPos, const se_core::Move& move, se_core::Move& nextMove) const;
		void clampToGround(const se_core::Pos& pos, se_core::Pos& nextPos, se_core::Move& nextMove) const;

		bool slide(se_core::Move& nextMove, const se_core::Point3& p1, const se_core::Point3& p2, coor_t rad, float threshold = 0.2f) const;
		void _away(se_core::Move& nextMove, const se_core::Point3& p1, const se_core::Point3& p2, coor_t speed) const;

		//static const se_core::Vector3 GRAVITY(0, -3 * se_core::COOR_STEP, 0);
		static const se_core::Vector3 GRAVITY;
		coor_t slideThreshold_;
	};



}

#endif
