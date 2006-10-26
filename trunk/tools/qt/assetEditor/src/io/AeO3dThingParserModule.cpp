#include "AeO3dThingParserModule.hpp"

#include "../schema/AssetEditorSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "sim/config/sim_config.hpp"

using namespace se_core;

namespace se_asset {
	AeO3dThingParserModule
	::AeO3dThingParserModule(se_core::Parser& parser)
		: ParserModule(parser, ParserModule::OGRE, ParserModule::THING, 1) {
	}


	void AeO3dThingParserModule
	::parse(se_core::InputStream& in) {
		int code;
		while((code = in.readInfoCode()) != 'Q') {
			LogMsg((char)(code));
			switch(code) {
			case 'N': // Name
				{
					String type;
					in.readString(type);
					break;
				}

			case 'M': // Mesh
				{
					String mesh;
					in.readString(mesh);
					AESchema::singleton().ogreView_->addObject(mesh.get());
				}
				break;

			case 'B': // Billboard
				{
					String materialName;
					in.readString(materialName);
				}
				break;

			case 'G': // static Geometry
				{
					String mesh;
					in.readString(mesh);
				}
				break;

			case 'S': // Scale
				{
					float scale = in.readFloat();
				}
				break;

			case 'R': // Scale is radius
				{
					bool doScaleByRadius_ = true;
				}
				break;

			case 'O': // movable object
				{
					String moType;
					in.readString(moType);
				}
				break;

			case 'P': // param
				{
					String n, v;
					in.readString(n);
					in.readString(v);
				}
				break;

			case 'C': // Animation channel count
				{
					int channelCount = in.readShort();
				}
				break;

			case 'A': // Animation
				{
					int channelId = in.readShort();
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					String name;
					in.readString(name);
					float speed = in.readFloat();
				}
				break;

			case 'D': // Default material / texture
				{
					String material;
					in.readString(material);
				}
				break;

			case 'L': // Level of detail
				{
					float popInSq = in.readFloat();
					float popOutSq = in.readFloat();
				}
				break;

			default:
				LogFatal("Unsupported code: " << (char)(code));
			}
		}

	}

}
