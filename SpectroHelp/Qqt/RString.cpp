//RString.cpp by Kostya Kozachuck as neurocod
//BSD license https://github.com/neurocod/Qqt
#include "pch.h"
#include "RString.h"

QString rtr(const wchar_t* strRu) {
	return QString::fromWCharArray(strRu);
}
QString rtr(const wchar_t* strRu, PCSTR strEn) {
#if defined(RUSSIAN_GUI) || defined(RUSSIAN_GUI_ONLY)
	Q_UNUSED(strEn)
	//static bool b = true;
	//if(b) {
	//	b = false;
	//	auto codec = QTextCodec::codecForName("windows-1251");
	//	ASSERT(codec);
	//	QTextCodec::setCodecForLocale(codec);
	//}
	//return QString::fromLocal8Bit(strRu);
	return QString::fromWCharArray(strRu);
#else
	Q_UNUSED(strRu)
	return QObject::tr(strEn);
#endif
}
RString::RString(PCWSTR ru, PCSTR en): strRu(ru), strEn(en) {
}
RString::RString(const RString & str): strRu(str.strRu), strEn(str.strEn) {
}
RString::operator QString()const {
	return rtr(strRu, strEn);
}
bool RString::operator==(const QString & s0)const {
	return s0==(QString)*this;
}
bool RString::operator!=(const QString & s0)const {
	return s0!=(QString)*this;
}