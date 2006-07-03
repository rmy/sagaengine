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


#ifndef ncurs_gfx_canvas_Canvas_hpp
#define ncurs_gfx_canvas_Canvas_hpp


#include <ncurses.h>

namespace se_ncurs {
	class Canvas {
	public:
		Canvas();
		virtual ~Canvas();
		void set(short x, short y, int v);
		void drawEnemy(short xc, short yc, int radius, int v);
		inline int get(short x, short y, int v) { return canvas_[x][y]; }
		void flip();
		// Put here to have all ncurses code in one place
		void say(const char* msg);
		int pressedKey();
		int releasedKey();
		void renderBackground();
		void setBackground(short x, short y, short v);
		void resizeBackground(short w, short h);

	protected:
		static const short WIDTH = 30 * 2;
		static const short HEIGHT = 20;
		short canvas_[ WIDTH ][ HEIGHT ];
		short* background_;
		short int bgWidth_;
		short int bgHeight_;
		WINDOW* window_;
		WINDOW* output_;
		short keyStates_[ KEY_MAX ];
	};
}

#endif
