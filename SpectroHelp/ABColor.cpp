//ABColor.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ABColor.h"

ABColor::ABColor(QRgb c) {
	auto g = qGreen(c);
	qreal a = qRed(c) + g/2.;
	qreal b = qBlue(c) + g/2.;
	m_a = (a/1.5);
	m_b = (b/1.5);
}
QRgb ABColor::imitate(MissedChannel ch, QRgb c) {
	int r = qRed(c);
	int g = qGreen(c);
	int b = qBlue(c);
	//int gr = qGray(c);
	switch(ch) {
	case MissedR:
		r = g;
		break;
	case MissedG:
		g = r;
		break;
	case MissedB:
		b = g;
		break;
	/*case MissedR:
		r = gr;
		break;
	case MissedG:
		g = gr;
		break;
	case MissedB:
		b = gr;
		break;*/
	default:
		ASSERT(0);
	}
	//return ABColor(qRgb(r,g,b)).toRgb(ch);
	return qRgb(r,g,b);
}
QRgb ABColor::toRgb(MissedChannel m)const {
	int r,g,b;
	switch(m) {
	case MissedR:
		r = m_a;
		g = m_a;
		b = m_b;
		break;
	case MissedG:
		r = m_a;
		g = (m_a + m_b)/2;
		b = m_b;
		break;
	case MissedB:
		r = m_a;
		g = m_b;
		b = m_b;
		break;
	default:
		ASSERT(0);
	}
	return qRgb(r,g,b);
}