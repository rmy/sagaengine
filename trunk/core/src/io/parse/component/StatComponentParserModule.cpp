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


#include "StatComponentParserModule.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/parse/Parser.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/custom/StatComponentFactory.hpp"
#include "sim/script/Script.hpp"



namespace se_core {
	const StatComponentParserModule parserStatComponent;

	StatComponentParserModule
	::StatComponentParserModule()
			: ComponentParserModule(IoSchema::parser(), TYPE, SUBTYPE)
			, dict_(DE_COMPONENT_TYPE, TYPE, "STAT") {
	}

	
	StatComponentParserModule
	::~StatComponentParserModule() {
	}


	ComponentFactory* StatComponentParserModule
	::parse(InputStream& in) {
		StatComponentFactory* factory = new StatComponentFactory();

		static int dict_PROPERTY_TYPE = DE_PROPERTY_TYPE;
		static const DictionaryEntry deShort(dict_PROPERTY_TYPE, Property::PT_SHORT, "SHORT");
		static const DictionaryEntry deInt(dict_PROPERTY_TYPE, Property::PT_INT, "INT");
		static const DictionaryEntry deFloat(dict_PROPERTY_TYPE, Property::PT_FLOAT, "FLOAT");
		static const DictionaryEntry deScript(dict_PROPERTY_TYPE, Property::PT_SCRIPT, "SCRIPT");
		static const DictionaryEntry deAction(dict_PROPERTY_TYPE, Property::PT_ACTION, "ACTION");
		static const DictionaryEntry deArea(dict_PROPERTY_TYPE, Property::PT_AREA, "AREA");

		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			// Abilites
			case 'A':
				{
					int speed = in.readShort();
					int attack = in.readShort();
					int defense = in.readShort();
					int level = in.readShort();
					factory->setAbilities(speed, attack, defense, level);
				}
				break;

			case 'P':
				{
					String name;
					in.readString(name);
					int propertyType = in.readDictionaryWord(dict_PROPERTY_TYPE);
					Property* p = 0;
					switch(propertyType) {
					case Property::PT_SHORT:
						p = new Property(name.get(), in.readShort());
						break;
					case Property::PT_INT:
						p = new Property(name.get(), in.readInt());
						break;
					case Property::PT_FLOAT:
						p = new Property(name.get(), in.readFloat());
						break;
					case Property::PT_STRING:
						{
							String* tmp = new String();
							in.readString(*tmp);
							p = new Property(name.get(), tmp);
						}
						break;
					case Property::PT_ACTION:
						{
							String tmp;
							in.readString(tmp);
							p = new Property(name.get(), Action::lookup(tmp.get()));
						}
						break;
					case Property::PT_SCRIPT:
						{
							String tmp;
							in.readString(tmp);
							p = new Property(name.get(), Script::lookup(tmp.get()));
						}
						break;
					}
					if(p) {
						factory->properties().add(*p);
					}
				}
				break;
			default:
				LogFatal("Unsupported code '" << (char)(code) << "' in file " << in.name());
			}
		}

		return factory;
	}

}
