//GraphicsTextItem.h by Kostya Kozachuck as neurocod
#pragma once

class GraphicsTextItem: public QGraphicsTextItem {
	public:
		GraphicsTextItem(QGraphicsItem * parent = 0);
		GraphicsTextItem(const QString & text, QGraphicsItem * parent = 0);
		virtual ~GraphicsTextItem() {}
		static qreal s_nDefaultPointSizeIncrement;
	protected:
	private:
		void _init();
};