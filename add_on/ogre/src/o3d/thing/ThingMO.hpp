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

#include "O3dPre.H"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"
#include "o3d_thing.hpp"

namespace se_ogre {
	class ThingMO {
	public:
		bool hasAnimation() const { return hasAnimation_; }
		void setVisible(bool state);
		void move(float stepDelta);
		virtual void animate(float stepDelta, float timeSinceLastFrame) = 0;

	protected:
		friend class ThingMOFactory;
		ThingMO(const ThingMOFactory& factory, const ThingMOInfo& info);
		~ThingMO();


	private:
		se_core::Thing* thing_;

		friend class ThingMOManager;
		const ThingMOFactory& factory_;
		const ThingMOInfo& info_;
		bool hasAnimation_;

		Ogre::SceneNode* node_;
		bool isVisible_;

		Ogre::Real currentScale_;
	};

}

#endif
