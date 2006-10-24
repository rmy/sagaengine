#include "ViewController.hpp"
#include <QFile>
#include <QTextStream>


ViewController
::ViewController(Ui_MainWindow & ui)
	: QObject(0), ui_(ui) {
}


void ViewController
::updateText(const QModelIndex & index) {
	QString filename(index.data(FILE_PATH).toString());

	QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {	
		QTextStream in(&file);
		ui_.sourceEditor->setText(in.readAll());
	}

}
