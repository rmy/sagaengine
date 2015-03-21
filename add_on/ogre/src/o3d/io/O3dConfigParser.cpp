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


#include "O3dConfigParser.hpp"
#include "../schema/O3dSchema.hpp"
#include "../area/O3dManager.hpp"
#include "../RenderEngine.hpp"
#include "io/parse/all.hpp"
#include "io/stream/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "O3dPre.hpp"
#include <cstring>
#include <cstdio>
#include <OgreSceneManager.h>
#include <OgreLight.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreStaticGeometry.h>


using namespace se_core;


namespace se_ogre {
	O3dConfigParser
	::O3dConfigParser(ParseManager& parser)
		: Parser(parser, Parser::OGRE, Parser::CONFIG, 1) {
	}


	void O3dConfigParser
	::parse(InputStream& in) {
		if(O3dSchema::sceneManager) {
			if(!O3dSchema::sceneManager->hasStaticGeometry("Scenery"))
				O3dSchema::sceneManager->createStaticGeometry("Scenery");
			else {
				O3dSchema::sceneManager->getStaticGeometry("Scenery")->destroy();
				O3dSchema::sceneManager->getStaticGeometry("Scenery")->reset();
			}
			O3dSchema::sceneManager->setSkyDome(false, "no_dome");
		}

		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'I': 
				{ // Include
					String file;
					in.readString(file);
					if(!IoSchema::fileManager->exists(file.get())) {
						LogFatal("Included config file " << file.get() << " does not exist.");
					}
					IoSchema::fileManager->load(file.get());
				}
				break;

			case 'R': 
				{ // Level resource
					readResources(in);
				}
				break;

			case 'S': 
				{ // SceneManager
					String sceneManager;
					in.readString(sceneManager);
					chooseSceneManager(sceneManager.get());
				}
				break;

			case 'C':
				{
					float vnear = in.readFloat();
					float vfar = in.readFloat();
					float vfovy = in.readFloat();
					createCamera(vnear, vfar, vfovy);
					createViewports();
					break;
				}

			case 'M':
				{
					String tmp;
					in.readString(tmp);
					float x = in.readFloat();
					float y = in.readFloat();
					float z = in.readFloat();

					static int unique = 0;
					char buffer[256];
					sprintf(buffer, "%s_%d(%02f,%02f,%02f)", tmp.get(), unique++, x, y, z);

					Ogre::SceneNode* node = O3dSchema::sceneManager->createSceneNode();
					Ogre::Entity* entity = O3dSchema::sceneManager->createEntity(buffer, tmp.get());
					O3dSchema::sceneManager->getStaticGeometry("Scenery")->addEntity(entity, Ogre::Vector3(x, y, z));
				}
				break;

			case 'L': // Light
				readLight(in);
				break;

			case 'D': 
				try { // Dome
					String material;
					in.readString(material);
					float curvature = in.readFloat();
					float tiling = in.readFloat();
					
					AssertFatal(O3dSchema::sceneManager,
						   "SceneManager must be created before loading ogre config file");
					O3dSchema::sceneManager->setSkyDome(true, material.get(), curvature, tiling, 512);
					LogDetail("Created skydome with material " << material);
				}
				catch(...) {
					LogDetail("Couldn't create skydome for ogre config file " << in.name());
				}
				break;

			case 'E': 
				try { // No dome
				} catch(...) {
					LogDetail("Couldn't remove skydome for ogre config file " << in.name());
				}
				break;

			case 'B': 
				try { // Box
					String material;
					in.readString(material);

					AssertFatal(O3dSchema::sceneManager,
						   "SceneManager must be created before loading ogre config file");
					O3dSchema::sceneManager->setSkyBox(true, material.get());
				}
				catch(...) {
					LogDetail("Couldn't create skybox for ogre config file " << in.name());
				}
				break;

			case 'A': 
				{
					// Area alignment
					short align = in.readShort();
					switch(align) {
					case 1:
						O3dSchema::worldManager->setCentreAligned(true);
						break;
					default:
						O3dSchema::worldManager->setCentreAligned(false);
					}
				}
				break;

			default:
				LogFatal("Unsupported code: " << (char)(code) << " in file " << in.name());
			}
		}

		if(O3dSchema::sceneManager && O3dSchema::sceneManager->hasStaticGeometry("Scenery")) {
			Ogre::StaticGeometry* sg = O3dSchema::sceneManager->getStaticGeometry("Scenery");
			//sg->setRegionDimensions(Ogre::Vector3(128, 128, 128));
			sg->build();
		}
	}


	void O3dConfigParser
	::chooseSceneManager(const char* sceneManager) {
		// Create the SceneManager, in this case a generic one
		Ogre::SceneManager* sm;
		O3dSchema::sceneManager = sm = O3dSchema::root->createSceneManager(sceneManager);
		//LogDetail("Created scene manager: " << O3dSchema::sceneManager->getTypeName().c_str());


		if(O3dSchema::overlaySystem) {
			O3dSchema::sceneManager->addRenderQueueListener(O3dSchema::overlaySystem);
		}

		// My laptop ATI Mobility Radeon 9200 needs this initial ambient light
		// even if it is changed later (or else everything goes dark)
		O3dSchema::sceneManager->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));

		//O3dSchema::sceneManager->createStaticGeometry("Scenery");
	}


	void O3dConfigParser
	::createCamera(float vnear, float vfar, float vfovy) {
		// Create the camera
		if(O3dSchema::playerCamera) {
			O3dSchema::sceneManager->destroyCamera(O3dSchema::playerCamera);
			O3dSchema::playerCamera = 0;
			O3dSchema::window->removeAllViewports();
			LogDetail("Destroyed camera");
		}

		O3dSchema::playerCamera = O3dSchema::sceneManager->createCamera("PlayerCam");
		O3dSchema::playerCamera->setNearClipDistance(vnear);
		O3dSchema::playerCamera->setFarClipDistance(vfar);
		O3dSchema::playerCamera->setFOVy(Ogre::Radian(Ogre::Degree(vfovy)));
		LogDetail("Created player camera");
	}


	void O3dConfigParser
	::createViewports(void) {
		// Create one viewport, entire window
		Ogre::Viewport* vp = O3dSchema::window->addViewport(O3dSchema::playerCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

		// Alter the camera aspect ratio to match the viewport
		O3dSchema::playerCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

		LogDetail("Created Ogre viewport");

	}



	void O3dConfigParser
	::readLight(InputStream& in) {
		int code;
		code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			case 'A': 
				{ // Ambient
					float r = in.readFloat();
					float g = in.readFloat();
					float b = in.readFloat();
					O3dSchema::sceneManager->setAmbientLight(Ogre::ColourValue(r, g, b));
				}
				break;

			case 'D':
				{ // Directional light
					Ogre::Light* light;
					if(O3dSchema::sceneManager->hasLight("MainLight")) {
						light = O3dSchema::sceneManager->getLight("MainLight");
					}
					else {
						light = O3dSchema::sceneManager->createLight("MainLight");
					}
					light->setType(Ogre::Light::LT_DIRECTIONAL);
					light->setCastShadows(false);

					while((code = in.readInfoCode()) != '/') {
						switch(code) {
						case 'V': // Direction vector
							{
								float x = in.readFloat();
								float y = in.readFloat();
								float z = in.readFloat();
								light->setDirection(x, y, z);
							}
							break;

						case 'C': // Colour
							{
								float r = in.readFloat();
								float g = in.readFloat();
								float b = in.readFloat();
								light->setDiffuseColour(Ogre::ColourValue(r, g, b));
							}
							break;

						case 'S': // Colour
							{
								float r = in.readFloat();
								float g = in.readFloat();
								float b = in.readFloat();
								light->setSpecularColour(Ogre::ColourValue(r, g, b));
							}
							break;

						case 'W': // Colour
							{
								light->setCastShadows(true);
							}
							break;

						default:
							LogFatal("Unknown directional lighting code: " << (char)(code));
						}
					}
				}
				break;

			case 'P':
				{
					String name;
					in.readString(name);
					Ogre::Light* light = O3dSchema::sceneManager->createLight(name.get());
					light->setType(Ogre::Light::LT_POINT);
					light->setCastShadows(false);

					while((code = in.readInfoCode()) != '/') {
						switch(code) {
						case 'W':
							light->setCastShadows(true);
							break;
						case 'T':
							{ // Translation							
								float x = in.readFloat();
								float y = in.readFloat();
								float z = in.readFloat();
								light->setPosition(x, y, z);
							}

						case 'V': // Direction vector
							{
								float x = in.readFloat();
								float y = in.readFloat();
								float z = in.readFloat();
								light->setPosition(x, y, z);
							}
							break;

						case 'C': // Colour
							LogDetail("Deprecated");
						case 'D': // Diffuse
							{
								float r = in.readFloat();
								float g = in.readFloat();
								float b = in.readFloat();
								light->setDiffuseColour(Ogre::ColourValue(r, g, b));
							}
							break;

						case 'S': // Specular
							{
								float r = in.readFloat();
								float g = in.readFloat();
								float b = in.readFloat();
								light->setSpecularColour(Ogre::ColourValue(r, g, b));
							}
							break;

						case 'A': // Attenuation
							{
								float range = in.readFloat();
								float constant = in.readFloat();
								float linear = in.readFloat();
								float quadratic = in.readFloat();
								light->setAttenuation(range, constant, linear, quadratic);
							}
							break;

						default:
							LogFatal("Unknown point lighting code: " << (char)(code));
						}
					}

				}
				break;

			case 'T':
				{ // Texture shadows
					short size = in.readShort();
					short count = in.readShort();
					float vfar = in.readFloat();
					float r = in.readFloat();
					float g = in.readFloat();
					float b = in.readFloat();

					try {
						if(O3dSchema::shadowSetting.value()) {
							AssertFatal(O3dSchema::sceneManager,
								   "SceneManager must be created before setting shadow technique");
							O3dSchema::sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
							O3dSchema::sceneManager->setShadowTextureSettings(size, count);
							O3dSchema::sceneManager->setShadowFarDistance(vfar);
							O3dSchema::sceneManager->setShadowColour(Ogre::ColourValue(r, g, b));

							LogDetail("Created " << count << " shadow textures of size " << size);
						}
						else {
							O3dSchema::sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
						}
					}
					catch(...) {
						LogDetail("Couldn't create shadow texture");
					}
				}
				break;

			default:
				LogFatal("Unknown lighting code: " << (char)(code));
			}
		}
	}

	void O3dConfigParser
	::readResources(InputStream& in) {
		int sectionCount = 0;
		const char* sections[8];

		int code;
		code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			case 'S': 
				{ // Level resource section
					String section;
					in.readString(section);
					sections[ sectionCount++ ] = section.copyValue();
				}
				break;
			}
		}

		if(sectionCount > 0) {
			sections[ sectionCount++ ] = 0;
			RenderEngine::singleton()->loadLevelResources(sections);
			/*
			for(int i = 0; i < sectionCount; ++i)
				delete sections[i];
			}
			*/
		}
	}

}

