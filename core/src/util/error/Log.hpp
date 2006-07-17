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


#ifndef Log_hpp
#define Log_hpp

#include <cstdlib>

extern char* log_msg();


#if defined(DEBUG_LEVEL_1) || defined(DEBUG_LEVEL_2)

namespace se_err {
	/** Must be defined in platform dependent module */
	extern void scream3(const char* file, int line, const char* msg2);
	extern void say3(const char* file, int line, const char* msg2);
	extern void dump(const char *s);
	/** Must be defined stop **/

	class Log {
	public:
		Log() : msgPos_(0), file_(0), line_(0) {
			msg_[0] = 0;
		}

		Log& file(const char* file, int line);
		Log& scream();
		Log& say();
		Log& operator << (const char* msg);
		Log& operator << (float n);
		Log& operator << (unsigned int n);
		Log& operator << (int n);
		Log& operator << (unsigned long n);
		Log& operator << (long long n);
		Log& mem(int n);

	private:
		void copy(const char* msg) {
			while(msgPos_ < MAX_MSG_SIZE - 1 && *msg != 0) {
				msg_[ msgPos_++ ] = *(msg++);
			}
			msg_[msgPos_] = 0;
		}

		static const short MAX_MSG_SIZE = 256;
		short msgPos_;
		char msg_[MAX_MSG_SIZE];

		const char* file_;
		int line_;
	};

	extern Log& log();
}
#endif

#ifdef DEBUG_LEVEL_2
#include <cstdio>
#include <cstdlib>

#ifndef IS_OGRE
void * operator new (size_t size, char const * file, int line);
void * operator new[](size_t size, char const * file, int line);
#define new new(__FILE__, __LINE__)
#endif

#define Assert(b) if(!(b)) { se_err::debugStop(); se_err::scream3(__FILE__, __LINE__, # b); }
#define DbgAssert(b) if(!(b)) { se_err::debugStop(); se_err::scream3(__FILE__, __LINE__, # b); }
//#define LogFatal(n) se_err::scream3(__FILE__, __LINE__, n)
//#define DbgLogFatal(n) se_err::scream3(__FILE__, __LINE__, n)
#define LogFatal(msg) (se_err::log().file(__FILE__, __LINE__) << msg ).scream()
#define DbgLogFatal(msg) (se_err::log().file(__FILE__, __LINE__) << msg ).scream()
//#define LogMsg(msg) se_err::say3(__FILE__, __LINE__, msg)
#define LogMsg(msg) (se_err::log().file(__FILE__, __LINE__) << msg ).say()
#define DebugExec(n) n
#define WasHere() { se_err::say3(__FILE__, __LINE__, __FUNCTION__); }
#define Dump(msg) { se_err::dump(msg); se_err::dump("\n"); }
#define DumpAllocs se_err::dumpAlloc();

namespace se_err {

	extern short debug_state;
	extern void dumpAlloc();
	extern void debugStop();

	extern int usedMem, maxUsedMem, objects, arrays, m2aCount, maxM2aCount;
}
#endif



#ifdef DEBUG_LEVEL_1
#include <cstdio>
#include <cstdlib>

#define Assert(b) if(!(b)) { se_err::scream3(__FILE__, __LINE__, # b); }
//#define LogFatal(n) se_err::scream3(__FILE__, __LINE__, n)
#define LogFatal(msg) (se_err::log().file(__FILE__, __LINE__) << msg ).scream()
#define DbgLogFatal(n)
#define DbgAssert(b)
//#define LogMsg(msg)
#define LogMsg(msg)
// se_err::debugStop()
//{ se_err::say3(__FILE__, __LINE__, __FUNCTION__); }
#define DebugExec(n)
#define WasHere()
#define Dump(msg)
#define DumpAllocs
#endif

#endif

namespace se_err {
	extern void hello();
}
#ifndef Assert
#define Assert(b)
#define LogFatal(n)
#define DbgLogFatal(n)
#define DbgAssert(b)
// Prevents strange bug in release mode ...
#define LogMsg(msg)
//se_err::hello()
#define DebugExec(n)
#define WasHere()
#define Dump(msg)
#define DumpAllocs
#endif

