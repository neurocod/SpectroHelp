//Image.h by Kostya Kozachuck as neurocod
#pragma once

class Image {
	public:
		Image();
		virtual ~Image() {}
		static void addBorder(QImage & img, int border, QColor clr = QColor(255,255,255));
		static QImage concat(const QImage & i0, const QImage & i1, Qt::Orientation or = Qt::Vertical, QColor clrFill = QColor(0xffffff));
		static QIcon replaceAll(const QIcon &ico, QRgb from, QRgb to);
		static void replaceAll(QImage & img, QRgb from, QRgb to);
		static QIcon icoFromFolder(QString folder);
};