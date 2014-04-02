//SettingsHash.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "SettingsHash.h"

SettingsHash::SettingsHash() {
}
SettingsHash::SettingsHash(QVariant v) {
	QVariantHash*pThis = this;
	*pThis = v.toHash();
}
SettingsHash::~SettingsHash() {
	if(m_bUseSettingsFile)
		synchWithQSettings(true);
}
SettingsHash SettingsHash::sub(QString name) {
	return SettingsHash(value(name));
}
void SettingsHash::setSub(QString name, const SettingsHash & sub) {
	insert(name, QVariant(sub));
}
void SettingsHash::useSettingsFile() {
	m_bUseSettingsFile = true;
	synchWithQSettings(false);
}
void SettingsHash::synchWithQSettings(bool saveToQSettings) {
	QSettings sett;
	//simple, but space-inefficient (for INI files) version:
	if(saveToQSettings)
		sett.setValue("SettingsHash", QVariant(*this));
	else
		*static_cast<QVariantHash*>(this) = sett.value("SettingsHash").toHash();
	//if(saveToQSettings) {
	//	auto arr = toByteArray();
	//	sett.setValue("SettingsHash", arr);
	//} else {
	//	auto arr = sett.value("SettingsHash").toByteArray();
	//	fromByteArray(arr);
	//}
}
/*
to save space twixe in INI file format (and write ff instead of \xff ):
QByteArray SettingsHash::toByteArray()const {
	QByteArray ret;
	{
		QDataStream stream(&ret, QIODevice::WriteOnly);
		stream << QVariant(*this);
	}
	//TODO: arr -> hex encode
	return ret;
}
void SettingsHash::fromByteArray(QByteArray arr) {
	//TODO: arr -> hex decode
	QDataStream stream(&arr, QIODevice::ReadOnly);
	QVariant v;
	stream >> v;
	*static_cast<QVariantHash*>(this) = v.toHash();
}*/