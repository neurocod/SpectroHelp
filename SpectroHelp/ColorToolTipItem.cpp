//ColorToolTipItem.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorToolTipItem.h"

ColorToolTipItem::ColorToolTipItem() {
	m_nMaxWidth = -1;
	m_bShowToolTip = true;
}
void ColorToolTipItem::readSettings(SettingsHash & _sett) {
	SettingsHash sett = _sett.sub("ColorToolTipItem");

	m_bShowToolTip = sett.value("show", m_bShowToolTip).toBool();
	QPointF pos = sett.value("scenePos", scenePos()).toPointF();
	QPointF pos2 = sett.value("lastColorPos", m_lastColorPos).toPointF();
	if(scene()->sceneRect().contains(pos)
			&& scene()->sceneRect().contains(pos2)) {
		setPos(pos);
		m_lastColorPos = pos2;
	}
}
void ColorToolTipItem::saveSettings(SettingsHash & _sett) {
	SettingsHash sett;

	sett.setValue("show", m_bShowToolTip);
	sett.setValue("scenePos", scenePos());
	sett.setValue("lastColorPos", m_lastColorPos);

	_sett.setSub("ColorToolTipItem", sett);
}
void ColorToolTipItem::show(QColor clr, QColor clr2) {
	if(!m_bShowToolTip) {
		hide();
		return;
	}
	m_clrDisplayed[0] = clr;
	m_clrDisplayed[1] = clr2;
	if(!clr.isValid()) {
		hide();
		return;
	}
	QString str;
	int r, g, b;
	clr.getRgb(&r,&g,&b);
	str = tr("\n\nR: %1\nG: %2\nB: %3")
		.arg(r, 3)
		.arg(g, 3)
		.arg(b, 3);
	setBorderColor(clr);
	setPlainText(str);
	setVisible(true);
}
void ColorToolTipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	__super::paint(painter, option, widget);
	if(-1==m_nMaxWidth) {
		QFontMetricsF m(font());
		m_nMaxWidth = m.width("R:  55");
	}
	ASSERT(m_clrDisplayed[0].isValid());
	int x = option->rect.left() + m_penBorder.widthF();
	int y = option->rect.top() + m_penBorder.widthF();
	int dy = 5;
	//rects with color
	QRectF rectColor(option->rect.topLeft(), QSizeF(boundingRect().width()/2, 16));
	painter->fillRect(rectColor, m_clrDisplayed[1]);

	rectColor.moveLeft(boundingRect().width()/2);
	rectColor.setWidth(option->rect.width()-rectColor.left()+0.5);
	painter->fillRect(rectColor, m_clrDisplayed[0]);

	y += rectColor.height();
	y += dy/2;
	auto y0 = y;

	int r, g, b;
	m_clrDisplayed[0].getRgb(&r,&g,&b);

	//vertical
	qreal width = m_nMaxWidth;
	painter->setPen(QPen(Qt::black));
	//painter->drawLine(x,		y-1, x,		y+dy*2);
	painter->drawLine(x+width,	y-1, x+width,	y+dy*2);

	//horizontal
	QColor clr = Qt::black;
	clr.setAlpha(128);
	QPen pen(clr);
	pen.setCapStyle(Qt::FlatCap);
	pen.setStyle(Qt::DotLine);
	painter->setPen(pen);

	painter->drawLine(x, y, x + width, y);
	y += dy;
	painter->drawLine(x, y, x + width, y);
	y += dy;
	painter->drawLine(x, y, x + width, y);

	pen.setColor(Qt::black);
	pen.setWidth(2);
	pen.setStyle(Qt::SolidLine);
	painter->setPen(pen);
	qreal wr = width * r/255.0;
	qreal wg = width * g/255.0;
	qreal wb = width * b/255.0;
	y = y0;
	painter->drawLine(x, y, x+wr, y);
	y += dy;
	painter->drawLine(x, y, x+wg, y);
	y += dy;
	painter->drawLine(x, y, x+wb, y);
}
void ColorToolTipItem::calculatePos(QPointF scenePos, QRectF sceneRect) {
	m_lastColorPos = scenePos;
	QPointF newPos = scenePos + QPointF(16, 0);
	qreal width = boundingRect().width();
	if(newPos.x() + width > sceneRect.width()) {
		newPos.rx() -= width + 16;
	} else {
		//newPos.rx() += 16;
	}
	qreal height= boundingRect().height();
	int diff = sceneRect.height() - (newPos.y() + height);
	if(diff < 0) {
		newPos.ry() += diff;
	} else {
		//newPos.ry() += 16;
	}
	setPos(newPos);
}