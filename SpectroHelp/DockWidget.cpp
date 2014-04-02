//DockWidget.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "DockWidget.h"

DockWidget::DockWidget(const QString & title, QWidget * parent, Qt::WindowFlags flags):
		QDockWidget(title, parent, flags) {
	m_bHideOnClose = true;
	setObjectName(title);
}
DockWidget::DockWidget(QWidget * parent, Qt::WindowFlags flags): QDockWidget(parent, flags) {
	m_bHideOnClose = true;
}
void DockWidget::closeEvent(QCloseEvent * event) {
	if(m_bHideOnClose) {
		event->ignore();
		hide();
		emit hiddenByClose();
		return;
	}
	__super::closeEvent(event);
}