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


#ifndef io_encode_EncodeManager_hpp
#define io_encode_EncodeManager_hpp

#include "../stream/OutputStream.hpp"
#include "sim/thing/sim_thing.hpp"
#include "io_encode.hpp"
#include "comp/Composite.hpp"
#include "Encoder.hpp"

namespace se_core {
	/**
	 * Save file encoders.  
	 */
	class _SeCoreExport EncodeManager : public Encoder {
	public:
		EncodeManager();
		virtual ~EncodeManager();
		void encode(OutputStream& out);
		void encode(OutputStream& out, Encoder& m);
		void add(Encoder& m);

	private:
		static const int MAX_MODULES = 12;
		int moduleCount_;
		Encoder* modules_[ MAX_MODULES ];
		Encoder* lastModule_;
	};
}

#endif
