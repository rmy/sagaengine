#ifndef ExController_hpp
#define ExController_hpp

#include <qobject.h>
#include <qstandarditemmodel.h>
#include <qlistview.h>
#include "ui_assetEditor.h"
#include "enums.hpp"


class ExController : public QObject {
	Q_OBJECT

public:
	ExController(QStandardItemModel& model, QListView& view);

	public slots:
	void updateList(const QModelIndex& index);

	signals:
	void myUsefulSignal(int);

private:
	QStandardItemModel& model_;
	QListView& view_;
	//QStringList x_;
};


#endif
