#include "ViewController.hpp"
#include "OgreView.hpp"
#include <QFile>
#include <QTextStream>


ViewController
::ViewController(Ui_MainWindow & ui)
		: QObject(0), ui_(ui) {
	ogreView_ = new OgreView;
	ogreView_->setFocusPolicy(Qt::ClickFocus);
	ogreView_->setMinimumSize(105, 355);
	ui_.tabWidget->addTab(ogreView_, "Mesh");
}


void ViewController
::updateText(const QModelIndex & index) {
	QString filename(index.data(FILE_PATH).toString());

	QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {	
		QTextStream in(&file);
		ui_.sourceEditor->setText(in.readAll());
	}

	ogreView_->addObject("blob.mesh");
}
