//ColorNameItem.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorNameItem.h"

bool operator<(const QColor & color1, const QColor & color2) {
	return color1.hslHue()<color2.hslHue();
}
//static
QString ColorNameItem::percentage(int n255) {
	int v = 100 * n255/255.f;
	QString s = toString(v) + "%";
	static QString space = " ";
	s.prepend(space.repeated(4-s.length()));
	return s;
}
QString ColorNameItem::h()const {
	int v = m_color.hue();
	if(-1==v)
		return "-";
	return toString(v) + QString::fromWCharArray(L"°");
}
QString ColorNameItem::s()const {
	return percentage(m_color.hslSaturation());
}
QString ColorNameItem::v()const {
	return percentage(m_color.value());
}