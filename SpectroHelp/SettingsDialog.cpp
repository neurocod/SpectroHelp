//SettingsDialog.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget*parent): QDialog(parent) {
	setWindowTitle(rtr(L"Настройки", "Settings"));

	VBoxLayout lay(this);
	//m_checkUpdatesAutomatically.text = rtr(L"Проверять обновления автоматически каждые 3 дня", "Check for updates automatically each 3 days");
	//lay << m_checkUpdatesAutomatically;

	lay << createGroupHotkey();
	lay << createGroupDelays();

	New<QDialogButtonBox> buttonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	lay << buttonBox;
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
QWidget* SettingsDialog::createGroupHotkey() {
	New<QGroupBox> box(rtr(L"Глобальное клавиатурное сокращение", "Global hotkey"));
	VBoxLayout lay(box);
	{
		HBoxLayout00 lay2(lay);

		lay2 << rtr(L"Глобальное клавиатурное сокращения для создания снимка экрана и показа окна: ",
			"Global hotkey to take screenshot and show window, even if program is minimized: ");
		lay2 << m_editShortcutShow;
		lay2 << rtr(L"По умолчанию: Ctrl+F12", " Default: Ctrl+F12");
	}
	lay << rtr(L"При задании клавиатурных сокращений, модификаторы типа Ctrl/Alt/Shift/Left/Right вводите в текстовом виде, "
		L"используя '+' в качестве разделителя, как в Ctrl+A.\n"
		L"Кнопки Enter и Return отличаются: Enter обычно расположен на цифровой клавиатуре, Return - в центре клавиатуры.",

		"When entering shortcuts, modifiers like Ctrl/Alt/Shift/Left/Right must be entered manually in text form, "
		"delimited with '+' sign, like Ctrl+A.\n"
		"'Enter' and 'Return' keys differ: Enter usually is in the numeric keyboard on the rigth, Return - in the middle of keyboard.");
	return box;
}
//Go to Start -> Control Panel -> Clock, Language, and Region / Change the display language
QWidget* SettingsDialog::createGroupDelays() {
	New<QGroupBox> box(rtr(L"Задержка перед снимком экрана", "Delay before screenshot"));
	VBoxLayout lay(box);
	{
		HBoxLayout00 lay2(lay);

		lay2 << rtr(L"Задержка в миллисекундах (1/1000 секунды): ", "Delay in milliseconds (1/1000 of second): ");
		lay2 << m_spinDelayScreenshot;
		m_spinDelayScreenshot.suffix = rtr(L" мс", " ms");
		m_spinDelayScreenshot.minimum = 1;
		m_spinDelayScreenshot.maximum = 1000;
		lay2 << rtr(L"Рекомендуемые значения: 500 мс при включенной анимации ОС (см ниже), 1 мс при отключенной",
			"Recommended values: 500 ms when OS animation is switched on (read below), 1 ms when animation is off");
	}
	auto label = new QLabel(
	rtr(L"Если вызвать создание снимка экрана при видимом окне SpectroHelp, то SpectroHelp сначала свернет окно,"
		L" подождет указанный интервал времени, затем сделает снимок экрана. Если сделать снимок экрана моментально,"
		L" то окно SpectroHelp может не успеть свернутся, и SpectroHelp снимет сама себя. В некоторых операционных системах (например WIndows XP) анимация может быть выключена изначально."
		L" Чтобы ускорить сворачивание окон, можно отключить анимацию сворачивания в настройках Windows.<br/>"
		L"<b>Для Windows 7: нажмите Пуск->свойства 'Мой компьютер'->Дополнительные параметры системы->вкладка 'Дополнительно'"
		L" -> вкладка 'Визуальные эффекты' -> выбрать 'Особые эффекты' -> снять крыжик с пункта 'Анимация окон при свертывании и развертывании'->Нажмите OK</b><br>"
		L"После этого можно установить значение задержки в 1 мс",

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