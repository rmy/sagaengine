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


#ifndef o3d_thing_EntityMaterial_hpp
#define o3d_thing_EntityMaterial_hpp

#include "O3dPre.H"


namespace se_ogre {

	class _SeOgreExport EntityMaterial {
	private:
		Ogre::MaterialPtr* originals_;
		Ogre::MaterialPtr* temporaries_;
		Ogre::Entity* entity_;
		int subEntityCount_;

	public:
		EntityMaterial(Ogre::Entity* e, const char* name);
		~EntityMaterial();
		void setTransparency(Ogre::Real transparency);
		void reset();

	protected:
		Ogre::MaterialPtr cloneMaterial(Ogre::MaterialPtr original, const char* cloneName);
		void setSubmeshTransparency (Ogre::MaterialPtr original
							  , Ogre::MaterialPtr temporary
							  , Ogre::Real transparency);
		void cloneEntityMaterials(const char* name);
	};


}

#endif
