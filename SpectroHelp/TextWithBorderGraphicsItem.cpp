//TextWithBorderGraphicsItem.cpp
#include "pch.h"
#include "TextWithBorderGraphicsItem.h"

TextWithBorderGraphicsItem::TextWithBorderGraphicsItem(QGraphicsItem *parent):
		GraphicsTextItem(parent) {
	_init();
}
TextWithBorderGraphicsItem::TextWithBorderGraphicsItem(const QString & text, QGraphicsItem *parent):
		GraphicsTextItem(text, parent) {
	_init();
}
TextWithBorderGraphicsItem::TextWithBorderGraphicsItem(const QString & text, const QString & tooltip, QGraphicsItem *parent):
		GraphicsTextItem(text, parent) {
	_init();
	setToolTip(tooltip);
}
void TextWithBorderGraphicsItem::_init() {
	m_clrBrush = QColor(230, 230, 230);
	m_bHighlightOnHover = true;
	m_penBorder = QPen(Qt::black);
	m_penBorder.setWidth(1);
}
void TextWithBorderGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setPen(m_penBorder);
	painter->setBrush(m_clrBrush);
	painter->drawRect(option->rect);
	QGraphicsTextItem::paint(painter, option, widget);
}
void TextWithBorderGraphicsItem::setBorderColor(QColor color) {
	if(m_penBorder.color()!=color) {
		m_penBorder.setColor(color);
		update();
	}
}
void TextWithBorderGraphicsItem::setBorderWidth(qreal width) {
	m_penBorder.setWidthF(width);
	update();
}
void TextWithBorderGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
	if(m_bHighlightOnHover)
		setBorderWidth(2);
	__super::hoverEnterEvent(event);
}
void TextWithBorderGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
	if(m_bHighlightOnHover)
		setBorderWidth(1);
	__super::hoverLeaveEvent(event);
}