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


#include "Script.hpp"
#include "ScriptComponent.hpp"
#include "../action/all.hpp"
#include "../thing/Actor.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	ScriptComponent
	::ScriptComponent(Actor* owner, ActionComponent* consumer)
		: SimComponent(sct_SCRIPT, owner)
		, currentScript_(0)
		, consumer_(consumer) {
		scriptStack_[currentScript_] = 0;
		consumer_->setActionFeed(this);
	}


	ScriptComponent
	::~ScriptComponent() {
		cleanup();
	}

	void ScriptComponent
	::cleanup() {
		if(consumer_)
			consumer_->resetActionFeed();
		consumer_ = 0;
		clearScripts();
	}


	void ScriptComponent
	::nextAction(const ActionComponent& performer, int channel, ActionAndParameter& out) {
		nextScriptAction(channel, out);
	}


	void ScriptComponent
	::nextScriptAction(short channel, ActionAndParameter& aap) {
		if(!script()) return;
		Parameter& p = aap.parameter();
		const Action* a = script()->nextAction(*owner_, channel, scriptData(), p);
		if(a) {
			aap.setAction(*a);
		}
	}


	void ScriptComponent
	::setDefaultScript(const Script* s) {
		if(!isActive() || currentScript_ != 0) {
			if(scriptStack_[0]) {
				scriptStack_[0]->release(scriptData_[0]);
			}
			scriptStack_[0] = s;
			scriptData_[0] = scriptStack_[0]->init(*owner_);
			return;
		}

		if(script()) script()->release(scriptData_[currentScript_]);
		scriptStack_[0] = s;
		scriptData_[0] = scriptStack_[0]->init(*owner_);
		//TODO:
		//for(int i = 0; i < CHANNEL_COUNT; ++i) {
		//	nextScriptAction(i);
		//}
	}


	void ScriptComponent
	::clearScripts() {
		// TODO:
		//if(showingCutscene_) {
		//	removeFromShowingCutscene();
		//}
		while(currentScript_ > 0) {
			scriptStack_[ currentScript_ ]->release( scriptData_[ currentScript_ ] );
			scriptStack_[ currentScript_-- ] = 0;
		}
		if(scriptStack_[0] != 0) {
			scriptStack_[ 0 ]->release( scriptData_[ 0 ] );
			scriptStack_[ 0 ] = 0;
		}
	}


	// TODO:
	//void ScriptComponent
	//::pushScript(const char* name) {
	//	// Asser(!isDead_);
	//	const Script* s = SimSchema::sortedSimObjectList().script(name);
	//	pushScript(s);
	//}


	void ScriptComponent
	::pushScript(const Script* s) {
		//TODO:
		//for(int i = 0; i < CHANNEL_COUNT; ++i) {
		//	clearPlannedAction(i);
		//	disrupt(i);
		//}

		// If there already is a script running...
		if(script() && !script()->isStacker()) {
			// Release script data
			popScript();
		}

		if(currentScript_ == 0 && scriptStack_[currentScript_] != 0) {
			++currentScript_;
		}

		Assert(currentScript_ < SCRIPT_STACK_SIZE);
		// Set the new script
		scriptStack_[currentScript_] = s;
		// Init script data
		scriptData_[currentScript_] = script()->init(*owner_);

		// If script is active
		if(isActive()) {
			// Plan actions for all channels
			for(short i = 0; i < CHANNEL_COUNT; ++i) {
				//TODO:
				//nextScriptAction(i);
			}
		}
	}


	void ScriptComponent
	::popScript() {
		// Asser(!isDead_);
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			//TODOq
			//clearPlannedAction(i);
		}
		//TODO:
		//disrupt();
		if(currentScript_ == 0) {
			Assert(script());
			script()->release(scriptData_[currentScript_]);
			scriptData_[currentScript_] = 0;
			scriptStack_[currentScript_] = 0;
			return;
		}

		if(script()) script()->release(scriptData_[currentScript_]);
		--currentScript_;
		if(!script()) return;

		script()->reinit(*owner_, scriptData());
		if(!isActive())
			return;
		for(short i = 0; i < CHANNEL_COUNT; ++i) {
			//TODO:
			//nextScriptAction(i);
		}
	}


	void ScriptComponent
	::stopScript() {
		//TODO:
		//if(showingCutscene_) {
		//	removeFromShowingCutscene();
		//}
		//else {
		Assert(script());
		script()->release(scriptData_[currentScript_]);
		scriptData_[currentScript_] = 0;
		currentScript_ = 0;
		scriptStack_[0] = 0;
		//TODO:
		//disrupt();
		//}
	}


	void ScriptComponent
	::setActive(bool state) {
		// Setting to active??
		if(isActive()) {
			// Start script
			if(script()) {
				// Init (or reinit) scripts data block
				script()->reinit(*owner_, scriptData());
				// Start script in all action channels
				for(int i = 0; i < CHANNEL_COUNT; ++i) {
					//TODO:
					//nextScriptAction(i);
				}
			}
		}
	}



}
