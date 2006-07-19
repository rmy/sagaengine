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


#ifndef sim_stat_SpawnPoint_hpp
#define sim_stat_SpawnPoint_hpp

#include "util/vecmath/Vector3.hpp"
#include "util/vecmath/Quat4.hpp"
#include "util/vecmath/Euler3.hpp"


namespace se_core {
	class SpawnPoint {
	public:
		SpawnPoint() {}
		SpawnPoint(Quat4& face, Vector3& disp)
				: face_(face), displace_(disp) {
		}

		SpawnPoint(Euler3& face, Vector3& disp)
				: face_(face), displace_(disp) {
		}

		/** Intitial facing direction of spawn */
		#ifdef SE_QUAT
		Quat4 face_;
		#else
		/** Intitial facing direction of spawn */
		Euler3 face_;
		#endif

		/** The spawn's displacement relative to the parent */
		Vector3 displace_;
	};
}

#endif
