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


#ifndef SimComponentIterator_hpp
#define SimComponentIterator_hpp

#include "sim_stat.hpp"
#include "../sim.hpp"
#include "../thing/sim_thing.hpp"
#include "../script/sim_script.hpp"
#include "../SimComponent.hpp"
#include "SimComponentList.hpp"
#include "../schema/SimSchema.hpp"

namespace se_core {

	class _SeCoreExport SimComponentIterator {
	public:
		SimComponentIterator();
		SimComponentIterator(MultiSimComponent& mgo);
		void init(MultiSimComponent& mgo);
		void init(short firstMode);

		inline bool hasNext() {
			return it_ != SimComponentList::end();
		}

		inline SimComponent& next() {
			return *(SimSchema::simComponentList.next(it_));
		}

	private:
		SimComponentList::iterator_type it_;
	};

}


#endif
