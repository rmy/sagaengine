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


#ifndef ClientEventBridge_hpp
#define ClientEventBridge_hpp

#include "sim/sim.hpp"
#include "sim/InitListener.hpp"
#include "sim/stat/sim_stat.hpp"
#include "sim/stat/MultiSimObjectListener.hpp"
#include "sim/thing/sim_thing.hpp"
#include "sim/thing/CameraHandler.hpp"

namespace se_client {

	/**
	 * Translates MultiSimObject events for Areas into ClientEvents.
	 */
	class _SeClientExport ClientEventBridge : public MultiSimObjectListener, public CameraHandler, public InitListener {
	public:
		ClientEventBridge();
		~ClientEventBridge();
		void simObjectAddedEvent(MultiSimObject& owner, SimObject& value);
		void simObjectRemovedEvent(MultiSimObject& owner, SimObject& value);

		void cameraLeftAreaEvent(Camera& caster, Area& area);
		void cameraEnteredAreaEvent(Camera& caster, Area& area);

		void initEngineEvent() {}
		void cleanupEngineEvent() {}
		void initGameEvent();
		void cleanupGameEvent();


		void setCamera(Camera* newCamera);
	};

}

#endif
