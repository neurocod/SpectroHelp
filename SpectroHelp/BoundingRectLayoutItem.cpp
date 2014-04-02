//BoundingRectLayoutItem.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "BoundingRectLayoutItem.h"

BoundingRectLayoutItem::BoundingRectLayoutItem(QGraphicsItem*item) {
	setGraphicsItem(item);
}
QSizeF BoundingRectLayoutItem::sizeHint(Qt::SizeHint which, const QSizeF & constraint)const {
	if(graphicsItem()) {
		return graphicsItem()->boundingRect().size();
	}
	return QSizeF();
}