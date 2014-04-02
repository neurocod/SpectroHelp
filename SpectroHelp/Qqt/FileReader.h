//FileReader.h by Kostya Kozachuck as neurocod - 21.10.2011 12:26:28
#pragma once
#include "ReadStatus.h"

class FileReader {
	public:
		static bool read(IN const QString & fileName, OUT QByteArray & arrFileData);
		static bool readAscii(IN const QString & fileName, OUT QString & strFileData);
		static bool readUnicode(IN const QString & fileName, OUT QString & strFileData);
		static bool readResourceFile(IN const QString & fileName, OUT QByteArray & arrFileData);
		static void reportError(QFile & file);
		static void reportError(const QString & file, const QString & error);
};