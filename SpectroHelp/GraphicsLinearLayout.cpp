//GraphicsLinearLayout.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "GraphicsLinearLayout.h"
#include "BoundingRectLayoutItem.h"

GraphicsLinearLayout::GraphicsLinearLayout(QGraphicsLayoutItem * parent): QGraphicsLinearLayout(parent) {
}
GraphicsLinearLayout::GraphicsLinearLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent):
		QGraphicsLinearLayout(orientation, parent) {
}
GraphicsLinearLayout & GraphicsLinearLayout::operator<<(QGraphicsLayoutItem * item) {
	addItem(item);
	return *this;
}
GraphicsLinearLayout & GraphicsLinearLayout::operator<<(QGraphicsItem * item) {
	auto a = new BoundingRectLayoutItem(item);
	addItem(a);
	return *this;
}
GraphicsLinearLayout & GraphicsLinearLayout::operator<<(QGraphicsWidget * item) {
	addItem(item);
	return *this;
}