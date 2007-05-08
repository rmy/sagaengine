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


#ifndef o3d_input_InputManager_hpp
#define o3d_input_InputManager_hpp

#include "o3d_input.hpp"

namespace se_ogre {
	class _SeOgreExport InputManager {
	public:
		InputManager();
		~InputManager();

		InputHandler* active();
		InputHandler* below();

		enum ShiftKeys { LSHIFT, RSHIFT, LCTRL, RCTRL, ALT, MODIFIER_COUNT };
		void setModifier(ShiftKeys key, bool state) {
			isModifierDown_[ key ] = state;
		}
	protected:
		friend class InputHandler;
		void add(InputHandler* h);
		void remove(InputHandler* h);

		void push(const char* name);
		void pop();
		InputHandler* handler(const char* name);
		void push(InputHandler* h);

		bool hasHandler(const char* name);
		int count() { return handlerCount_; }
		InputHandler* handler(int index) { return handlers_[ index ]; }

		bool isShiftDown() const { return isModifierDown_[ LSHIFT ] || isModifierDown_[ RSHIFT ]; }
		bool isAltDown() const { return isModifierDown_[ ALT ] || isModifierDown_[ ALT ]; }
		bool isControlDown() const { return isModifierDown_[ LCTRL ] || isModifierDown_[ RCTRL ]; }

		static const int MAX_STACK_SIZE = 64;
		static const int MAX_HANDLER_COUNT = 32;
		int handlerCount_;
		int handlerStackPos_;
		InputHandler* handlerStack_[ MAX_STACK_SIZE ];
		InputHandler* handlers_[ MAX_HANDLER_COUNT ];
		bool isModifierDown_[ MODIFIER_COUNT ];
	};
}

#endif
