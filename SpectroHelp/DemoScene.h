//DemoScene.h by Kostya Kozachuck as neurocod
#pragma once
#include "EGScene.h"

class DemoScene: public QGraphicsScene {
	public:
		static const int s_nKeyMode = 0;
		static const int s_nKeyProfile = 1;

		DemoScene(QObject * parent = 0);
		virtual ~DemoScene() {}
		static QImage hslHsvCircles(int radius, int border=100);
		static QImage hsvCircle(int radius, int border=100);
		static QImage hslCircle(int radius, int border=100);
		static QImage hsvRect(int radius, int border=100);
		static QImage hslRect(int radius, int border=100);
		static QImage hsvScene(int radius, int border=100);
		static QImage hslScene(int radius, int border=100);
	protected:
		void createDemo();
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * ev);
		void highlight(QGraphicsItem*item, bool b);

		QGraphicsItem* m_itemPrevHighlighted;
};