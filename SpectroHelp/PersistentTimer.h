//PersistentTimer.h by Kostya Kozachuck as neurocod
#pragma once
#include "SettingsHash.h"

//PersistentTimer supports big intervals (say weeks), serialization, and continues to work after deserialization
//Was used for autoupdate checks
class PersistentTimer: public QObject {
	Q_OBJECT
	public:
		PersistentTimer();
		virtual ~PersistentTimer() {}
		QString m_strSerializationPrefix;
		quint64 m_msecsInterval;

		void setDefaultsNotStart(quint64 intervalMsecs, QDateTime nextTick);
		void readSettings(SettingsHash & sett);
		void saveSettings(SettingsHash & sett);
		void start();
	signals:
		void timeout();
	public slots:
		void onTimer();
	protected:
		QDateTime m_timeNextTick;
		QTimer m_timer;
};