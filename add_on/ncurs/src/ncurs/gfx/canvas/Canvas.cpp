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


#include "Canvas.hpp"
#include "util/error/Log.hpp"
#include "util/math/CoorT.hpp"
#include <cstdio>
#include <ncurses.h>

namespace se_ncurs {

	Canvas
	::Canvas() : background_(0), bgWidth_(0), bgHeight_(0) {
		initscr();
		noecho();
		nonl();
		cbreak();
		window_ = newwin(HEIGHT, WIDTH, 0, 0);
		nodelay(window_, true);
		intrflush(window_, FALSE);
		keypad(window_, TRUE);
		wclear(window_);

		output_ = newwin(5, 80, 20, 0);
		scrollok(output_, true);
		idlok(output_, true);
		//leaveok(output_, true);
		wclear(output_);
		renderBackground();

		for(int i = 0; i < KEY_MAX; ++i) {
			keyStates_[i] = 0;
		}
		resizeBackground(WIDTH, HEIGHT);
	}


	Canvas
	::~Canvas() {
		delete[] background_;
		delwin(output_);
		delwin(window_);
		endwin();
	}




	void Canvas
	::set(short x, short y, int v) {
		Assert(x >= 0 && x < WIDTH);
		Assert(y >= 0 && y < WIDTH);
		canvas_[x][y] = v;
	}

	int r(int v, int shift) {
		//return ((v + (1 << (shift - 1)) - 1) >> shift);
		return (v >> shift);
	}

	void Canvas
	::drawEnemy(short xc, short yc, int rad, int v) {
		for(int mx = 0; mx < rad; mx += COOR_RES / 8) {
			for(int my = 0; my < rad; my += COOR_RES / 4) {
				int dist = mx * mx + my * my;
				if(dist < rad * rad) {
					short x, y;
					x = r(xc + mx, COOR_SHIFT - 1);
					y = r(yc + my, COOR_SHIFT);
					if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
						set(x, y, v);
					}
					x = r(xc + mx, COOR_SHIFT - 1);
					y = r(yc - my, COOR_SHIFT);
					if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
						set(x, y, v);
					}
					x = r(xc - mx, COOR_SHIFT - 1);
					y = r(yc + my, COOR_SHIFT);
					if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
						set(x, y, v);
					}
					x = r(xc - mx, COOR_SHIFT - 1);
					y = r(yc - my, COOR_SHIFT);
					if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
						set(x, y, v);
					}
				}
			}
		}

	}

	void Canvas
	::resizeBackground(short w, short h) {
		bgWidth_ = w;
		bgHeight_ = h;

		delete[] background_;
		background_ = new short int[bgWidth_ * bgHeight_];
		for(short i = 0; i < bgWidth_ * bgHeight_; ++i) {
			background_[i] = ' ';
		}
	}

	void Canvas
	::setBackground(short x, short y, short v) {
		Assert(x >= 0 && x < bgWidth_);
		Assert(y >= 0 && y < bgHeight_);
		background_[ x + y * bgWidth_ ] = v;
	}


	void Canvas
	::renderBackground() {
		for(short y = 0; y < HEIGHT; ++y) {
			for(short x = 0; x < WIDTH; ++x) {
				if(x >= bgWidth_ || y >= bgHeight_) {
					canvas_[x][y] = ' ';
				}
				else {
					canvas_[x][y] = background_[ x + y * bgWidth_ ];
				}
			}
		}
	}


	void Canvas
	::flip() {
		werase(window_);
		for(short y = 0; y < HEIGHT; ++y) {
			for(short x = 0; x < WIDTH; ++x) {
				waddch(window_, canvas_[x][y]);
			}
		}
		wrefresh(window_);

		for(int i = 0; i < KEY_MAX; ++i) {
			if(keyStates_[i] > 1)
				--keyStates_[i];
		}
	}

	int Canvas
	::pressedKey() {
		int k;
		while((k = wgetch(window_)) != ERR) {
			if(keyStates_[ k ]) {
				keyStates_[ k ] = 4;
				continue;
			}
			keyStates_[ k ] = 4;
			return k;
		}
		return 0;
	}


	int Canvas
	::releasedKey() {
		for(int i = 0; i < KEY_MAX; ++i) {
			if(keyStates_[i] == 1) {
				--keyStates_[i];
				return i;
			}
		}
		return 0;
	}


	void Canvas
	::say(const char* msg) {
		waddch(output_, '\n');
		for(int i = 0; msg[i] != 0; ++i) {
			if(i > 1000) break;
			waddch(output_, msg[i]);
		}
		wrefresh(output_);
	}


}
