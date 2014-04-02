//ColorNameTableView.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorNameTableView.h"
#include "ColorNameTableModel.h"
#include "RgbItemDelegate.h"

ColorNameTableView::ColorNameTableView(ColorNameTableModel*m, QWidget*parent): QTableView(parent) {
	m_model = m;
	setModel(m);
	setItemDelegate(new RgbItemDelegate(m, this));
	//horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	horizontalHeader()->resizeSection(0, 150);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setContextMenuPolicy(Qt::NoContextMenu);
}
void ColorNameTableView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected) {
	__super::selectionChanged(selected, deselected);
	QModelIndexList li = selectionModel()->selectedRows();
	if(li.size()!=1)
		return;
	int row = li[0].row();
	QColor color = m_model->item(row)->m_color;
	emit colorSelected(color);
}