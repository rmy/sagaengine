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


#ifndef angel_script_Performer_hpp
#define angel_script_Performer_hpp

#include "comp/comp.hpp"
#include <string>

namespace se_core {
	class Performer {
	public:
		Performer(const Composite& composite);

		static bool init();
		static void cleanup() {}

		void addRef() { ++refCount_; }
		void release() { --refCount_; }


		void logSelf();
		void log(std::string& s);
		bool hasTarget();
		bool didMove();
		void perform(const std::string& s);

		void defaultAction();

	private:
		const Composite& composite_;
		int refCount_;
	};

}



#endif
