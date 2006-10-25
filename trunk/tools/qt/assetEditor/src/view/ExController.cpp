#include "ExController.hpp"


ExController
::ExController(QStandardItemModel& model, QListView& view)
	: QObject(0), model_(model), view_(view) {
}


void ExController
::updateList(const QModelIndex& index) {
	int type = index.data(TYPE).toInt();
	switch(type) {
	case 0:
		{
			QStringList incHeaders(index.data(INCLUDE).toStringList());
			const QStandardItemModel* sourceModel = static_cast<const QStandardItemModel*>(index.model());
			QStandardItem* root = sourceModel->item(index.row(), index.column());

			// Traverse children to get all included
			for(int i = 0; i < model_.rowCount(); ++i) {
				QStandardItem* item = model_.item(i, 0);
				QString header(item->data(HEADER_CODE).toString());
				view_.setRowHidden(i, !incHeaders.contains(header));
			}
		}
		break;
	}

}
