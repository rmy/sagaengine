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


#ifndef SimpleCameraFactory_hpp
#define SimpleCameraFactory_hpp

#include "GamePre.H"

namespace game {

	class SimpleCameraFactory : public se_core::ThingFactory {
	public:
		SimpleCameraFactory(se_core::String* name);
		~SimpleCameraFactory();

		se_core::Thing* create() const;

		void setRadius(float r);
		void setScript(const char* name);
		void setPhysics(const char* name);
		void setCollide(const char* name);
		void setPickable(bool isPickable);
		void setCollideable(bool isCollideable);

	private:
		coor_t radius_;
		bool isPickable_;
		bool isCollideable_;
		const se_core::Script* script_;
		const se_core::Physics* physics_;
		const se_core::ThingCollide* collide_;
	};

}

#endif
