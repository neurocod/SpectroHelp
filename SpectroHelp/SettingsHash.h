//SettingsHash.h by Kostya Kozachuck as neurocod
#pragma once
#include "InitializedPod.h"

/*SettingsHash: works like QSettings, but can store other SettingsHash and form trees,
so it's possible to use same code for top-level nesting as in QSettings and more nested levels */
class SettingsHash: public QVariantHash {
	public:
		SettingsHash();
		SettingsHash(QVariant v);
		virtual ~SettingsHash();
		void useSettingsFile();

		SettingsHash sub(QString name);
		void setSub(QString name, const SettingsHash & sub);

		void setValue(const QString & name, const QVariant & v) { insert(name, v); }
	protected:
		InitializedPod<bool, false> m_bUseSettingsFile;
		void synchWithQSettings(bool saveToQSettings);
};