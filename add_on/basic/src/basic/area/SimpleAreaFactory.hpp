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


#ifndef SimpleAreaFactory_hpp
#define SimpleAreaFactory_hpp


#include "basic_area.hpp"
#include "sim/area/AreaFactory.hpp"
#include "BasicPre.hpp"

namespace se_basic {

	class _SeBasicExport SimpleAreaFactory : public se_core::AreaFactory {
	public:
		SimpleAreaFactory(se_core::String* name, coor_tile_t w, coor_tile_t h, se_core::ByteArray* data = 0);
		~SimpleAreaFactory();
		se_core::Area* create(se_core::String* name, int pageX, int pageY, int pageZ, int gridId) const;

	protected:
		se_core::ByteArray* data_;
		coor_tile_t width_, height_;
	};

}

#endif
