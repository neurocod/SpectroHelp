//DemoPixelDiagram.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "TextWithBorderGraphicsItem.h"
#include "GridItemsLayout.h"
#include "DemoPixelDiagram.h"
#include "DemoScene.h"

DemoPixelDiagram::DemoPixelDiagram(ColorProfile pr, Mode mode, QString str):
		m_layout(this),
		r0("Red"), g0("Green"), b0("Blue"),
		r1("Red"), g1("Green"), b1("Blue") {
	auto desc = ColorProfileDesc::get(pr);
	if(str.isEmpty())
		str = rtr(L"%2 (Профиль: %1, Режим: %2)", "%2 (Profile: %1, Mode: %2)")
		.arg(desc->name)
		.arg(desc->mode(mode).name);
	QString tooltip = desc->mode(mode).tooltip;
	m_layout.setSizeBetween(50, 50);
	int row = 0;
	if(!str.isEmpty()) {
		New<GraphicsTextItem> item(str);
		item->setData(DemoScene::s_nKeyProfile, (int)pr);
		item->setData(DemoScene::s_nKeyMode, (int)mode);
		item->setToolTip(tooltip);
		m_layout.place(1.5, 0.2 + row, item);
		row++;
	}
	QList<QGraphicsItem*> li; li
		<< r0 << g0 << b0
		<< r1 << g1 << b1;
	for(auto item: li) {
		item->setData(DemoScene::s_nKeyProfile, (int)pr);
		item->setData(DemoScene::s_nKeyMode, (int)mode);
		item->setToolTip(tooltip);
	}
	m_layout.place(0, row, r0);
	m_layout.place(1, row, g0);
	m_layout.place(2, row, b0);

	row++;
	m_layout.place(0, row, r1);
	m_layout.place(1, row, g1);
	m_layout.place(2, row, b1);

	QColor clr;
	if(ProfileR==pr || ModeR==mode) clr = Qt::red;
	if(ProfileG==pr || ModeG==mode) clr = Qt::green;
	if(ProfileB==pr || ModeB==mode) clr = Qt::blue;
	if(clr.isValid()) {
		QList<TextWithBorderGraphicsItem*> li; li << r0 << r1 << g0 << g1 << b0 << b1;
		for(auto i: li)
			i->setBorderColor(clr);
	}
}