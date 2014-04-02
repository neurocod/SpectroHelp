//GraphicsTextItem.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "GraphicsTextItem.h"

qreal GraphicsTextItem::s_nDefaultPointSizeIncrement = 0;

GraphicsTextItem::GraphicsTextItem(QGraphicsItem * parent): QGraphicsTextItem(parent) {
	_init();
}
GraphicsTextItem::GraphicsTextItem(const QString & text, QGraphicsItem * parent): QGraphicsTextItem(text, parent) {
	_init();
}
void GraphicsTextItem::_init() {
	if(0!=s_nDefaultPointSizeIncrement) {
		QFont f = font();
		qreal sz = f.pointSizeF();
		if(-1!=sz) {
			f.setPointSizeF(sz + s_nDefaultPointSizeIncrement);
			setFont(f);
		}
	}
}