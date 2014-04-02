//DemoPixelDiagram.h by Kostya Kozachuck as neurocod
#pragma once
#include "Arrow.h"
#include "ColorProfileDesc.h"

class DemoPixelDiagram: public QGraphicsRectItem {
	public:
		DemoPixelDiagram(ColorProfile pr, Mode mode, QString str=QString());
		virtual ~DemoPixelDiagram() {}
		New<TextWithBorderGraphicsItem> r0, g0, b0;
		New<TextWithBorderGraphicsItem> r1, g1, b1;

		void createArrow(QGraphicsItem*i0, QGraphicsItem*i1) {
			Arrow::connectNew(i0, i1)->setParentItem(this);
		}
		GridItemsLayout m_layout;
};