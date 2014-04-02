//ColorToolTipItem.h by Kostya Kozachuck as neurocod
#pragma once
#include "TextWithBorderGraphicsItem.h"
#include "SettingsHash.h"

class ColorToolTipItem: public TextWithBorderGraphicsItem {
	public:
		ColorToolTipItem();
		virtual ~ColorToolTipItem() {}
		void show(QColor clr, QColor clr2);
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
		void calculatePos(QPointF scenePos, QRectF sceneRect);

		void readSettings(SettingsHash & sett);
		void saveSettings(SettingsHash & sett);

		QPointF m_lastColorPos;
		bool m_bShowToolTip;
	protected:
		QColor m_clrDisplayed[2];
		int m_nMaxWidth;
};