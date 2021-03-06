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


#include "Log.hpp"
#include "../system/util_system.hpp"
#include "../math/BrayT.hpp"
#include <malloc.h>
#ifdef _WINDOWS
#include <Windows.h>
#endif
#include <cstdio>
#include <cstdlib>

namespace se_err {
	void dump(const char *s) {
#	ifdef _WINDOWS
		OutputDebugString(s);
		OutputDebugString("\n");
#	endif

		fputs(s, stderr);
		fputs("\n", stderr);
		fflush(stderr);
	}


	void scream3(const char* file, int line, const char* msg) {
		char buffer[512];
		sprintf(buffer, "Fat: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);

#	ifdef _WINDOWS
		sprintf(buffer, "(%s-%d)", file, line);
		::MessageBox (0, msg, buffer, MB_ICONEXCLAMATION | MB_OK);
#	endif
		// Unhandled exception throws to debugger in Visual C++.
		throw 0.0f;
	}
	void say3(const char* file, int line, const char* msg) {
		char buffer[512];
		sprintf(buffer, "Wrn: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);
	}
	void whisper3(const char* file, int line, const char* msg) {
		char buffer[512];
		sprintf(buffer, "Msg: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);
	}
	void silent3(const char* file, int line, const char* msg) {
		char buffer[512];
		sprintf(buffer, "Det: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);
	}

	void debugStop() {
		static int count = 0;
		//say3(__FILE__, __LINE__, s);
		++count;
	}
}

char* log_msg() {
	static char message[512] VAR_IN_EWRAM;
	return message;
}


namespace se_err {
	/*
	Log& Log
	::singleton() {
		static Log lg;
		return lg;
	}
	*/


	Log& Log
	::file(const char* file, int line) {
		msgPos_ = 0;
		msg_[0] = 0;
		line_ = line;
		file_ = file;
		return *this;
	}


	Log& Log
	::scream() {
		scream3(file_, line_, msg_);
		return *this;
	}


	Log& Log
	::say() {
		say3(file_, line_, msg_);
		return *this;
	}


	Log& Log
	::whisper() {
		whisper3(file_, line_, msg_);
		return *this;
	}


	Log& Log
	::silent() {
		silent3(file_, line_, msg_);
		return *this;
	}


	Log& Log
	::operator << (const char* msg) {
		copy(msg);
		return *this;
	}


	Log& Log
	::operator << (char c) {
		sprintf(tmp_, "%c", c);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (float n) {
		sprintf(tmp_, "%f", n);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (double n) {
		sprintf(tmp_, "%f", n);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (int n) {
		sprintf(tmp_, "%d", n);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (unsigned int n) {
		sprintf(tmp_, "%d", n);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (bray_t n) {
		sprintf(tmp_, "%f", se_core::BrayT::toDeg(n));
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (unsigned long n) {
		sprintf(tmp_, "%d", n);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (long n) {
		sprintf(tmp_, "%d", n);
		copy(tmp_);
		return *this;
	}


	Log& Log
	::operator << (const se_core::String& s) {
		copy(s.get());
		return *this;
	}


	Log& Log
	::operator << (const se_core::String* s) {
		if(!s) 
			copy("(null)");
		else
			copy(s->get());
		return *this;
	}



	Log& Log
	::mem(int n) {
		sprintf(tmp_, "%x", n);
		copy(tmp_);
		return *this;
	}

	short debug_state = 0;
	int usedMem = 0;
	int maxUsedMem = 0;
	int objects = 0;
	int arrays = 0;
	int allocCount = 0;
	struct Alloc {
		const char* file;
		int line;
		short count;
		short prevCount;
		int totalSize;
	} allocs[200];

	struct Mem2Alloc {
		void* p;
		Alloc* a;
		unsigned short size;
	} *m2a = 0;
	int m2aCount = 0;
	int maxM2aCount = 0;



}

#ifdef new
#undef new

#ifdef delete
#undef delete
#endif

namespace se_err {

	Alloc& findAlloc(const char* file, int line) {
		for(int i = 0; i < allocCount; ++i) {
			if(allocs[i].file == file && allocs[i].line == line) {
				return allocs[i];
			}
		}
		allocs[allocCount].file = file;
		allocs[allocCount].line = line;
		allocs[allocCount].count = 0;
		allocs[allocCount].prevCount = 0;
		allocs[allocCount].totalSize = 0;
 		return allocs[allocCount++];
	}

	void dumpAlloc() {
		dump(";;--------------------------\n");
		for(int i = 0; i < allocCount; ++i) {
			if(allocs[i].count == 0) continue;
			Alloc& a = allocs[i];


			if(a.count != a.prevCount)
				dump((sprintf(log_msg(), ";; %5d, %d - %s(%d)\n", a.totalSize, a.count, a.file, a.line), log_msg()));
			a.prevCount = a.count;
		}
		int m2aSize = 0;
		for(int j = 0; j < m2aCount; ++j) {
			m2aSize += m2a[j].size;
		}

		dump(";;+++++++++++++++++++++++++++++++\n");
		dump((sprintf(log_msg(), ";;Objects: %d, Arrays: %d\n", se_err::objects, se_err::arrays), log_msg()));
		dump((sprintf(log_msg(), ";;Allocs: %d, MaxAllocs: %d\n", se_err::m2aCount, se_err::maxM2aCount), log_msg()));
		dump((sprintf(log_msg(), ";;Alloc places: %d\n", allocCount), log_msg()));
		dump((sprintf(log_msg(), ";;Allocated bytes (now, max): %d, %d - check: %d\n", se_err::usedMem, se_err::maxUsedMem, m2aSize), log_msg()));
		dump(";;+++++++++++++++++++++++++++++++\n");
	}

	void addP(const char* file, int line, void* p, unsigned short size) {
		static Mem2Alloc pool[ 2000 ];
		if(!m2a) m2a = pool; //(Mem2Alloc*)malloc(sizeof(Mem2Alloc) * 2000);
		Alloc& a = findAlloc(file, line);
		++a.count;
		a.totalSize += size;
 		m2a[m2aCount].p = p;
		m2a[m2aCount].a = &a;
		m2a[m2aCount].size = size;
		if(usedMem > maxUsedMem) maxUsedMem = usedMem;
		usedMem += size;
		++m2aCount;
		if(m2aCount > maxM2aCount)
			maxM2aCount = m2aCount;
		//Assert(m2aCount < 4000);
	}

	int findM2A(void* p) {
		//Assert(m2a);
		if(!m2a) return -1;
		for(int i = 0; i < m2aCount; ++i) {
			if(m2a[i].p == p) {
				return i;
			}
		}
		// Bug!
		//Assert(false);
		return -1;
	}

	void delP(void* p) {
		int index = findM2A(p);
		if(index < 0) return;
		unsigned short size = m2a[index].size;
		usedMem -= size;
		m2a[index].a->count--;
		m2a[index].a->totalSize -= size;
		--m2aCount;
		m2a[index].a = m2a[m2aCount].a;
		m2a[index].size = m2a[m2aCount].size;
		m2a[index].p = m2a[m2aCount].p;
	}
}

using namespace se_err;

void * operator new (size_t size, char const * file, int line) {
	Alloc* p = (Alloc*)malloc (size);
	++objects;
	addP(file, line, p, size);
	return (p + 0);
}


void * operator new (size_t size) {
	Alloc* p = (Alloc*)malloc (size);
	++objects;
	addP(__FILE__, __LINE__, p, size);
	return (p + 0);
}


void operator delete (void * p, char const * file, int line) {
	--objects;
	delP(p);
	free (((Alloc*)p) - 0);
}


void operator delete (void * p) {
	--objects;
	delP(p);
	free (((Alloc*)p) - 0);
}


void * operator new[] (size_t size, char const * file, int line) {
	Alloc* p = (Alloc*)malloc (size);
	++arrays;
	addP(file, line, p, size);
	return p + 0;
}


void * operator new[] (size_t size) {
	Alloc* p = (Alloc*)malloc (size);
	++arrays;
	addP(__FILE__, __LINE__, p, size);
	return p + 0;
}


void operator delete[] (void * p, char const * file, int line) {
	--arrays;
	delP(p);
	free (((Alloc*)p) - 0);
}


void operator delete[] (void * p) {
	--arrays;
	delP(p);
	free (((Alloc*)p) - 0);
}

#endif

