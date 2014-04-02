//RgbItemDelegate.h by Kostya Kozachuck as neurocod
#pragma once
class ColorNameTableModel;

class RgbItemDelegate: public QStyledItemDelegate {
	public:
		RgbItemDelegate(ColorNameTableModel*model, QObject * parent = 0);
		virtual ~RgbItemDelegate() {}
		virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)const;

		static const int s_nColToPaint = 2;//see ColorNameItem
		ColorNameTableModel* m_model;
	protected:
		void paintRgbCell(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)const;
};