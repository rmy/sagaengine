#include "io/schema/IoSchema.hpp"
#include "io/stream/FileManager.hpp"
#include "ViewController.hpp"
#include "../schema/AssetEditorSchema.hpp"
#include "OgreView.hpp"
#include <QFile>
#include <QTextStream>
#include <QStringList>


ViewController
::ViewController(Ui_MainWindow & ui)
		: QObject(0), ui_(ui) {
	ogreView_ = new OgreView;
	ogreView_->setFocusPolicy(Qt::ClickFocus);
	ogreView_->setMinimumSize(105, 355);
	se_asset::AESchema::singleton().ogreView_ = ogreView_;
	ui_.tabWidget->addTab(ogreView_, "Mesh");

	se_core::IoSchema::fileManager->loadDirectory("ogre/dict");
}


void ViewController
::updateText(const QModelIndex & index) {
	QStringList filename(se_core::IoSchema::dataPath);
	filename << index.data(FILE_PATH).toString();

	QFile file(filename.join("/"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {	
		QTextStream in(&file);
		ui_.sourceEditor->setText(in.readAll());
	}
	file.close();
	se_core::IoSchema::fileManager->load(filename.last().toAscii().constData());
}
