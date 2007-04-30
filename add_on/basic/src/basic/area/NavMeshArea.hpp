#ifndef basic_area_NavMeshArea_hpp
#define basic_area_NavMeshArea_hpp

#include "basic_area.hpp"
#include "BasicPre.hpp"
#include "sim/area/Area.hpp"

namespace se_basic {

	class _SeBasicExport NavMeshArea : public se_core::Area {
	public:
		typedef ComponentPtr<NavMeshArea, se_core::sct_BLOB> Ptr;
		enum TerrainStyle {
			TS_VOID = se_core::Pos::TS_VOID,
			TS_WALKABLE
		};


		NavMeshArea(se_core::Composite* owner, const se_core::ComponentFactory* factory, se_core::String* name, coor_tile_t w, coor_tile_t h, const NavMesh* navMesh);
		virtual ~NavMeshArea();

		short index(const se_core::Point3& coor, short oldIndex = -1) const;
		coor_t groundHeight(const se_core::Point3& coor, short index = -1) const;

		void force(const se_core::Point3& coor, se_core::Vector3& dest) const;

		short terrainStyle(const se_core::Point3& coor, short index = -1) const;
		short nextTerrainStyle(bray_t direction, const se_core::Point3& coor);

		bool isLineOfSight(const se_core::Pos& from, const se_core::Pos& to);
		coor_t farthestLineOfSight(const se_core::Pos& from, bray_t yaw, coor_t maxLen, coor_t maxOffNavMesh, se_core::Point3& dest) const;
		coor_t farthestLineOfSight(const se_core::Point3& fromPoint, short fromIndex, bray_t yaw, coor_t maxLen, coor_t maxOffNavMesh) const;
		void farthestLineOfSight(const se_core::Pos& from, const se_core::Pos& to, se_core::Point3& dest) const;

		void path(const se_core::Pos& from, const se_core::Pos& to, se_core::Point3& out) const;
		bool isNeighbour(const Area& area) const;

		bray_t slideAngle(const se_core::Pos& from, const se_core::Point3& to) const;
		bray_t wallAngle(const se_core::Pos& from, const se_core::Point3& to) const;
		bool doesTouchVoid(const se_core::Point3& wc, short index, coor_t radius) const;

		coor_double_t findNearest(const se_core::Point3& p, se_core::Point3& out) const;

	private:
		short farthestLos(const se_core::Point3& from, short fromIndex, short toIndex) const;

		const NavMesh* navMesh_;
	};

}

#endif
