//RString.h by Kostya Kozachuck as neurocod
//BSD license https://github.com/neurocod/Qqt
#pragma once
#include "ToString.h"

/*rtr/RString - russian + english PCSTR. Which PCSTR to use depends on preprocessor symbols.
I use it because I don't have time to split translation between code and tr files,
and then update translation after each code changes. */
QString rtr(const wchar_t* strRu, PCSTR strEn);
QString rtr(const wchar_t* strRu);

class RString {
	public:
		RString(const wchar_t* ru, PCSTR en);
		RString(const wchar_t* str);
		RString(const RString & str);
		operator QString()const;
		bool operator==(const QString & s0)const;
		bool operator!=(const QString & s0)const;
	protected:
		PCWSTR strRu;
		PCSTR strEn;
};