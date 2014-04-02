//GraphicsBoxLayout.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "GraphicsBoxLayout.h"

GraphicsBoxLayout::GraphicsBoxLayout(QGraphicsScene*scene, Qt::Orientation or, int margin, int spacing) {
	m_scene = scene;
	m_orientation = or;
	m_lastPos = 0;
	m_spacing = spacing;
	m_margin = margin;
	m_nItemsAdded = 0;
	//reserve invisible item for bounding rect
	m_scene->addLine(0,0,1,1)->hide();
}
void GraphicsBoxLayout::addItem(QGraphicsItem * item) {
	if(0==m_nItemsAdded++) {
		m_lastPos += m_margin;
	} else {
		m_lastPos += m_spacing;
	}
	m_scene->addItem(item);
	qreal sz = maxSizeRecursive(item);
	if(Qt::Vertical==m_orientation) {
		item->setPos(0, m_lastPos);
	} else {
		item->setPos(m_lastPos, 0);
	}
	m_lastPos += sz;
}
GraphicsBoxLayout & GraphicsBoxLayout::operator<<(QGraphicsItem * item) {
	addItem(item);
	return *this;
}
qreal GraphicsBoxLayout::maxSizeRecursive(QGraphicsItem * item)const {
	qreal ret = size(item);
	for(QGraphicsItem*sub: item->childItems()) {
		QPointF pos = sub->boundingRect().bottomRight();
		qreal rpos = (Qt::Vertical==m_orientation) ? pos.y() : pos.x();
		rpos += maxSizeRecursive(sub);
		if(rpos>ret)
			ret = rpos;
	}
	return ret;
}
qreal GraphicsBoxLayout::size(QGraphicsItem * item)const {
	QRectF rect = item->boundingRect();
	if(Qt::Vertical==m_orientation)
		return rect.height();
	return rect.width();
}
void GraphicsBoxLayout::setSpacing(qreal s) {
	ASSERT(s>=0);
	if(s>=0)
		m_spacing = s;
}
void GraphicsBoxLayout::addEnd() {
	m_lastPos += m_margin;
	auto item = new QGraphicsLineItem(0,0,1,1);
	if(Qt::Vertical==m_orientation) {
		item->setPos(0, m_lastPos);
	} else {
		item->setPos(m_lastPos, 0);
	}
	m_scene->addItem(item);
}