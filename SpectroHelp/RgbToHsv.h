//RgbToHsv.h by Kostya Kozachuck as neurocod
#pragma once

class Hsv {
	protected:
		qint16 m_h;
		qint16 m_sv;
	public:
		Hsv(): m_h(0), m_sv(0) {}
		Hsv(int h, int s, int v) {
			m_h = h;
			m_sv = (s<<8) | (v);
		}
		Hsv(QRgb col) {
			int h,s,v;
			QColor(col).getHsv(&h, &s, &v);
			*this = Hsv(h,s,v);
		}
		int h()const { return m_h; }
		int s()const { return (m_sv>>8) & 0xff; }
		int v()const { return (m_sv) & 0xff; }
		bool operator==(Hsv other)const { return m_h==other.m_h && m_sv==other.m_sv; }
};

class RgbToHsv {
	public:
		virtual ~RgbToHsv() {}
		Hsv get(QRgb rgb)const;
		void get(QRgb rgb, int & h, int &s, int & v)const;
		static const Hsv InvalidColor;
		static const RgbToHsv& instance();
	protected:
		RgbToHsv();
		static const int SZ = 256*256*256;
		mutable QVector<Hsv> m_v;
};