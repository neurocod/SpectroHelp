//DockWidget.h by Kostya Kozachuck as neurocod
#pragma once

class DockWidget: public QDockWidget {
	Q_OBJECT
	public:
		DockWidget(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
		DockWidget(QWidget * parent = 0, Qt::WindowFlags flags = 0);
		virtual ~DockWidget() {}
	signals:
		void hiddenByClose();
	protected:
		bool m_bHideOnClose;
		virtual void closeEvent(QCloseEvent * event);
};