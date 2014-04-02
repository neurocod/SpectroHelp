//Arrow.h by Kostya Kozachuck as neurocod
#pragma once

class Arrow: public QGraphicsLineItem {
	public:
		Arrow();
		virtual ~Arrow() {}
		void connect(QGraphicsItem* i0, QGraphicsItem* i1);
		//creates new Arrow
		static Arrow* connectNew(QGraphicsItem* i0, QGraphicsItem* i1);
		void drawArrow(QPainter* painter, const QPointF & p1, const QPointF & p2);
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *op, QWidget *w);
};