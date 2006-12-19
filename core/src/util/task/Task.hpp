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


#ifndef util_task_Task_hpp
#define util_task_Task_hpp

#include "util/type/util_type.hpp"
#include "../vecmath/Vector3.hpp"

namespace se_core {
	class _SeCoreExport Task {
	public:
		Task(int priority, int weight) : priority_(priority), weight_(0) { }
		Task() : priority_(1), weight_(256) {}
		virtual void perform() = 0;
		int priority() {
			return priority_;
		}
		int weight() {
			return weight_;
		}
		bool operator<(Task const &op) {
			return priority_ < op.priority_;
		}

		private:
			int priority_;
			int weight_;
	};
}

#endif
