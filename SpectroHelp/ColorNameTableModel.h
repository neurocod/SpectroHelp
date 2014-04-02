//ColorNameTableModel.h by Kostya Kozachuck as neurocod
#pragma once
#include "ColorNameList.h"

class ColorNameTableModel: public QAbstractTableModel {
	MAYBE_SUPER(QAbstractTableModel)
	public:
		ColorNameTableModel(QObject*parent=0);
		virtual ~ColorNameTableModel() {}
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)const;
		virtual QVariant data(const QModelIndex& index, int nRole = Qt::DisplayRole)const override;
		enum Columns {
			ColName=0,
			ColHex,
			ColColor,
			ColR,
			ColG,
			ColB,
			ColH,
			ColS,
			ColV,

			ColEnd
		};
		virtual int columnCount(const QModelIndex & parent = QModelIndex())const override {
			return ColEnd;
		}
		virtual int rowCount(const QModelIndex & parent = QModelIndex())const override {
			return m_items.count();
		}
		const ColorNameItem* item(int row)const;
	protected:
		QList<const ColorNameItem*> m_items;
};