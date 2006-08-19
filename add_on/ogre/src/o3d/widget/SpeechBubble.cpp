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


#include "O3dPre.H"
#include "SpeechBubble.hpp"
#include "../schema/O3dSchema.hpp"


using namespace se_client;
using namespace se_core;

namespace se_ogre {
	SpeechBubble
	::SpeechBubble() : speaker_(0) {
		SimSchema::messageCentral.addListener(*this);
	}


	SpeechBubble
	::~SpeechBubble() {
		SimSchema::messageCentral.removeListener(*this);
	}


	void SpeechBubble
	::infoEvent(char* text) {
		LogMsg(text);
	}


	void SpeechBubble
	::speechEvent(se_core::Actor& speaker, const char* messageName) {
		LogMsg(speaker.name());
		const char* message = ClientSchema::phrases.getPhrase(Phrase::SPEECH, messageName);
		LogMsg(message);
		speaker_ = &speaker;
		O3dSchema::window->setDebugText(message);
	}


	void SpeechBubble
	::trackUserFeedback() {
		if(speaker_)
			speaker_->script()->trackUserFeedback();
	}

}

