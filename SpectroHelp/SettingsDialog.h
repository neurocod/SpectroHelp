//SettingsDialog.h by Kostya Kozachuck as neurocod
#pragma once

class SettingsDialog: public QDialog {
	public:
		SettingsDialog(QWidget*parent=0);
		virtual ~SettingsDialog() {}
		LineEdit m_editShortcutShow;
		SpinBox m_spinDelayScreenshot;
		//CheckBox m_checkUpdatesAutomatically;
	protected:
		QWidget* createGroupHotkey();
		QWidget* createGroupDelays();
};