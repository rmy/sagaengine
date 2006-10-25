#include <QApplication>
#include <QDirModel>
#include <QStandardItemModel>
#include <QMainWindow>
#include <QStack>
#include <QTextStream>
#include "ui_assetEditor.h"
#include "view/ExController.hpp"
#include "view/ViewController.hpp"
#include "view/OgreView.hpp"

void traverseSubdir(QStringList& path, QStandardItemModel* entitiesModel) {
	// Traverse files in current dir
	QDir files(path.join("/"));
	files.setFilter(QDir::Files);

	QStringList entries(files.entryList(QStringList() << "*.txt" << "*.bin" ));
	QStringList::iterator it = entries.begin();
	while(it != entries.end()) {
		QStandardItem* item = new QStandardItem(*it);
		path << *it;
		item->setData(QVariant(path.join("/")), FILE_PATH);
		QFile file(path.join("/"));
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {	
			QTextStream in(&file);
			QString headerCode(in.readLine(4));
			item->setData(headerCode, HEADER_CODE);
		}


		path.removeLast();
		entitiesModel->appendRow(item);
		++it;
	}

	files.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList dirList(files.entryList());
	while(!dirList.isEmpty()) {
		path << dirList.takeFirst();
		traverseSubdir(path, entitiesModel);
		path.removeLast();
	}
}



int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QMainWindow *window = new QMainWindow;
	Ui_MainWindow ui;
	ui.setupUi(window);

	window->show();

	QString dataPath(QDir::homePath() + "/svn/projects/blue/code/data/");

	// Files
	QDirModel* dirModel = new QDirModel(QStringList() << "*.txt" << "*.bin"
										, QDir::AllDirs | QDir::NoDotAndDotDot
										, QDir::Name);
	ui.files->setModel(dirModel);
	ui.files->setRootIndex(dirModel->index(dataPath));
	ui.files->hideColumn(1);
	ui.files->hideColumn(2);
	ui.files->hideColumn(3);

	// Levels
	QDirModel* levelModel = new QDirModel(QStringList() << "*.txt"
										, QDir::Files
										, QDir::Name);
	ui.levels->setModel(levelModel);
	ui.levels->setRootIndex(levelModel->index(dataPath));
	ui.levels->hideColumn(1);
	ui.levels->hideColumn(2);
	ui.levels->hideColumn(3);


	// Types
	QStandardItemModel* typeModel = new QStandardItemModel;
	QFile file("types.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QStack<QStandardItem*> stack;
		stack.push(typeModel->invisibleRootItem());

		QTextStream in(&file);
		QStandardItem *item = 0;
		while (!in.atEnd()) {
			QString line = in.readLine().trimmed();
			if(line.isEmpty())
				continue;
			if(line.compare("{") == 0) {
				if(item != 0)
					stack.push(item);
				continue;
			}
			if(line.compare("}") == 0) {
				stack.pop();
				continue;
			}

			QStringList parts = line.split("/");
			item = new QStandardItem(QString(parts.takeLast()));
			item->setData(QVariant(0), TYPE);
			item->setData(QVariant(parts), INCLUDE);
			stack.top()->appendRow(item);
		}
	}
	ui.types->setModel(typeModel);

	// Entities
	QStandardItemModel* entitiesModel = new QStandardItemModel;
	QStringList path(dataPath);
	traverseSubdir(path, entitiesModel);
	ui.entities->setModel(entitiesModel);

	//
	ExController contr(*entitiesModel, *ui.entities);
	QObject::connect(ui.types, SIGNAL(clicked(QModelIndex)),
					 &contr, SLOT(updateList(QModelIndex)));


	ViewController inspect(ui);
	QObject::connect(ui.entities, SIGNAL(clicked(QModelIndex)),
					 &inspect, SLOT(updateText(QModelIndex)));


	
	return app.exec();
}
