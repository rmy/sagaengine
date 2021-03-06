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


#include "O3dThingParser.hpp"
#include "../schema/O3dSchema.hpp"
#include "../thing/ThingMOInfo.hpp"
#include "../thing/ThingMOInfoList.hpp"
#include "../thing/O3dAnimation.hpp"
#include "../thing/O3dThingComponentFactory.hpp"
#include "../io/all.hpp"
#include "io/parse/all.hpp"
#include "io/stream/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/type/String.hpp"
#include "util/type/TmpString.hpp"
#include "util/error/Log.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/pos/Anim.hpp"
#include "sim/spawn/SpawnManager.hpp"
#include "comp/CompositeFactory.hpp"
#include <cstring>
#include <cstdio>
#include <vector>
#include <OgreEntity.h>
#include <OgreLight.h>


using namespace se_core;


namespace se_ogre {
	O3dThingParser
	::O3dThingParser(ParseManager& parser)
		: Parser(parser, Parser::OGRE, Parser::THING, 1) {
	}


	void O3dThingParser
	::parse(InputStream& in) {
		ThingMOInfoList* infoList = new ThingMOInfoList;

		int code = in.readInfoCode();
		Assert(code == 'N');
		in.readString(infoList->thingType_);

		while((code = in.readInfoCode()) != 'Q') {
			//LogDetail((char)(code));
			switch(code) {
			case '{':
				{
					ThingMOInfo* info = new ThingMOInfo();
					parseThingInfo(in, info);
					infoList->add(info);
					//LogDetail("Added info for " << infoList->thingType_);
				}
				break;

			default:
				LogFatal("Unsupported code: " << (char)(code) << " in " << in.name());
			}
		}

#ifdef SE_INTERNAL
		ThingMOInfo* info = new ThingMOInfo();
		info->movableObjectType_.set("Debug");
		infoList->add(info);
#endif

		if(infoList->infoCount_ > 0) {
			O3dSchema::thingMOManager.addInfoList(infoList);
			CompositeFactory* f = SimSchema::spawnManager().factory(infoList->thingType_.get());
			f->addComponent(&O3dSchema::otcf);
		}
		else {
			delete infoList;
		}
	}


	void O3dThingParser
	::parseThingInfo(InputStream& in, ThingMOInfo* info) {
		int code;
		while((code = in.readInfoCode()) != '}') {
			//LogDetail((char)(code));
			switch(code) {
			case 'M': // Mesh
				{
					String mesh;
					in.readString(mesh);
					info->movableObjectType_.set(Ogre::EntityFactory::FACTORY_TYPE_NAME.c_str());
					info->params_["mesh"] = mesh.get();
				}
				break;

			case 'X': // Point light
				{
					info->movableObjectType_.set(Ogre::LightFactory::FACTORY_TYPE_NAME.c_str());
					float r = in.readFloat();
					float g = in.readFloat();
					float b = in.readFloat();
					info->diffuse_ = Ogre::ColourValue(r, g, b);
					r = in.readFloat();
					g = in.readFloat();
					b = in.readFloat();
					info->specular_ = Ogre::ColourValue(r, g, b);
				}
				break;

			case 'N': // Not shadow caster
				{
					info->isShadowCaster_ = false;
				}
				break;

			case 'T': // Translation offset
				{
					info->offset_.x = in.readFloat();
					info->offset_.y = in.readFloat();
					info->offset_.z = in.readFloat();
				}
				break;

			case 'F': // Translation offset
				{
					se_core::Euler3 eTmp;
					eTmp.yaw_ = BrayT::fromDeg(in.readFloat());
					eTmp.pitch_ = BrayT::fromDeg(in.readFloat());
					eTmp.roll_ = BrayT::fromDeg(in.readFloat());

					se_core::Quat4 qTmp(eTmp);

					info->rot_.w = QuatT::toFloat(qTmp.w_);
					info->rot_.x = QuatT::toFloat(qTmp.x_);
					info->rot_.y = QuatT::toFloat(qTmp.y_);
					info->rot_.z = QuatT::toFloat(qTmp.z_);

				}
				break;

			case 'B': // Billboard
				{
					in.readString(info->defaultMaterial_);
					info->movableObjectType_.set("Billboard");
					info->params_["material"] = info->defaultMaterial_.get();
				}
				break;

			case 'G': // static Geometry
				{
					String mesh;
					in.readString(mesh);
					info->movableObjectType_.set("StaticGeometry");
					info->params_["mesh"] = mesh.get();
					info->isStatic_ = true;
				}
				break;

			case 'S': // Scale
				info->scale_ = in.readFloat();
				break;

			case 'R': // Scale is radius
				info->doScaleByRadius_ = true;
				break;

			case 'O': // movable object
				in.readString(info->movableObjectType_);
				break;

			case 'P': // param
				{
					String n, v;
					in.readString(n);
					in.readString(v);
					info->params_[ n.get() ] = v.get();
				}
				break;

			case 'C': // Animation channel count
				{
					int channelCount = in.readShort();
					info->setAnimationChannels(channelCount);
				}
				break;

			case 'A': // Animation
				{
					int channelId = in.readShort();
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					O3dAnimation* anim = info->createAnimation(channelId, animId);
					in.readString(anim->name_);
					anim->speed_ = in.readFloat();
					
					anim->doLoop_ = (in.readInfoCode() == '+');
				}
				break;

			case 'D': // Default material / texture
				{
					in.readString(info->defaultMaterial_);
				}
				break;

			case 'L': // Level of detail
				{
					info->popInSq_ = in.readFloat();
					info->popInSq_ *= info->popInSq_;
					info->popOutSq_ = in.readFloat();
					info->popOutSq_ *= info->popOutSq_;
				}
				break;

			default:
				LogFatal("Unsupported code: " << (char)(code) << " in file " << in.name());
			}
		}

		AssertFatal(!info->movableObjectType_.isEmpty(), "You must define exactly one of mesh name or factory in file " << in.name());

	}

}

