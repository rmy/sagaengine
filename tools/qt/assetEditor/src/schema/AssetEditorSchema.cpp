#include "AssetEditorSchema.hpp"

AESchema
::AESchema() {
}


AESchema& AESchema
::singleton() {
	static AESchema s;
	return s;
}

