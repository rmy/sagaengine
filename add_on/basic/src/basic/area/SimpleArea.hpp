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


#ifndef basic_area_SimpleArea_hpp
#define basic_area_SimpleArea_hpp

#include "BasicPre.hpp"
#include "sim/area/Area.hpp"

namespace se_basic {

	class _SeBasicExport SimpleArea : public se_core::Area {
	public:
		typedef ComponentPtr<SimpleArea, se_core::sct_BLOB> Ptr;

		enum TerrainStyle {
			TS_VOID = se_core::Pos::TS_VOID,
			TS_WALKABLE
		};


		SimpleArea(se_core::Composite* owner, const se_core::ComponentFactory* factory, coor_tile_t w, coor_tile_t h);
		virtual ~SimpleArea();

		short index(const se_core::Point3&, short oldIndex = -1) const { return -1; }
		coor_t groundHeight(const se_core::Point3& coor, short index = 0) const;
		void force(const se_core::Point3& coor, se_core::Vector3& dest) const;

		short terrainStyle(const se_core::Point3& coor, short index = 0) const;
		short nextTerrainStyle(bray_t direction, const se_core::Point3& coor);

		bool doesTouchVoid(const se_core::Point3 & wc, short ind, coor_t radius) const {
			return (index(wc, ind) == -1);
		}

	private:
	};

}

#endif
