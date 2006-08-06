#ifndef game_area_NavMeshArea_hpp
#define game_area_NavMeshArea_hpp

#include "game_area.hpp"
#include "GamePre.H"

namespace game {

	class NavMeshArea : public se_core::Area {
	public:
		enum TerrainStyle {
			TS_VOID = se_core::Pos::TS_VOID,
			TS_WALKABLE
		};


		NavMeshArea(se_core::String* name, coor_tile_t w, coor_tile_t h, const NavMesh* navMesh);
		virtual ~NavMeshArea();

		short index(const se_core::Coor& coor, short oldIndex = -1) const;
		coor_t groundHeight(const se_core::Coor& coor, short index = -1) const;

		void force(const se_core::Coor& coor, se_core::Force& dest) const;

		short terrainStyle(const se_core::Coor& coor, short index = -1) const;
		short nextTerrainStyle(bray_t direction, const se_core::Coor& coor);

		bool isLineOfSight(const se_core::Pos& from, const se_core::Pos& to);
		void path(const se_core::Pos& from, const se_core::Pos& to, se_core::Point3& out) const;

	private:
		const NavMesh* navMesh_;
	};

}

#endif
