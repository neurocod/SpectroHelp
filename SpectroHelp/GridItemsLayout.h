//GridItemsLayout.h by Kostya Kozachuck as neurocod
#pragma once

class GridItemsLayout: public QObject {
	public:
		GridItemsLayout(QGraphicsScene*scene);
		GridItemsLayout(QGraphicsItemGroup*gr);
		GridItemsLayout(QGraphicsItem*item);
		virtual ~GridItemsLayout() {}
		void place(qreal x, qreal y, QGraphicsItem*item);
		void setSizeBetween(QSizeF sz) { m_sz = sz; }
		void setSizeBetween(qreal x, qreal y) { setSizeBetween(QSizeF(x, y)); }
		void placeRow(qreal y, QList<QGraphicsItem*> items);
		void setAlignCenter(bool b) { m_bAlignCenter = b; }
		
		QPointF m_topLeft;
	protected:
		QSizeF m_sz;
		QGraphicsScene* m_scene;
		QGraphicsItemGroup* m_group;
		QGraphicsItem* m_item;
		bool m_bAlignCenter;

		void init();//C++11
};