//BoundingRectLayoutItem.h by Kostya Kozachuck as neurocod
#pragma once

class BoundingRectLayoutItem: public QGraphicsLayoutItem {
	public:
		BoundingRectLayoutItem(QGraphicsItem*item);
		virtual ~BoundingRectLayoutItem() {}
		virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF())const;
};