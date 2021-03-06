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


#ifndef o3d_thing_ThingMO_hpp
#define o3d_thing_ThingMO_hpp

#include "O3dPre.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"
#include "o3d_thing.hpp"
#include "util/vecmath/ViewPoint.hpp"
#include "sim/pos/PosComponent.hpp"

namespace se_ogre {
	class _SeOgreExport ThingMO {
	public:
		bool hasAnimation() const { return hasAnimation_; }
		void setVisible(bool state);
		bool isVisible() { return isVisible_; }
		void move(long when, float stepDelta, float timeSinceLastFrame);
		void resetPos();
		virtual void animate(long when, float stepDelta, float timeSinceLastFrame) = 0;
		bool hasThing(se_core::Composite& thing) { return thing_.owner()->id() == thing.id(); }
		void setParentNode(Ogre::SceneNode* sn);
		const char* name() const { return thing_.name(); }

	protected:
		friend class ThingMOFactory;
		ThingMO(se_core::PosComponent& thing, const ThingMOInfo& info, const ThingMOFactory& factory);
		virtual ~ThingMO();


	protected:
		se_core::PosComponent& thing_;

		friend class ThingMOManager;
		const ThingMOFactory& factory_;
		const ThingMOInfo& info_;
		bool hasAnimation_;

		Ogre::SceneNode* parentNode_;
		Ogre::SceneNode* node_;
		bool isVisible_;

		Ogre::Real currentScale_;

		se_core::ViewPoint last_;
	};

}

#endif
