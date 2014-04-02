//PersistentTimer.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "PersistentTimer.h"

PersistentTimer::PersistentTimer() {
}
void PersistentTimer::setDefaultsNotStart(quint64 intervalMsecs, QDateTime nextTick) {
	m_msecsInterval = intervalMsecs;
	m_timeNextTick = nextTick;
	connect(&m_timer, SIGNAL(timeout()), SLOT(onTimer()));
}
void PersistentTimer::readSettings(SettingsHash & sett) {
	m_msecsInterval = sett.value(m_strSerializationPrefix + "msInterval", m_msecsInterval).toULongLong();
	QDateTime tm = sett.value(m_strSerializationPrefix + "nextTick", m_timeNextTick).toDateTime();
	QString str = tm.toString();
	if(tm.isValid())
		m_timeNextTick = tm;
}
void PersistentTimer::saveSettings(SettingsHash & sett) {
	sett.setValue(m_strSerializationPrefix + "msInterval", m_msecsInterval);
	sett.setValue(m_strSerializationPrefix + "nextTick", m_timeNextTick);
}
void PersistentTimer::start() {
	m_timer.setSingleShot(true);
	m_timer.start(1);
}
void PersistentTimer::onTimer() {
	QDateTime tmNow = QDateTime::currentDateTime();
	if(tmNow.secsTo(m_timeNextTick)<0) {
		while(tmNow.secsTo(m_timeNextTick)<0) {
			m_timeNextTick = m_timeNextTick.addSecs(m_msecsInterval/1000);
		}
		emit timeout();
	}
	//now m_timeNextTick always in future
	int secs = tmNow.secsTo(m_timeNextTick);
	secs = qMin(secs, (int)86400);
	m_timer.start(secs*1000);
}