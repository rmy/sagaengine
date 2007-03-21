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


#ifndef ScriptComponent_hpp
#define ScriptComponent_hpp

#include "sim_script.hpp"
#include "comp/Component.hpp"
#include "comp/Composite.hpp"
#include "../action/ActionComponent.hpp"
#include "../action/ActionFeed.hpp"
#include "../action/sim_action.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/all.hpp"

namespace se_core {
	class _SeCoreExport ScriptComponent : public Component, public ActionFeed {
	public:
		typedef Ptr<ScriptComponent, sct_SCRIPT> Ptr;

		ScriptComponent(Composite* owner, ActionComponent* consumer);
		virtual ~ScriptComponent();

		static ScriptComponent* get(Composite& composite) {
			ScriptComponent* c = static_cast<ScriptComponent*>(composite.component(se_core::sct_SCRIPT));
			return c;
		}

		static ScriptComponent* get(Component& component) {
			ScriptComponent* c = static_cast<ScriptComponent*>(component.owner()->component(se_core::sct_SCRIPT));
			return c;
		}

		//
		// Override from ActionFeed
		void nextAction(const ActionComponent& performer, int channel, ActionAndParameter& out);

		/**
		 * Returns true if the Actor is inside an active area.
		 */
		virtual void setActive(bool state);



		/** Push a script on the script stack of the Actor.
		 *
		 * The script will be executed until it is popped, or
		 * another script is pushed on top of it.
		 */
		void pushScript(const Script* s);

		/** Push a script with a given name on the script stack of the Actor.
		 *
		 * The script will be executed until it is popped, or
		 * another script is pushed on top of it.
		 */
		void pushScript(const char* name);

		/** Pop a script from the script stack of the Actor.
		 *
		 * The script below the present script on the script stack will be resumed.
		 */
		void popScript();

		/** Return true if the Actor has an active script.
		 */
		inline bool hasActiveScript() const {
			return scriptStack_[currentScript_] != 0;
		}

		/**
		 * Get the active script of the Actor.
		 */
		inline const Script* script() const {
			return scriptStack_[currentScript_];
		}

		inline ScriptData* scriptData() {
			return scriptData_[currentScript_];
		}


		/** Clear all scripts from the script stack.
		 *
		 * Remove Actor from any running Cutscene, and stop all
		 * script execution. Performet at cleanup.
		 */
		void clearScripts();

		/** Ask currently running script for the next action in a channel.
		 *
		 *
		 */
		void nextScriptAction(short channel, ActionAndParameter& aap);

		/**
		 * Set the default script of the Actor.
		 * Sets the script that the Actor will run when no cutscenes are showing.
		 * Actions perfomed inside a Cutscene (and outside if you want) that wants to
		 * set the script that the Actor uses after the Cutscene is done, should use
		 * this method.
		 */
		void setDefaultScript(const Script* s);

		/**
		 * Stop the presently active script.
		 * Removes the actor from the showing cutscene, if any.
		 * If not, stop the default script and set the Actor to have no default
		 * script.
		 */
		void stopScript();

		void touch(void* param = 0);

	protected:
		void cleanup();
		short currentScript_;
		ActionComponent* consumer_;
		static const short SCRIPT_STACK_SIZE = 6;
		const Script* scriptStack_[SCRIPT_STACK_SIZE];
		ScriptData* scriptData_[SCRIPT_STACK_SIZE];
	};


}

#endif
