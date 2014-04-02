//RgbItemDelegate.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "RgbItemDelegate.h"
#include "ColorNameTableModel.h"

RgbItemDelegate::RgbItemDelegate(ColorNameTableModel*model, QObject * parent): QStyledItemDelegate(parent) {
	m_model = model;
}
//virtual
void RgbItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)const {
	__super::paint(painter, option, index);
	if(index.column()==s_nColToPaint) {
		paintRgbCell(painter, option, index);
		return;
	}
}
void RgbItemDelegate::paintRgbCell(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)const {
	auto rect = option.rect;
	auto item = m_model->item(index.row());
	QColor clr = item->m_color;
	qreal r = clr.redF();
	qreal g = clr.greenF();
	qreal b = clr.blueF();
	QColor clrPen = option.palette.color(QPalette::WindowText);
	QColor clrPenThin = clrPen;
	clrPenThin.setAlpha(128);
	QPen pen(clrPenThin);
	qreal penWidth = 3;
	pen.setCapStyle(Qt::FlatCap);

	qreal width = rect.width();
	qreal height = rect.height();
	qreal x = rect.left();
	qreal y0 = rect.top() + height/4;
	qreal dy = penWidth + 1;

	qreal y = y0;
	pen.setStyle(Qt::DotLine);
	painter->setPen(pen);
	painter->drawLine(x, y, x + width, y);
	y += dy;
	painter->drawLine(x, y, x + width, y);
	y += dy;
	painter->drawLine(x, y, x + width, y);

	pen.setStyle(Qt::SolidLine);
	pen.setWidthF(penWidth);
	pen.setColor(clrPen);
	painter->setPen(pen);
	y = y0;
	painter->drawLine(x, y, x + width*r, y);
	y += dy;
	painter->drawLine(x, y, x + width*g, y);
	y += dy;
	painter->drawLine(x, y, x + width*b, y);
}