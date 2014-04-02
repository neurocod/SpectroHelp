//ColorTransformationTable.h by Kostya Kozachuck as neurocod
#pragma once

template<class type>
void translate(type a, type b, type c, type d, type & x) {
	// x is in [a b] -> [c d]
	x = c + (x-a)*(d - c)/(b-a);
}
class ColorTransformationTable {
	public:
		ColorTransformationTable();
		virtual ~ColorTransformationTable() {}
		QRgb get(QRgb col)const;

		static const int InvalidColor = 0xffffff + 1;
	protected:
		static const int SZ = 0xffffff + 1;//colors lay in [0 .. 0xffffff]
		mutable QVector<QRgb> m_v;
};