//DemoScene.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "DemoScene.h"
#include "DemoVisionDiagram.h"
#include "GraphicsBoxLayout.h"
#include "MainWindow.h"
#include "HsvToRgb.h"
#include "HslToRgb.h"
#include "Image.h"
#include "DemoPixelDiagram.h"

DemoScene::DemoScene(QObject * parent): QGraphicsScene(parent) {
	GraphicsTextItem::s_nDefaultPointSizeIncrement = 3;
	createDemo();
	GraphicsTextItem::s_nDefaultPointSizeIncrement = 0;
}
void DemoScene::createDemo() {
	GraphicsBoxLayout lay(this, Qt::Vertical, 10, 10);
	lay << new GraphicsTextItem(rtr(L"Нажмите на любую группу, чтобы переключиться в нужный режим", 
		"Click on any group to switch to that mode"));
	//lay << new GraphicsTextItem(rtr(L"Цветовой профиль->режим", "Color profile->color mode:"));
	{
		New<DemoPixelDiagram> d(ProfileDemo, ModeOriginal, rtr(L"Оригинальное изображение без изменений", "Original image without changes"));
		d->createArrow(d->r0, d->r1);
		d->createArrow(d->g0, d->g1);
		d->createArrow(d->b0, d->b1);
		lay << d;
	}
	{
		New<DemoPixelDiagram> d(ProfileDemo, ModeR);
		d->createArrow(d->r0, d->r1);
		lay << d;
	}
	{
		New<DemoPixelDiagram> d(ProfileR, ModeCone);
		d->createArrow(d->r0, d->r1);
		d->createArrow(d->r0, d->g1);
		d->createArrow(d->r0, d->b1);
		lay << d;
	}
	{
		New<DemoPixelDiagram> d(ProfileDemo, ModeG);
		d->createArrow(d->g0, d->g1);
		lay << d;
	}
	{
		New<DemoPixelDiagram> d(ProfileG, ModeCone);
		d->createArrow(d->g0, d->r1);
		d->createArrow(d->g0, d->g1);
		d->createArrow(d->g0, d->b1);
		lay << d;
	}
	{
		New<DemoPixelDiagram> d(ProfileDemo, ModeB);
		d->createArrow(d->b0, d->b1);
		lay << d;
	}
	{
		New<DemoPixelDiagram> d(ProfileB, ModeCone);
		d->createArrow(d->b0, d->r1);
		d->createArrow(d->b0, d->g1);
		d->createArrow(d->b0, d->b1);
		lay << d;
	}
	{
		New<DemoVisionDiagram> d;
		lay << d;
	}
	lay.addEnd();
}
void DemoScene::mousePressEvent(QGraphicsSceneMouseEvent * ev) {
	QGraphicsItem *item = itemAt(ev->scenePos(), QTransform());
	if(!item)
		return;
	if(!qgraphicsitem_cast<QGraphicsTextItem*>(item))
		return;
	QVariant v0 = item->data(s_nKeyMode);
	QVariant v1 = item->data(s_nKeyProfile);
	if(v0.type()!=QVariant::Int || v1.type()!=QVariant::Int) {
		return;
	}
	Mode mode = (Mode)v0.toInt();
	ColorProfile cp = (ColorProfile)v1.toInt();
	if(!EGScene::checkRange(mode) || !EGScene::checkRange(cp)) {
		ASSERT(0);
		return;
	}
	MainWindow::instance()->showTab(cp, mode);
}
void DemoScene::highlight(QGraphicsItem*item, bool b) {
	//data.textItem->setBorderWidth(nPenWidth);
}
template<class HsxClass>
QImage DemoScene_hsxCircle(int lightness, int _radius, int border) {
	auto & hsx = HsxClass::instance();
	qreal radius = _radius;
	int diameter = 2*_radius;
	QImage img(diameter, diameter, QImage::Format_ARGB32);
	QPointF center(radius, radius);
	for(int y=0; y<diameter; y++) {
		for(int x=0; x<diameter; x++) {
			qreal dx = x - center.x();
			qreal dy = y - center.y();
			dy = -dy;
			qreal len = qSqrt(dx*dx + dy*dy);
			if(len>=radius) {
				img.setPixel(x, y, qRgb(255,255,255));
				continue;
			}
			qreal sat = 255*len/radius;
			qreal angle = qAtan2(dx, dy);
			int hue = angle * 360.0/(2*PI);
			hue -= 90;
			if(hue<0)
				hue += 360;
			if(hue>360)
				hue-=360;
			QRgb rgb = hsx.get(hue, sat, lightness);
			img.setPixel(x, y, rgb);
		}
	}
	Image::addBorder(img, border);
	return img;
}
//static
QImage DemoScene::hsvCircle(int radius, int border) {
	return DemoScene_hsxCircle<HsvToRgb>(255, radius, border);
}
//static
QImage DemoScene::hslCircle(int radius, int border) {
	return DemoScene_hsxCircle<HslToRgb>(128, radius, border);
}
template<class HsxClass>
QImage DemoScene_hsxRect(int radius, int border) {
	auto & hsx = HsxClass::instance();
	int width = radius*2;
	int height = radius;
	QImage img(width, height, QImage::Format_ARGB32);
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			qreal hue = x;
			hue /= width;
			hue *= 360;
			qreal v = y;
			v /= height;
			v *= 255;

			int sat = 255;
			QRgb rgb = hsx.get(hue, sat, v);
			img.setPixel(x, y, rgb);
		}
	}
	Image::addBorder(img, border);
	return img;
}
//static
QImage DemoScene::hsvRect(int radius, int border) {
	return DemoScene_hsxRect<HsvToRgb>(radius, border);
}
//static
QImage DemoScene::hslRect(int radius, int border) {
	return DemoScene_hsxRect<HslToRgb>(radius, border);
}
//static
QImage DemoScene::hsvScene(int radius, int border) {
	QImage i0 = hsvCircle(radius, border/2);
	QImage i1 = hsvRect(radius, border/2);
	return Image::concat(i0, i1, Qt::Vertical);
}
//static
QImage DemoScene::hslScene(int radius, int border) {
	QImage i0 = hslCircle(radius, border/2);
	QImage i1 = hslRect(radius, border/2);
	return Image::concat(i0, i1, Qt::Vertical);
}
//static
QImage DemoScene::hslHsvCircles(int radius, int border) {
	auto hsl = hslScene(radius, border);
	auto hsv = hsvScene(radius, border);
	//Image::addBorder(i, border/2);
	return Image::concat(hsl, hsv, Qt::Horizontal);
}