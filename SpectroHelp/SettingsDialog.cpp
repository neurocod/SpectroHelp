//SettingsDialog.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget*parent): QDialog(parent) {
	setWindowTitle(rtr(L"���������", "Settings"));

	VBoxLayout lay(this);
	//m_checkUpdatesAutomatically.text = rtr(L"��������� ���������� ������������� ������ 3 ���", "Check for updates automatically each 3 days");
	//lay << m_checkUpdatesAutomatically;

	lay << createGroupHotkey();
	lay << createGroupDelays();

	New<QDialogButtonBox> buttonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	lay << buttonBox;
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
QWidget* SettingsDialog::createGroupHotkey() {
	New<QGroupBox> box(rtr(L"���������� ������������ ����������", "Global hotkey"));
	VBoxLayout lay(box);
	{
		HBoxLayout00 lay2(lay);

		lay2 << rtr(L"���������� ������������ ���������� ��� �������� ������ ������ � ������ ����: ",
			"Global hotkey to take screenshot and show window, even if program is minimized: ");
		lay2 << m_editShortcutShow;
		lay2 << rtr(L"�� ���������: Ctrl+F12", " Default: Ctrl+F12");
	}
	lay << rtr(L"��� ������� ������������ ����������, ������������ ���� Ctrl/Alt/Shift/Left/Right ������� � ��������� ����, "
		L"��������� '+' � �������� �����������, ��� � Ctrl+A.\n"
		L"������ Enter � Return ����������: Enter ������ ���������� �� �������� ����������, Return - � ������ ����������.",

		"When entering shortcuts, modifiers like Ctrl/Alt/Shift/Left/Right must be entered manually in text form, "
		"delimited with '+' sign, like Ctrl+A.\n"
		"'Enter' and 'Return' keys differ: Enter usually is in the numeric keyboard on the rigth, Return - in the middle of keyboard.");
	return box;
}
//Go to Start -> Control Panel -> Clock, Language, and Region / Change the display language
QWidget* SettingsDialog::createGroupDelays() {
	New<QGroupBox> box(rtr(L"�������� ����� ������� ������", "Delay before screenshot"));
	VBoxLayout lay(box);
	{
		HBoxLayout00 lay2(lay);

		lay2 << rtr(L"�������� � ������������� (1/1000 �������): ", "Delay in milliseconds (1/1000 of second): ");
		lay2 << m_spinDelayScreenshot;
		m_spinDelayScreenshot.suffix = rtr(L" ��", " ms");
		m_spinDelayScreenshot.minimum = 1;
		m_spinDelayScreenshot.maximum = 1000;
		lay2 << rtr(L"������������� ��������: 500 �� ��� ���������� �������� �� (�� ����), 1 �� ��� �����������",
			"Recommended values: 500 ms when OS animation is switched on (read below), 1 ms when animation is off");
	}
	auto label = new QLabel(
	rtr(L"���� ������� �������� ������ ������ ��� ������� ���� SpectroHelp, �� SpectroHelp ������� ������� ����,"
		L" �������� ��������� �������� �������, ����� ������� ������ ������. ���� ������� ������ ������ �����������,"
		L" �� ���� SpectroHelp ����� �� ������ ���������, � SpectroHelp ������ ���� ����. � ��������� ������������ �������� (�������� WIndows XP) �������� ����� ���� ��������� ����������."
		L" ����� �������� ������������ ����, ����� ��������� �������� ������������ � ���������� Windows.<br/>"
		L"<b>��� Windows 7: ������� ����->�������� '��� ���������'->�������������� ��������� �������->������� '�������������'"
		L" -> ������� '���������� �������' -> ������� '������ �������' -> ����� ������ � ������ '�������� ���� ��� ����������� � �������������'->������� OK</b><br>"
		L"����� ����� ����� ���������� �������� �������� � 1 ��",

		"If 'Take screenshot' command is invoked when SpectroHelp window is visible, SpectroHelp will be minimized first,"
		" then it will wait for specified period of time, then it will take screenshot. If SpectroHelp takes screenshot immediately,"
		" there is no time to minimize window, and SpectroHelp will shoot oneself. Some operating systems like Windows XP may not have animation mode."
		" To speed up windows minimization time and thus minimize delay before screenshot, switch off windows animation in OS settings:<br/>"
		"<b>In WIndows 7, press Start menu->'Computer' menu properties->'Advanced system settings' link->'Advanced' tab"
		" -> 'Performace' section 'settings' button -> 'Visual effects' tab -> choose 'Custom' -> check off 'Animate windows when minimizing and maximizing' -> Press OK</b><br>"
		"After that you can set delay here to 1 ms"));
	label->setWordWrap(true);
	lay << label;
	return box;
}