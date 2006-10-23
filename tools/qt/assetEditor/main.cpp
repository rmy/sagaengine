#include "ui_assetEditor.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     QMainWindow *window = new QMainWindow;
     Ui_MainWindow ui;
     ui.setupUi(window);

     window->show();

	 ui.textEdit->setText("Test");

	 ui.type->setColumnCount(1);
	 QTreeWidgetItem* typeAll = new QTreeWidgetItem(ui.type, QStringList("All types"));
	 QTreeWidgetItem* typeThing = new QTreeWidgetItem(typeAll, QStringList("Thing")); 
	 QTreeWidgetItem* typeThingLogic = new QTreeWidgetItem(typeThing, QStringList("Logic"));
	 QTreeWidgetItem* typeThingAnim = new QTreeWidgetItem(typeThing, QStringList("Animations"));
	 QTreeWidgetItem* typeUnknown = new QTreeWidgetItem(typeAll, QStringList("Unknown"));


	 ui.type->addTopLevelItem(typeAll);
	 typeAll->addChild(typeThing);
	 typeAll->addChild(typeUnknown);

	 typeThing->addChild(typeThingLogic);
	 typeThing->addChild(typeThingAnim);

     return app.exec();
 }
