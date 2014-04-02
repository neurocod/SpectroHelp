//ColorTransformationTable.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorTransformationTable.h"

template<class type>
void translateBack(type a, type b, type c, type d, type & x) {
	// x is in [a b] -> [c d], but in reverse order - a->d b->c
	x = c + (x-a)*(d - c)/(b-a);
}
ColorTransformationTable::ColorTransformationTable() {
	m_v.fill(InvalidColor, SZ);
}
QRgb ColorTransformationTable::get(QRgb _col)const {
	QRgb alpha = qAlpha(_col);
	QRgb col = _col & RGB_MASK;
	QRgb & ret = m_v[col];
	if(InvalidColor==ret) {
		int h,s,v;
		QColor(col).getHsv(&h, &s, &v);
		if(s!=0)
			translate(0, 360, 60, 300, h);
		QColor c = QColor::fromHsv(h, s, v);
		ret = c.rgb();
	}
	return ret | alpha;
}