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


#include "ViewPoint.hpp"
#include "util/math/all.hpp"
#include <cstdio>
#include <cstring>


namespace se_core {

	se_err::Log& operator<< (se_err::Log& log, const ViewPoint& vp) {
		log << "ViewPoint" << vp.face_ << vp.coor_;
		/*
		vp.face_.toString(log.tmp());
		log.copy(log.tmp());
		vp.coor_.toString(log.tmp());
		log.copy(log.tmp());
		*/
		return log;
	}

	const char* ViewPoint
	::toLog() const {
		static char buffer[256];

		face_.toString(buffer);
		coor_.toString(buffer + strlen(buffer) );
		return buffer;
	}

}
