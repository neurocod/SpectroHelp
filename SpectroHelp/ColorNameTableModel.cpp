//ColorNameTableModel.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorNameTableModel.h"

ColorNameTableModel::ColorNameTableModel(QObject*parent) {
	ColorNameList li;
	m_items = li.items();
}
QVariant ColorNameTableModel::headerData(int section, Qt::Orientation orientation, int role)const {
	if(Qt::Horizontal==orientation && role==Qt::DisplayRole) {
		switch(section) {
		case ColName: return tr("name");
		case ColHex: return tr("hex");
		case ColColor: return tr("color");
		case ColR: return tr("red");
		case ColG: return tr("green");
		case ColB: return tr("blue");
		case ColH: return tr("hue");
		case ColS: return tr("saturation");
		case ColV: return tr("value");
		}
		ASSERT(0);
		return QVariant();
	}
	if(Qt::Vertical==orientation) {
		auto item = m_items[section];
		if(Qt::BackgroundColorRole == role) {
			return QVariant(item->m_color);
		}
		if(Qt::BackgroundRole==role) {
			return QVariant(item->m_color);
		}
		if(Qt::DecorationRole==role) {
			return QVariant(item->m_color);
		}
	}
	return QVariant();
}
QVariant ColorNameTableModel::data(const QModelIndex& index, int role)const {
	if(role==Qt::DisplayRole) {
		int col = index.column();
		auto item = this->item(index.row());
		if(!item)
			return QVariant();
		switch(col) {
			case ColName:  return item->m_name;
			case ColHex:   return item->m_color;
			case ColColor: return QVariant();
			case ColR:	   return item->r();
			case ColG:	   return item->g();
			case ColB:	   return item->b();
			case ColH:	   return item->h();
			case ColS:	   return item->s();
			case ColV:	   return item->v();
		}
	}
	if(index.column()==0) {
		auto item = m_items[index.row()];
		if(Qt::DecorationRole==role) {
			return QVariant(item->m_color);
		}
	}
	return QVariant();
}
const ColorNameItem* ColorNameTableModel::item(int row)const {
	if(!isIndexValid(m_items, row))
		return 0;
	return m_items[row];
}