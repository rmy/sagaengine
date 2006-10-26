#include "AssetEditorSchema.hpp"
#include "../io/AeO3dThingParserModule.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/InitListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_asset {
	AESchema
	::AESchema()
		: window_(0), ui_(0), ogreView_(0) {
	}


	AESchema
	::~AESchema() {
	}

	AESchema& AESchema
	::singleton() {
		static AESchema s;
		return s;
	}


	class _SeAeExport AutoInit : public se_core::InitListener {
	public:
		AutoInit() {
			// Create and register parser modules
			static AeO3dThingParserModule thingPM(IoSchema::parser());

			// Register physics

			// Register as init event listener
			SimSchema::initListeners().addListener(*this);
			LogMsg("Registered Asset editor add-on");
		}

		~AutoInit() {
			SimSchema::initListeners().removeListener(*this);
			LogMsg("Cleaned up Asset editor add-on");
		}

		void initEngineEvent() {
		}
		void cleanupEngineEvent() {}
		void initGameEvent() {}
		void cleanupGameEvent() {}
	} autoInit;
};
