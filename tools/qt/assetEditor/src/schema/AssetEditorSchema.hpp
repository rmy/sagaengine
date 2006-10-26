#ifndef AssetEditorSchema_hpp
#define AssetEditorSchema_hpp

#include "ui_assetEditor.h"
#include "../view/OgreView.hpp"
#include <QMainWindow>

namespace se_asset {

	class AESchema {
	public:
		static AESchema& singleton();
		QMainWindow *window_;
		Ui_MainWindow* ui_;
		OgreView* ogreView_;

	private:
		AESchema();
		~AESchema();
	};

}


#endif
