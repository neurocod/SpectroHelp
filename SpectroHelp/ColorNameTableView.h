//ColorNameTableView.h by Kostya Kozachuck as neurocod
#pragma once
class ColorNameTableModel;

class ColorNameTableView: public QTableView {
	Q_OBJECT
	public:
		ColorNameTableView(ColorNameTableModel*m, QWidget*parent=0);
		virtual ~ColorNameTableView() {}
	signals:
		void colorSelected(QColor color);
	protected slots:
		virtual void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
	protected:
		ColorNameTableModel*m_model;
};