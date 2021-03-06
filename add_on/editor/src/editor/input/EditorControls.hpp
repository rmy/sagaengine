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


#ifndef EditorControls_hpp
#define EditorControls_hpp

#include "sim/sim.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/script/Script.hpp"
#include "o3d/input/InputHandler.hpp"
#include "util/vecmath/ViewPoint.hpp"

namespace se_editor {
	class _SeEditorExport EditorControls : public se_ogre::InputHandler, public se_core::Script {
	public:
		EditorControls();
		virtual ~EditorControls();
		void _init();
		void grabbedFocusEvent() {}
		void lostFocusEvent();
		void showMenu(int slot, bool mod3);

		void nextAction(const se_core::ScriptComponent& performer, int channel, se_core::ScriptData* sd, se_core::ActionAndParameter& out) const;
		void mouseDragged(const OIS::MouseEvent* e);
		void mouseMoved(const OIS::MouseEvent* e);
		void keyPressed(const OIS::KeyEvent* e);
		void keyReleased(const OIS::KeyEvent* e);
		void mousePressed(const OIS::MouseEvent* e, int button);
		int modifier(bool mod1, bool mod2);

		static EditorControls& singleton();

	protected:
		void setAction(int channel, const se_core::Action& a, const se_core::Parameter* p = 0, int deadline = -1);
		void setDefaultAction(int channel, const se_core::Action& a, const se_core::Parameter* p = 0);
		void spawnCreature(int id, bool mod1, bool mod2, bool mod3);
		void placeEntrance(int id);

		se_core::ViewPoint cursor_;
		bool isRelative_;
		mutable se_core::ActionAndParameter plannedAction_[se_core::CHANNEL_COUNT];
		mutable se_core::ActionAndParameter defaultAction_[se_core::CHANNEL_COUNT];

		int modifier_;
	};


}

#endif
