//ColorNameTableWidget.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorNameTableWidget.h"
#include "ColorNameTableModel.h"

ColorNameTableWidget::ColorNameTableWidget(QWidget*parent): QWidget(parent) {
	setWindowTitle(rtr(L"����������� �����", "Named colors"));
	CompactVBoxLayout lay(this);
	auto label = new QLabel(
		rtr(L"�������� ������ � ������ ��� ������ ����� ����� �� �����������",
		"Select row with color to show that color on the image"));
	label->setWordWrap(true);
	lay << label;
	New<ColorNameTableModel> model(this);
	m_view = new ColorNameTableView(model);
	lay << m_view;
}