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


#ifndef PulseaudioComponent_hpp
#define PulseaudioComponent_hpp

#include "sim/sim.hpp"
#include "comp/comp.hpp"
#include "comp/node/AreaChildComponent.hpp"


namespace se_pulseaudio {
	class _SePulseaudioExport PulseaudioComponent : public se_core::AreaChildComponent {
	public:
		typedef ComponentPtr<PulseaudioComponent, se_core::sct_PULSEAUDIO> Ptr;

		PulseaudioComponent(se_core::Composite* owner, const se_core::ComponentFactory* factory = 0);
		virtual ~PulseaudioComponent();
		const char* name() { return "Pulseaudio"; }

	protected:
		friend class PulseaudioComponentFactory;
		void cleanup();

		//VorbisSound* stream_;
	};

}

#endif
