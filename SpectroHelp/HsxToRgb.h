//HsxToRgb.h by Kostya Kozachuck as neurocod
#pragma once
#include "Vector2d.h"

template<QColor func(int, int, int, int) = QColor::fromHsv>
class HsxToRgb {
	public:
		HsxToRgb() {
			m_v.resize(361, 256*256, InvalidColor);
		}
		virtual ~HsxToRgb() {}
		static const int InvalidColor = 0xffffff + 1;
		QRgb get(int h, int s, int v)const {
			if(h<-1 || s<0 || v<0 || h>359 || s>=256 || v>=256) {
				ASSERT(0);
				return 0;
			}
			int x = h+1;
			int y = (s << 8) | v;
			QRgb & ret = m_v.ref(x, y);
			if(InvalidColor==ret) {
				QColor c = func(h, s, v, 255);
				ret = c.rgb();
			}
			return ret;
		}
	protected:
		static const int SZ = 256*256*361;
		mutable Vector2d<QRgb> m_v;
};