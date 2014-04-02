//ColorNameList.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorNameList.h"
#include "Qqt/Parser.h"

ColorNameList::ColorNameList() {
	QByteArray arr;
	if(!FileReader::readResourceFile(":/ColorNameEn.txt", arr)) {
		ASSERT(0);
		return;
	}
	QString str = arr;
	parse(str);
}
void ColorNameList::parse(QString str) {
	str.replace("\t", " ");
	str.replace("\r\n", "\n");
	str.replace("\r", "");
	while(str.contains("  "))
		str.replace("  ", " ");
	auto lines = str.split('\n');
	for(QString line: lines) {
		parseLine(line);
	}
}
void ColorNameList::parseLine(QString str) {
	str = str.trimmed();
	if(str.isEmpty())
		return;
	if(str.startsWith("//"))
		return;
	int index = str.indexOf(' ');
	if(-1==index)
		return;
	QString strColor = str.left(index);
	QString strName = str.mid(index);
	strColor = strColor.trimmed();
	strName = strName.trimmed();
	if(!strColor.startsWith("#"))
		return;
	if(strColor.length()!=7) {
		ASSERT(0);
		return;
	}
	strColor.remove(0, 1);
	bool ok;
	int ncolor= strColor.toInt(&ok, 16);
	if(!ok) {
		ASSERT(0);
		return;
	}
	parseWikiName(strName);
	QRgb rcolor = ncolor;
	QColor color = rcolor;
	New<ColorNameItem> item;
	item->m_name = strName;
	item->m_color = color;
	m_items << item;
}
void ColorNameList::parseWikiName(QString & str) {
	Parser::removeBefore("[[", str);
	Parser::removeAfter("]]", str);
	if(str.count("|")>1) {
		ASSERT(0);//edit manually
	}
	Parser::removeBefore("|", str);
	Parser::removeAfter("#", str);
	//Parser::removeAfter(" (color)", str);
}