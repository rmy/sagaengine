#ifndef basic_area_NavMeshArea_hpp
#define basic_area_NavMeshArea_hpp

#include "basic_area.hpp"
#include "BasicPre.hpp"

namespace se_basic {

	class _SeBasicExport NavMeshArea : public se_core::Area {
	public:
		enum TerrainStyle {
			TS_VOID = se_core::Pos::TS_VOID,
			TS_WALKABLE
		};


		NavMeshArea(se_core::String* name, coor_tile_t w, coor_tile_t h, const NavMesh* navMesh);
		virtual ~NavMeshArea();

		short index(const se_core::Point3& coor, short oldIndex = -1) const;
		coor_t groundHeight(const se_core::Point3& coor, short index = -1) const;

		void force(const se_core::Point3& coor, se_core::Vector3& dest) const;

		short terrainStyle(const se_core::Point3& coor, short index = -1) const;
		short nextTerrainStyle(bray_t direction, const se_core::Point3& coor);

		bool isLineOfSight(const se_core::Pos& from, const se_core::Pos& to);
		void farthestLineOfSight(const se_core::Pos& from, const se_core::Pos& to, se_core::Point3& dest);
		void farthestLineOfSight(const se_core::Pos& from, bray_t yaw, coor_t maxLen, coor_t maxOffNavMesh, se_core::Point3& dest);
		coor_t farthestLineOfSight(const se_core::Pos& from, bray_t yaw, coor_t maxLen, coor_t maxOffNavMesh);

		void path(const se_core::Pos& from, const se_core::Pos& to, se_core::Point3& out) const;
		bool isNeighbour(const Area& area) const;

	private:
		const NavMesh* navMesh_;
	};

}

#endif
