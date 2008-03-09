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


#ifndef client_io_PlayerEncoder_hpp
#define client_io_PlayerEncoder_hpp

#include "io/encode/Encoder.hpp"
#include "io/stream/io_stream.hpp"
#include "../thing/client_thing.hpp"
#include "sim/area/sim_area.hpp"
#include "sim/stat/sim_stat.hpp"
#include "sim/pos/sim_pos.hpp"
#include "sim/sim.hpp"
#include "sim/thing/sim_thing.hpp"


namespace se_client {
	class _SeClientExport PlayerEncoder : public se_core::Encoder  {
	public:
		PlayerEncoder(se_core::EncodeManager& parser);
		virtual ~PlayerEncoder();
		void encode(se_core::OutputStream& in);
	};

}

#endif
