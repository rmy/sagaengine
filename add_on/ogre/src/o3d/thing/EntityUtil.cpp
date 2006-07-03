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


#include "O3dPre.H"
#include "EntityUtil.hpp"

namespace se_ogre {

	EntityMaterial
	::EntityMaterial(Ogre::Entity* e, const char* name) {
		entity_ = e;
		subEntityCount_ = entity_->getNumSubEntities();
		originals_ = new Ogre::MaterialPtr[ subEntityCount_ ];
		temporaries_ = new Ogre::MaterialPtr[ subEntityCount_ ];

		// Assign a temprary material
		cloneEntityMaterials(name);
	}


	EntityMaterial
	::~EntityMaterial() {
		reset();
		delete[] temporaries_;
		delete[] originals_;
	}


	void EntityMaterial
	::reset() {
		// Iterate through subentities
		for(int i = 0; i < subEntityCount_; ++i) {
			// Reset to original material
			Ogre::SubEntity* se = entity_->getSubEntity(i);
			se->setMaterialName(originals_[i]->getName());
			// Delete temproary material
			if (!temporaries_[i].isNull ()) {
				Ogre::MaterialManager::getSingleton().remove(temporaries_[i]->getName ());
				temporaries_[i].setNull();
			}
		}
	}


	void EntityMaterial
	::setSubmeshTransparency (Ogre::MaterialPtr original
					   , Ogre::MaterialPtr temporary
					   , Ogre::Real transparency) {
		if (transparency > 1.0f)
			transparency = 1.0f;

		unsigned short i = 0, j;

		Ogre::ColourValue sc, dc; // Source colour, destination colour
		Ogre::Material::TechniqueIterator techniqueIt = temporary->getTechniqueIterator ();
		while (techniqueIt.hasMoreElements ()) {
			Ogre::Technique *t = techniqueIt.getNext ();
			Ogre::Technique::PassIterator passIt = t->getPassIterator ();
			j = 0;
			while (passIt.hasMoreElements ()) {
				//sc = original->getTechnique(i)->getPass(j)->getDiffuse();
				//dc = sc;
				//dc.a = sc.a * (1.0f - transparency);
				dc = temporary->getTechnique(i)->getPass(j)->getDiffuse();
				dc.a = (1.0f - transparency);
				passIt.peekNext()->setAmbient(original->getTechnique(i)->getPass(j)->getAmbient());
				passIt.peekNext()->setDiffuse(dc);
				passIt.moveNext();

				++j;
			}
			++i;
		}
	}



	void EntityMaterial
	::setTransparency (Ogre::Real transparency) {
		for (int i = 0; i < subEntityCount_; i++) {
			Ogre::SubEntity* se = entity_->getSubEntity(i);
			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(se->getMaterialName());
			setSubmeshTransparency(originals_[i], temporaries_[i], transparency);
		}
	}


	Ogre::MaterialPtr EntityMaterial
	::cloneMaterial(Ogre::MaterialPtr original, const char* cloneName) {
		// Clone
		Ogre::MaterialPtr material = original->clone(cloneName);

		// Return material
		return material;
	}


	void EntityMaterial
	::cloneEntityMaterials(const char* name) {
		char cloneName[256];
		for (int i = 0; i < subEntityCount_; i++) {
			Ogre::SubEntity* se = entity_->getSubEntity(i);
			originals_[i] = Ogre::MaterialManager::getSingleton().getByName(se->getMaterialName());

			sprintf(cloneName, "%s.%d", name, i);
			LogMsg(cloneName);
			temporaries_[i] = cloneMaterial(originals_[i], cloneName);
			se->setMaterialName(cloneName);
		}
	}

}
