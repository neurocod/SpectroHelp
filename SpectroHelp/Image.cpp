//Image.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "Image.h"

Image::Image() {
}
//static
void Image::addBorder(QImage & img, int border, QColor clr) {
	ASSERT(border>=0);
	if(border<=0)
		return;
	int sx = img.rect().width();
	int sy = img.rect().height();
	QImage img2(sx + 2*border, sy + 2*border, QImage::Format_ARGB32);
	img2.fill(clr);
	for(int y=0; y<sy; y++) {
		for(int x=0; x<sx; x++) {
			auto rgb = img.pixel(x, y);
			img2.setPixel(x + border, y + border, rgb);
		}
	}
	img = img2;
}
//static
QImage Image::concat(const QImage & i0, const QImage & i1, Qt::Orientation or, QColor clrFill) {
	const QSize sz0 = i0.rect().size();
	const QSize sz1 = i1.rect().size();
	if(sz0.isEmpty())
		return i1;
	if(sz1.isEmpty())
		return i0;

	QSize sz;
	QPoint offset1;
	if(Qt::Horizontal==or) {
		sz.rwidth() = sz0.width() + sz1.width();
		sz.rheight() = qMax(sz0.height(), sz1.height());
		offset1 = QPoint(sz0.width(), 0);
	} else {
		sz.rwidth() = qMax(sz0.width(), sz1.width());
		sz.rheight() = sz0.height() + sz1.height();
		offset1 = QPoint(0, sz0.height());
	}
	QImage ret(sz, QImage::Format_ARGB32);
	ret.fill(clrFill);
	for(int y=0; y<sz0.height(); y++) {
		for(int x=0; x<sz0.width(); x++) {
			auto rgb = i0.pixel(x, y);
			ret.setPixel(x, y, rgb);
		}
	}
	for(int y=0; y<sz1.height(); y++) {
		for(int x=0; x<sz1.width(); x++) {
			auto rgb = i1.pixel(x, y);
			ret.setPixel(offset1 + QPoint(x, y), rgb);
		}
	}
	return ret;
}
QIcon Image::replaceAll(const QIcon &ico, QRgb from, QRgb to) {
	QIcon ret;
	for(auto state = QIcon::On; true; state=QIcon::Off) {
		QIcon::Mode modes[] = {QIcon::Normal, QIcon::Disabled, QIcon::Active, QIcon::Selected};
		for(int iMode=0; iMode<_countof(modes); iMode++) {
			auto mode = modes[iMode];
			QList<QSize> sizes = ico.availableSizes(mode, state);
			for(QSize sz: sizes) {
				QPixmap pix = ico.pixmap(sz, mode, state);
				QImage img = pix.toImage();
				replaceAll(img, from, to);
				ret.addPixmap(QPixmap::fromImage(img), mode, state);
			}
		}
		if(state==QIcon::Off)
			break;
	}
	return ret;
}
void Image::replaceAll(QImage & img, QRgb from, QRgb to) {
	QSize sz = img.rect().size();
	for(int y=0; y<sz.height(); y++) {
		for(int x=0; x<sz.width(); x++) {
			auto rgb = img.pixel(x, y);
			if(rgb==from)
				rgb = to;
			img.setPixel(x, y, rgb);
		}
	}
}
QIcon Image::icoFromFolder(QString folder) {
	QIcon ret;
	QDirIterator dir(folder);
	while(dir.hasNext()) {
		QString file = dir.next();
		ret.addFile(file);
	}
	return ret;
}