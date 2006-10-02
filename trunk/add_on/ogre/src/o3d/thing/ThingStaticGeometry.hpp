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


#ifndef o3d_thing_ThingStaticGeometry_hpp
#define o3d_thing_ThingStaticGeometry_hpp

#include "O3dPre.hpp"
#include "ThingMO.hpp"
#include "o3d_thing.hpp"
#include "sim/thing/sim_thing.hpp"

namespace se_ogre {

	/**
	 * An entity visually representing a se_core::Thing
	 * in 3D.
	 */
	class _SeOgreExport ThingStaticGeometry : public ThingMO {
	public:
		void animate(long when, float stepDelta, float timeSinceLastFrame) {}

	protected:
		friend class ThingStaticGeometryFactory;
		/**
		 * @param thing The thing that this entity is visually representing.
		 */
		ThingStaticGeometry(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory);
		~ThingStaticGeometry();
		void addToStaticGeometry(const char* name);

	private:
		Ogre::Entity* entity_;
		Ogre::StaticGeometry* staticGeometry_;
	};

}


#endif
