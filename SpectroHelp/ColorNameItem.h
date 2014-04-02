//ColorNameItem.h by Kostya Kozachuck as neurocod
#pragma once

class ColorNameItem {
	public:
		QString m_name;
		QColor m_color;
		QString empty()const { return QString(); };
		QString h()const;
		QString s()const;
		QString v()const;

		QString r()const {return percentage(m_color.red()); }
		QString g()const {return percentage(m_color.green()); }
		QString b()const {return percentage(m_color.blue()); }
		static QString percentage(int n255);
};