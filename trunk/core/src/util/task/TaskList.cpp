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


#include "TaskList.hpp"

namespace se_core {

	int TaskList
	::perform(int weight) {
		int n = next();
		if(n < 0) {
			freePoints_ = 0;
			return 0;
		}
		int w = 0;
		freePoints_ += (n >= 0) ? weight : -freePoints_;
		/*
		if((n >= 0) && (w + tasks_[n]->weight() < freePoints_)) {
			tasks_[n]->perform();
			w += tasks_[n]->weight();
			tasks_[n] = tasks_[ --taskCount_ ];
			freePoints_ = 0;
		}
		*/
		while(n >= 0 && w + tasks_[n]->weight() < freePoints_) {
			tasks_[n]->perform();
			w += tasks_[n]->weight();
			tasks_[n]->isInProgress_ = false;
			tasks_[n] = tasks_[ --taskCount_ ];
			n = next();
		}
		freePoints_ -= w;
		return w;
	}

	int TaskList
	::next() {
		int n = -1;
		int p = 65536;
		for(int i = 0; i < taskCount_; ++i) {
			if(tasks_[i]->priority() < p) {
				n = i;
				p = tasks_[i]->priority();
			}
		}
		return n;
	}

	void TaskList
	::add(Task& t) {
		Assert(&t != 0);
		if(t.isInProgress_) {
			remove(t);
		}
		t.isInProgress_ = true;
		tasks_[ taskCount_++ ] = &t;
	}


	void TaskList
	::remove(Task& t) {
		if(!t.isInProgress_)
			return;

		for(int i = 0; i < taskCount_; ++i) {
			if(tasks_[ i ] == &t) {
				tasks_[ i ]->isInProgress_ = false;
				tasks_[ i ] = tasks_[ --taskCount_ ];
				--i;
			}
		}
	}

	void TaskList
	::reset() {
		for(int i = 0; i < taskCount_; ++i) {
			tasks_[ i ]->isInProgress_ = false;
		}
		taskCount_ = 0;
		freePoints_ = 0;
	}
}
