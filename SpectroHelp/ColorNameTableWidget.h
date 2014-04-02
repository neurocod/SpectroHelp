//ColorNameTableWidget.h by Kostya Kozachuck as neurocod
#pragma once
#include "ColorNameTableView.h"

class ColorNameTableWidget: public QWidget {
	public:
		ColorNameTableWidget(QWidget*parent=0);
		virtual ~ColorNameTableWidget() {}
		ColorNameTableView* view()const { return m_view; }
	protected:
		ColorNameTableView* m_view;
};