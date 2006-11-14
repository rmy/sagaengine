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
#include "sim/stat/sim_stat.hpp"
#include "sim/stat/MultiSimObjectListener.hpp"
#include "sim/thing/sim_thing.hpp"
#include "sim/thing/CameraHandler.hpp"

namespace se_client {

	/**
	 * Translates some Sim events into ClientListener events.
	 */
	class _SeClientExport ClientEventBridge 
		: public se_core::MultiSimObjectListener
		, public se_core::CameraHandler {
	public:
		ClientEventBridge();
		~ClientEventBridge();
		void simObjectAddedEvent(se_core::MultiSimObject& owner, se_core::SimObject& value);
		void simObjectRemovedEvent(se_core::MultiSimObject& owner, se_core::SimObject& value);

		void cameraLeftAreaEvent(se_core::Camera& caster, se_core::Area& area);
		void cameraEnteredAreaEvent(se_core::Camera& caster, se_core::Area& area);

		void setCamera(se_core::Camera* newCamera);

		void setActive(bool state);

	private:
		bool isActive_;
	};

}

#endif
