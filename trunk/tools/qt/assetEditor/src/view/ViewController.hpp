#ifndef ViewController_hpp
#define ViewController_hpp

#include "ui_assetEditor.h"
#include "enums.hpp"
#include "OgreView.hpp"
#include <qobject.h>
#include <qstandarditemmodel.h>

class ViewController : public QObject {
	Q_OBJECT

public:
	ViewController(Ui_MainWindow& ui);

	public slots:
	void updateText(const QModelIndex& index);

private:
	OgreView* ogreView_;
	Ui_MainWindow & ui_;
};


#endif
