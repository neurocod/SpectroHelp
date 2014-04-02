//DemoVisionDiagram.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "DemoVisionDiagram.h"
#include "Arrow.h"
#include "EGScene.h"

void DemoVisionDiagram::createArrow(QGraphicsItem*i0, QGraphicsItem*i1) {
	addToGroup(Arrow::connectNew(i0, i1));
}
DemoVisionDiagram::DemoVisionDiagram():
	m_grid	(this),
	Pixel	(ProfileDemo, ModeOriginal, "Pixel", this),
	R	(ProfileDemo, ModeR, rtr(L"Красный", "Red"), this),
	G	(ProfileDemo, ModeG, rtr(L"Зеленый", "Green"), this),
	B	(ProfileDemo, ModeB, rtr(L"Синий", "Blue"), this),
	scone	(ProfileB, ModeCone, rtr(L"S-колбочки", "S-cone"), this),
	mcone	(ProfileG, ModeCone, rtr(L"M-колбочки", "M-cone"), this),
	lcone	(ProfileR, ModeCone, rtr(L"L-колбочки", "L-cone"), this),
	lms	(ProfileR, ModeRG_B, "L + M - S", this),//специально, чтобы нижняя строка была белой - как сенсоры
	lm	(ProfileR, ModeLM, "L + M", this),
	l_m	(ProfileR, ModeL_M, "L - M", this),
	blueYellow(ProfileR, ModeLM_S, rtr(L"Желтый-Синий", "Yellow-Blue"), this),
	luminance(ProfileDemo, ModeImitateAll, rtr(L"Яркость", "Luminance"), this),
	redGreen(ProfileR, ModeL_M, rtr(L"Красный-Зеленый", "Red-Green"), this)
{
	setHandlesChildEvents(false);
	m_grid.setSizeBetween(100, 50);
	R->setBorderColor(Qt::red);
	G->setBorderColor(QColor(0, 200, 0));
	B->setBorderColor(Qt::blue);

	typedef QList<QGraphicsItem*> Row;
	qreal row = 0;
	m_grid.m_topLeft = QPointF(0, 30);
	m_grid.place(1, row, new GraphicsTextItem(rtr(L"Модель восприятия цвета человеком", "Model of human color perception")));
	row += 0.7;
	m_grid.place(1, row++, Pixel);
	m_grid.placeRow(row++, Row() << R << G << B);
	m_grid.placeRow(row++, Row() << lcone << mcone << scone);
	m_grid.placeRow(row++, Row() << l_m << lm);
	m_grid.place(2, row++, lms);
	m_grid.placeRow(row++, Row() << redGreen << luminance << blueYellow);

	createArrow(Pixel, R);
	createArrow(Pixel, G);
	createArrow(Pixel, B);

	createArrow(R, lcone);
	createArrow(G, mcone);
	createArrow(B, scone);

	createArrow(scone, lms);
	createArrow(mcone, lm);
	createArrow(mcone, l_m);
	createArrow(lcone, lm);
	createArrow(lcone, l_m);

	createArrow(lm, lms);
	createArrow(lm, luminance);
	createArrow(l_m, redGreen);

	createArrow(lms, blueYellow);
}