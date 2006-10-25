#ifndef AssetEditorSchema_hpp
#define AssetEditorSchema_hpp

#include "ui_assetEditor.h"

class AESchema {
public:
	static AESchema& singleton();
	Ui_MainWindow* ui_;

private:
	AESchema();
};


#endif
