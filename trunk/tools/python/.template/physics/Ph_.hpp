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


#ifndef %n_%s_hpp
#define %n_%s_hpp

#include "sim/sim.hpp"
#include "sim/physics/Physics.hpp"

namespace %n {
	class _%NExport Ph%s : public se_core::Physics {
	public:
		Ph%s() : se_core::Physics("%s") {}

		void calcNext(const se_core::Actor& actor
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		void affect(se_core::Actor& actor) const;

	private:
	};


	extern _%NExport const Ph%s physics%s;

}

#endif
