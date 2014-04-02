//GraphicsBoxLayout.h by Kostya Kozachuck as neurocod
#pragma once

class GraphicsBoxLayout {
	public:
		GraphicsBoxLayout(QGraphicsScene*scene, Qt::Orientation or = Qt::Vertical, int margin=50, int spacing = 50);
		virtual ~GraphicsBoxLayout() {}
		GraphicsBoxLayout & operator<<(QGraphicsItem * item);
		void addItem(QGraphicsItem * item);
		qreal maxSizeRecursive(QGraphicsItem * item)const;
		qreal size(QGraphicsItem * item)const;
		void setSpacing(qreal s);
		void setMargin(qreal m) { m_margin = m; }
		void addEnd();
	protected:
		QGraphicsScene*m_scene;
		Qt::Orientation m_orientation;
		qreal m_margin;
		qreal m_lastPos;
		qreal m_spacing;
		int m_nItemsAdded;
};