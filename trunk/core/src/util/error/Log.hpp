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
#include "../type/String.hpp"

extern _SeCoreExport char* log_msg();


namespace se_err {
	/** Must be defined in platform dependent module */
	extern _SeCoreExport void scream3(const char* file, int line, const char* msg2);
	extern _SeCoreExport void say3(const char* file, int line, const char* msg2);
	extern _SeCoreExport void whisper3(const char* file, int line, const char* msg2);
	extern _SeCoreExport void dump(const char *s);
	/** Must be defined stop **/

	class _SeCoreExport Log {
	public:
		Log() : msgPos_(0), file_(0), line_(0) {
			msg_[0] = 0;
		}

		Log& file(const char* file, int line);
		Log& scream();
		Log& say();
		Log& whisper();
		Log& operator << (const char* msg);
		Log& operator << (float n);
		Log& operator << (double n);
		Log& operator << (unsigned int n);
		Log& operator << (int n);
		Log& operator << (char c);
		Log& operator << (unsigned long n);
		Log& operator << (long long n);
		Log& operator << (const se_core::String& s);
		Log& operator << (const se_core::String* s);
		Log& mem(int n);

		static Log& singleton() {
			static Log log;
			return log;
		}

		void copy(const char* msg) {
			while(msgPos_ < MAX_MSG_SIZE - 1 && *msg != 0) {
				msg_[ msgPos_++ ] = *(msg++);
			}
			msg_[msgPos_] = 0;
		}

		char* tmp() {
			return tmp_;
		}

	private:
		static const short MAX_MSG_SIZE = 512;
		short msgPos_;
		char msg_[MAX_MSG_SIZE];
		char tmp_[MAX_MSG_SIZE];

		const char* file_;
		int line_;
	};
}

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
#define AssertFatal(b, msg) if(!(b)) { (se_err::Log::singleton().file(__FILE__, __LINE__) << (# b) << (" ") << msg ).scream(); }
#define AssertWarning(b, msg) if(!(b)) { (se_err::Log::singleton().file(__FILE__, __LINE__) << (# b) << (" ") << msg ).say(); }
//#define LogFatal(n) se_err::scream3(__FILE__, __LINE__, n)
//#define DbgLogFatal(n) se_err::scream3(__FILE__, __LINE__, n)
#define LogFatal(msg) (se_err::Log::singleton().file(__FILE__, __LINE__) << msg ).scream()
#define DbgLogFatal(msg) (se_err::Log::singleton().file(__FILE__, __LINE__) << msg ).scream()
//#define LogMsg(msg) se_err::say3(__FILE__, __LINE__, msg)
#define LogWarning(msg) (se_err::Log::singleton().file(__FILE__, __LINE__) << msg ).say()
#define LogMsg(msg) (se_err::Log::singleton().file(__FILE__, __LINE__) << msg ).whisper()
#define DebugExec(n) n
#define WasHere() { se_err::whisper3(__FILE__, __LINE__, __FUNCTION__); }
#define Dump(msg) { se_err::dump(msg); se_err::dump("\n"); }
#define DumpAllocs se_err::dumpAlloc();

namespace se_err {

	extern _SeCoreExport short debug_state;
	extern _SeCoreExport void dumpAlloc();
	extern _SeCoreExport void debugStop();

	extern _SeCoreExport int usedMem, maxUsedMem, objects, arrays, m2aCount, maxM2aCount;
}
#endif



#ifdef DEBUG_LEVEL_1
#include <cstdio>
#include <cstdlib>

#define Assert(b) if(!(b)) { se_err::scream3(__FILE__, __LINE__, # b); }
#define AssertFatal(b, msg) if(!(b)) { (se_err::Log::singleton().file(__FILE__, __LINE__) << (# b) << (" ") << msg ).scream(); }
#define AssertWarning(b, msg) if(!(b)) { (se_err::Log::singleton().file(__FILE__, __LINE__) << (# b) << (" ") << msg ).say(); }
//#define LogFatal(n) se_err::scream3(__FILE__, __LINE__, n)
#define LogFatal(msg) (se_err::Log::singleton().file(__FILE__, __LINE__) << msg ).scream()
#define DbgLogFatal(n)
#define DbgAssert(b)
//#define LogMsg(msg)
#define LogWarning(msg) (se_err::Log::singleton().file(__FILE__, __LINE__) << msg ).say()
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
	extern _SeCoreExport void hello();
}
#ifndef Assert
#define Assert(b)
#define AssertFatal(b, m)
#define AssertWarning(b, m)
#define LogFatal(n)
#define DbgLogFatal(n)
#define DbgAssert(b)
#define LogMsg(msg)
#define LogWarning(msg)
// Prevents strange bug in release mode ...
//se_err::hello()
#define DebugExec(n)
#define WasHere()
#define Dump(msg)
#define DumpAllocs
#endif

