//RgbToHsv.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "RgbToHsv.h"

const Hsv RgbToHsv::InvalidColor(0xffff, 0xff, 0xff);
//static
const RgbToHsv& RgbToHsv::instance() {
	static RgbToHsv s;
	return s;
}
RgbToHsv::RgbToHsv() {
	m_v.fill(InvalidColor, SZ);
}
Hsv RgbToHsv::get(QRgb _col)const {
	QRgb col = _col & RGB_MASK;
	Hsv & ret = m_v[col];
	if(InvalidColor==ret) {
		ret = Hsv(col);
	}
	return ret;
}
void RgbToHsv::get(QRgb rgb, int & h, int &s, int & v)const {
	Hsv hsv = get(rgb);
	h = hsv.h();
	s = hsv.s();
	v = hsv.v();
}