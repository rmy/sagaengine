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


#include "ActionHelperMethods.hpp"
#include "../stat/Pos.hpp"
#include "../schema/SimSchema.hpp"
#include "../message/MessageCentral.hpp"
#include "../thing/Actor.hpp"
#include "../area/Area.hpp"

#include <cstdio>

namespace se_core {



	/*
	bool ActionHelperMethods
	::isBlocking(Area* area, float x, float y) {
		if(!area->isLegalCoor(x, y)) {
			return true;
		}
		if(area->terrain(static_cast<int>(x), static_cast<int>(y)).isBlocking()) {
			return true;
		}
		return false;
	}


	bool ActionHelperMethods
	::isBlockingFront(Area* area, Pos &p) {
		if(isBlocking(area, p.getSurroundingX(0), p.getSurroundingY(0)))
			return true;
		if(isBlocking(area, p.getSurroundingX(45), p.getSurroundingY(45)))
			return true;
		if(isBlocking(area, p.getSurroundingX(-45), p.getSurroundingY(-45)))
			return true;
		return false;
	}


	void ActionHelperMethods
	::calcWhenLimit(long when, Thing& performer) {
		return;

		performer.pos().resetWhenLimitThings(when);
		performer.pos().resetWhenLimitTerrain();
		if(performer.pos().getSpeed() == 0) {
			return;
		}

		int step = static_cast<int>(1.0 / performer.pos().getSpeed());


		Area* area = performer.area();
		Pos p;
		long currentWhen = performer.pos().when();
		long w = currentWhen;
		for(;;) {
			w += step;
			if(w - currentWhen > 40000) {
				printf("calcWhenLimit was hanging...: %f\n", performer.pos().getSpeed());
				break;
			}
			p.interpolate(w, performer.pos());
			if(isBlockingFront(area, p))
				break;
		}

		int min = w - step;
		int max = w;

		while(max > min + 1) {
			p.interpolate((max + min) / 2, performer.pos());
			if(isBlockingFront(area, p)) {
				max = p.when();
			}
			else {
				min = p.when();
			}
		}
		w = min;

		performer.pos().setWhenLimitTerrain(w);
	}
	*/


	void ActionHelperMethods
	::inform(char* text) {
		SimSchema::messageCentral.info(text);
	}

	void ActionHelperMethods
	::say(Actor& speaker, char* text) {
		SimSchema::messageCentral.speech(speaker, text);
	}

}
