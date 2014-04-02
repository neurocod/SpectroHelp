//GridItemsLayout.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "GridItemsLayout.h"

GridItemsLayout::GridItemsLayout(QGraphicsScene*scene): QObject(scene) {
	init();
	m_scene = scene;
}
GridItemsLayout::GridItemsLayout(QGraphicsItemGroup*gr) {
	init();
	m_group = gr;
}
GridItemsLayout::GridItemsLayout(QGraphicsItem*item) {
	init();
	m_item = item;
}
void GridItemsLayout::init() {
	m_scene = 0;
	m_group = 0;
	m_item = 0;
	m_sz = QSize(100, 50);
	m_bAlignCenter = true;
}
void GridItemsLayout::place(qreal x, qreal y, QGraphicsItem*item) {
	ASSERT(item);
	qreal _x = x * m_sz.width();
	qreal _y = y * m_sz.height();
	if(m_bAlignCenter) {
		auto rc = item->boundingRect();
		_x -= rc.width()/2;
		_y -= rc.height()/2;
	}
	if(m_scene) {
		m_scene->addItem(item);
	} else if(m_item) {
		item->setParentItem(m_item);
	} else {
		ASSERT(m_group);
		m_group->addToGroup(item);
	}
	_x += m_topLeft.x();
	_y += m_topLeft.y();
	item->setPos(_x, _y);
}
void GridItemsLayout::placeRow(qreal y, QList<QGraphicsItem*> items) {
	for(int x=0; x<items.count(); x++) {
		place(x, y, items[x]);
	}
}