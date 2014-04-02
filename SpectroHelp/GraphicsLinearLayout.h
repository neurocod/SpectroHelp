//GraphicsLinearLayout.h by Kostya Kozachuck as neurocod
#pragma once

class GraphicsLinearLayout: public QGraphicsLinearLayout {
	public:
		GraphicsLinearLayout(QGraphicsLayoutItem * parent = 0);
		GraphicsLinearLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent = 0);
		virtual ~GraphicsLinearLayout() {}
		GraphicsLinearLayout & operator<<(QGraphicsLayoutItem * item);
		GraphicsLinearLayout & operator<<(QGraphicsItem * item);
		GraphicsLinearLayout & operator<<(QGraphicsWidget * item);
};