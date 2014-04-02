//ColorNameList.h by Kostya Kozachuck as neurocod
#pragma once
#include "ColorNameItem.h"

class ColorNameList {
	public:
		ColorNameList();
		virtual ~ColorNameList() {}
		void parse(QString str);
		void parseLine(QString str);
		QList<const ColorNameItem*> items()const { return m_items; }
	protected:
		QList<const ColorNameItem*> m_items;

		void parseWikiName(QString & str);
};