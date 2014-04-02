//Arrow.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "Arrow.h"

Arrow::Arrow() {
	QColor myColor = Qt::black;
	setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
void Arrow::connect(QGraphicsItem* i0, QGraphicsItem* i1) {
	ASSERT(i0 && i1);
	ASSERT(i0->pos().y()<i1->pos().y());//need more complex -> use C:\Qt\4.7.4-32\examples\graphicsview\diagramscene\arrow.cpp
	auto r0 = i0->boundingRect();
	auto r1 = i1->boundingRect();
	QPointF p0(i0->pos());
	QPointF p1(i1->pos());
	p0.rx() += r0.width()/2;
	p1.rx() += r1.width()/2;
	p0.ry() += r0.height();
	setLine(QLineF(p0, p1));
}
//static
Arrow* Arrow::connectNew(QGraphicsItem* i0, QGraphicsItem* i1) {
	New<Arrow> ret;
	ret->connect(i0, i1);
	return ret;
}
void Arrow::drawArrow(QPainter* painter, const QPointF & p1, const QPointF & p2) {
	QPointF p3 = p1 - p2;
	qreal dx = p3.x();
	qreal dy = p3.y();

	qreal angle = atan2(dy, dx);
	const qreal dAngle = PI/6;
	qreal angle0 = angle + dAngle;
	qreal angle1 = angle - dAngle;
	qreal len = 7;

	QPointF p;

	p.rx() = p2.x() + len*cos(angle0);
	p.ry() = p2.y() + len*sin(angle0);
	painter->drawLine(p, p2);

	p.rx() = p2.x() + len*cos(angle1);
	p.ry() = p2.y() + len*sin(angle1);
	painter->drawLine(p, p2);
}
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *op, QWidget *w) {
	__super::paint(painter, op, w);
	auto _line = line();
	drawArrow(painter, _line.p1(), _line.p2());
}