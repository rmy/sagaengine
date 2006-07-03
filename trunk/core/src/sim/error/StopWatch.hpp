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


#ifndef StopWatch_hpp
#define StopWatch_hpp

namespace se_core {
	class StopWatch {
	public:
		StopWatch();
		virtual ~StopWatch() {}
		virtual long currentSystemTime();
		virtual float systemTimeAsMillis(long t) { return static_cast<float>(t); }

		long systemTimeUsed(int n) {
			return stopTime[n] - startTime[n];
		}

		float millisUsed(int n) {
			return systemTimeAsMillis(systemTimeUsed(n));
		}

		float maxMillisUsed(int n) {
			return systemTimeAsMillis(maxUsed[n]);
		}

		void setN(int n) {
			(*this).n = n;
		}

		char* millisUsedAsString();

		void start() {
			startTime[n] = currentSystemTime();
		}

		void stop() {
			stopTime[n] = currentSystemTime();
			if(systemTimeUsed(n) > maxUsed[n] || (systemTimeAsMillis(lastMaxUsed[n]) < systemTimeAsMillis(stopTime[n]) - 1000)) {
				lastMaxUsed[n] = stopTime[n];
				maxUsed[n] = systemTimeUsed(n);
			}
			++n;
		}


	private:
		long startTime[10], stopTime[10];
		long maxUsed[10], lastMaxUsed[10];
		int n;
	};

}

#endif
