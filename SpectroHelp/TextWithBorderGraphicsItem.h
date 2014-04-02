//TextWithBorderGraphicsItem.h
#pragma once
#include "GraphicsTextItem.h"

class TextWithBorderGraphicsItem: public GraphicsTextItem {
	public:
		TextWithBorderGraphicsItem(QGraphicsItem *parent = 0);
		TextWithBorderGraphicsItem(const QString & text, QGraphicsItem *parent = 0);
		TextWithBorderGraphicsItem(const QString & text, const QString & tooltip, QGraphicsItem *parent = 0);
		virtual ~TextWithBorderGraphicsItem() {}
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

		void setBorderColor(QColor color);
		void setBorderWidth(qreal width);
	protected:
		QPen m_penBorder;
		bool m_bHighlightOnHover;
		QColor m_clrBrush;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	private:
		virtual void _init();//C++11
};