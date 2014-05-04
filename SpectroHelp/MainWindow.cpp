//MainWindow.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "MainWindow.h"
#include "EGView.h"
#include "ColorNameTableWidget.h"
#include "Qxt/gui/qxtglobalshortcut.h"
#include "DemoScene.h"
#include "SettingsDialog.h"
#include "Image.h"
//#include "NewVersionChecker.h"

MainWindow* MainWindow::s_instance = 0;
MainWindow* MainWindow::instance() {
	return s_instance;
}
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags),
		m_demoScene(this) {
	ASSERT(!s_instance);
	s_instance = this;
	m_bQuit = false;
	m_bFirstLaunch = true;
	m_bFirstClose = false;
	m_stateBeforeMinimize = 0;
	m_strProgramName = tr("SpectroHelp");
	m_strProgramVersion = "1.0.9";
	m_strProgramVersionPrev = "1.0.8";
	m_nScreenshotDelay = 500;
	setWindowTitle(m_strProgramName + " " + m_strProgramVersion);
	m_spectroToolbars = new SpectroToolbarsManager(this);
	//m_newVersionChecker = new NewVersionChecker(m_strProgramVersion, m_strProgramVersionPrev);
	{
		QGraphicsView* viewDemo = new QGraphicsView(m_demoScene);
		viewDemo->centerOn(0, 0);
		m_dockDemo = new DockWidget(rtr(L"Схема восприятия цвета", "Color sensation scheme"));
		connect(m_dockDemo, SIGNAL(hiddenByClose()), SLOT(updateVisibilityButtons()) );
		m_dockDemo->setWidget(viewDemo);
		addDockWidget(Qt::LeftDockWidgetArea, m_dockDemo);
	}
	{
		m_scene = new EGScene(this);
		m_spectroToolbars->set(m_scene);
		m_view = new EGView(m_scene, m_spectroToolbars, this);
	}
	{
		New<ColorNameTableWidget> wid(this);
		m_dockNames = new DockWidget(wid->windowTitle());
		connect(m_dockNames, SIGNAL(hiddenByClose()), SLOT(updateVisibilityButtons()) );
		m_dockNames->setWidget(wid);
		connect(wid->view(), SIGNAL(colorSelected(QColor)), m_scene, SLOT(selectColor(QColor)) );
		addDockWidget(Qt::LeftDockWidgetArea, m_dockNames);
	}
	{
		New<QShortcut> quit(QKeySequence("Ctrl+Q"), this);
		connect(quit, SIGNAL(activated()), this, SLOT(quit()));

		m_shortcutShow = new QxtGlobalShortcut(this);
		connect(m_shortcutShow, SIGNAL(activated()), this, SLOT(shootAgain()));
		m_shortcutShow->setShortcut(QKeySequence("Ctrl+F12"));
	}
	createToolbars();
	setCentralWidget(m_view);
	createTrayIcon();
	readSettings();
	QTimer::singleShot(1, this, SLOT(messageQueueStarted()));
}
MainWindow::~MainWindow() {
	ASSERT(s_instance==this);
	s_instance = 0;
}
void MainWindow::createToolbars() {
	{
		ToolBar toolbar(this);
		toolbar.objectName = "toolbar.file";
		addToolBar(toolbar);

		Action actShoot(this, QIcon(":/images/camera24.png"));
		m_actShoot = actShoot;
		actShoot.connectClicks(this, SLOT(shootAgain()));
		toolbar << actShoot;

		addVideoMenu(toolbar);
		addOpenMenu(toolbar);
		addSaveMenu(toolbar);
		
		toolbar->insertSeparator(0);
		toolbar << m_spectroToolbars->createActionTooltipVisibility();

		Action actInfo(this,
			rtr(L"Показать/скрыть описание восприятия цвета", "Show/hide color sensation scheme"),
			QIcon(":/images/forge/24/sitemap.png"));
		actInfo.checkable = true;
		m_actShowColorScheme = actInfo;
		actInfo.addShortcutToTooltip();
		actInfo.connectClicks(this, SLOT(showDemo()));
		toolbar << actInfo;

		Action actNames(this,
			rtr(L"Показать/скрыть таблицу именованных цветов", "Show/hide color names table"),
			QIcon(":/images/table.png"));
		actNames.checkable = true;
		m_actShowNames = actNames;
		actNames.connectClicks(this, SLOT(showNames()));
		actNames.addShortcutToTooltip();
		toolbar << actNames;

		addDemoMenu(toolbar);

		Action actSettings(this,
			rtr(L"Показать окно с настройками", "Show settings editor"),
			QIcon(":/images/gear24.png"));
		actSettings.connectClicks(this, SLOT(showSettings()));
		actSettings.addShortcutToTooltip();
		toolbar << actSettings;

		addHelpMenu(toolbar);
	}
	m_spectroToolbars->createToolbars();
}
//virtual
void MainWindow::keyPressEvent(QKeyEvent * ev) {
	quint32 vkey = ev->nativeVirtualKey();
	if(Qt::Key_Escape==ev->key()) {
		ev->accept();
		hide();
		return;
	}
	//change tab?
	//nativeVirtualKey == Qt::Key
	static QList<quint32> arrTabs;//qwertyuiop[]
	if(arrTabs.isEmpty()) {
		arrTabs
			<< Qt::Key_Q
			<< Qt::Key_W
			<< Qt::Key_E
			<< Qt::Key_R
			<< Qt::Key_T
			<< Qt::Key_Y
			<< Qt::Key_U
			<< Qt::Key_I
			<< Qt::Key_O
			<< Qt::Key_P
			<< Qt::Key_BracketLeft
			<< Qt::Key_BracketRight;
	};
	QString text = ev->text();
	//qDebug() << "keys: " << text << ev->nativeScanCode() << ev->nativeVirtualKey();
	int index = arrTabs.indexOf(vkey);
	if(-1!=index && index<m_spectroToolbars->tabModes()->count()) {
		m_spectroToolbars->tabModes()->setCurrentIndex(index);
		ev->accept();
		return;
	}
	{ //set slider pos?
		static const QString keys = "1234567890";
		int index = keys.indexOf(vkey);
		if(-1!=index) {
			setSliderRelative(m_spectroToolbars->sliderColorRange(), index);
			ev->accept();
			return;
		}
	}
	{ //set profile?
		static const QString keys = "asdf";
		//TODO if(maybeSetTabIndexByChar(ev, keys, m_spectroToolbars->boxProfiles()))
			//return;
	}
	{ //set profile?
		static const QString keys = "zxcv";
		if(maybeSetTabIndexByChar(ev, keys, m_spectroToolbars->tabTools()))
			return;
	}
}
bool MainWindow::maybeSetTabIndexByChar(QKeyEvent * ev, const QString & keys, QTabBar*tab) {
	int index = keys.indexOf(ev->text());
	if(-1!=index && !ev->text().isEmpty()) {
		tab->setCurrentIndex(index);
		ev->accept();
		return true;
	}
	return false;
}
void MainWindow::showHelpTab() {
}
void MainWindow::showTestTab() {
}
void MainWindow::createTrayIcon() {
	QIcon ico = Image::icoFromFolder(":/images/ico/");
	setWindowIcon(ico);
	New<QSystemTrayIcon> icon(ico, this);
	icon->setToolTip(windowTitle());
	connect(icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(launch(QSystemTrayIcon::ActivationReason)));
	{
		Action actShow(this, rtr(L"Показать/скрыть окно", "&Show/Hide window"));
		actShow.connectClicks(this, SLOT(toggleVisibility()), Qt::QueuedConnection);

		Action actShoot(this, rtr(L"Снимок экрана", "S&creenshot"), QIcon(":/images/camera24.png"));
		actShoot.connectClicks(this, SLOT(shootAgain()), Qt::QueuedConnection);

		Action quitAction(this, rtr(L"Выход", "&Quit"), QIcon(":/qt-project.org/styles/commonstyle/images/stop-24.png"));
		connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

		New<QMenu> trayIconMenu(this);
		trayIconMenu->addAction(actShow);
		trayIconMenu->addAction(actShoot);
		trayIconMenu->addAction(quitAction);
		icon->setContextMenu(trayIconMenu);
	}
	icon->show();
}
void MainWindow::launch(QSystemTrayIcon::ActivationReason reason) {
	switch(reason) {
	case QSystemTrayIcon::Context:
		/*QSystemTrayIcon::Unknown
		QSystemTrayIcon::DoubleClick
		QSystemTrayIcon::Trigger
		QSystemTrayIcon::MiddleClick*/
		break;
	default:
		//m_timer.singleShot(1000, this, SLOT(launch()));
		//toggleVisibility();
		shootAgain();
	}
}
void MainWindow::closeEvent(QCloseEvent * ev) {
	if(m_bFirstClose) {
		m_bFirstClose = false;
		msgBox(
			rtr(L"Чтобы свернуть окно вместо закрытия, используйте клавишу 'Esc'",
				"To minimize window to tray instead of closing it, use the 'Esc' key."));
	}
	//if(m_bQuit) {
		saveSettings();
		__super::closeEvent(ev);
	/*} else {
		ev->ignore();
		hide();
	}*/
}
void MainWindow::changeEvent(QEvent * ev) {
	if(ev->type()==QEvent::WindowStateChange) {
		QWindowStateChangeEvent* p = static_cast<QWindowStateChangeEvent*>(ev);
		auto snew = windowState();
		if(snew & Qt::WindowMinimized) {
			m_stateBeforeMinimize = p->oldState();;
		}
	}
	__super::changeEvent(ev);
}
QTime s_time;
void MainWindow::shootAgain() {
	if(isMinimized() || !isVisible()) {
		continueShoot();
	} else {
		hide();
		/*QCoreApplication::processEvents();
		QCoreApplication::processEvents();*/
		QTimer::singleShot(m_nScreenshotDelay, this, SLOT(continueShoot()));
	}
}
void MainWindow::continueShoot() {
	m_scene->shootScreen();
	toggleVisibility();
}
bool EMainWindow_raise(WId wid);
void MainWindow::toggleVisibility() {
	if(s_time.elapsed()<500){QApplication::beep();} s_time = QTime::currentTime();
	if(isHidden()) {
		show();
		activateWindow();
	} else if(isMinimized()) {
		if(m_stateBeforeMinimize & Qt::WindowMaximized)
			showMaximized();
		else
			showNormal();
		activateWindow();
	} else {
		//Visible (possibly under other windows)
#ifdef WIN32
	   if(EMainWindow_raise(internalWinId()))
		   hide();
	   else
		   activateWindow();
#else
		raise();
		showNormal();
		activateWindow();
#endif
	}
}
//static
void MainWindow::setSliderRelative(QSlider*s, int n/*0 to 9*/) {
	if(n<0) n = 0;
	if(n>9) n = 9;
	int nmin = s->minimum();
	int nmax = s->maximum();
	int dif = nmax - nmin;
	int result = nmin + (n * dif)/10;
	s->setValue(result);
}
void MainWindow::showDemo() {
	m_dockDemo->setVisible(!m_dockDemo->isVisible());
}
void MainWindow::showNames() {
	m_dockNames->setVisible(!m_dockNames->isVisible());
}
void MainWindow::showHelp() {
	QDesktopServices::openUrl(QUrl("https://sourceforge.net/p/spectrohelp/wiki/Home/"));
}
void MainWindow::showSettings() {
	SettingsDialog dlg(this);
	dlg.m_editShortcutShow->setText(m_shortcutShow->shortcut().toString());
	//dlg.m_checkUpdatesAutomatically->setChecked(m_newVersionChecker->m_bCheckAutomatically);
	dlg.m_spinDelayScreenshot->setValue(m_nScreenshotDelay);
	if(QDialog::Accepted==dlg.exec()) {
		m_shortcutShow->setShortcut( dlg.m_editShortcutShow->text() );
		updateShortcutTooltips();
		//{
		//	bool old = m_newVersionChecker->m_bCheckAutomatically;
		//	m_newVersionChecker->m_bCheckAutomatically = dlg.m_checkUpdatesAutomatically->isChecked();
		//	if(!old && m_newVersionChecker->m_bCheckAutomatically)
		//		m_newVersionChecker->checkByTimer();
		//}
		m_nScreenshotDelay = dlg.m_spinDelayScreenshot->value();
		saveSettings();
	}
}
void MainWindow::showAbout() {
	QString str = m_strProgramName +
		rtr(L" версии %1<br>автор - <a href=\"%2\">Константин Козачук</a>, (C) 2011-2012.",
		" version %1 by <a href=\"%2\">Konstantine Kozachuck</a>, (C) 2011-2012.")
		.arg(m_strProgramVersion)
		.arg(feedEmailUrl())
		+ rtr(L"<br>\nНекоторые иконки от <a href=\"http://p.yusukekamiyamane.com/\">Yusuke Kamiyamane</a> и <a href=\"http://turbomilk.com/\">Turbomilk</a>.",
			"<br>\nSome icons by <a href=\"http://p.yusukekamiyamane.com/\">Yusuke Kamiyamane</a> and <a href=\"http://turbomilk.com/\">Turbomilk</a>.");
	QMessageBox::information(this, rtr(L"О программе", "About ") + m_strProgramName, str);
}
void MainWindow::startVideo(Qt::Alignment side) {
	switch(side) {
	case Qt::AlignRight:
	case Qt::AlignLeft:
	case Qt::AlignTop:
	case Qt::AlignBottom:
		break;
	default:
		ASSERT(0);
		side = Qt::AlignRight;
	}
	QDesktopWidget* dw = QApplication::desktop();
	QRect rc = dw->screen()->rect();
	if(rc.isEmpty()) {
		ASSERT(0);
		m_scene->toggleAnimation();
		return;
	}
	/*const int border = 4;
	if(rc.width()>border)
		rc.adjust(border, 0, -border, 0);*/

	int xScroll = 0;
	int yScroll = 0;
	int w2 = rc.width()/2;
	int h2 = rc.height()/2;
	switch(side) {
	case Qt::AlignRight:
		rc.setLeft(w2);
		break;
	case Qt::AlignLeft:
		rc.setRight(w2);
		xScroll = w2;
		break;
	case Qt::AlignTop:
		rc.setBottom(h2);
		yScroll = h2;
		break;
	case Qt::AlignBottom:
		rc.setTop(h2);
		break;
	default:
		ASSERT(0);
		return;
	}
	showNormal();
	resize(rc.size());
	move(rc.topLeft());
	m_view->verticalScrollBar()->setValue(yScroll);
	m_view->horizontalScrollBar()->setValue(xScroll);
	m_scene->setInAnimationMode(true);
	m_dockDemo->hide();
	m_dockNames->hide();
	updateVisibilityButtons();
}
void MainWindow::videoR() {
	startVideo(Qt::AlignRight);
}
void MainWindow::videoL() {
	startVideo(Qt::AlignLeft);
}
void MainWindow::videoT() {
	startVideo(Qt::AlignTop);
}
void MainWindow::videoB() {
	startVideo(Qt::AlignBottom);
}
void MainWindow::startVideo() {
	m_scene->setInAnimationMode(!m_scene->inAnimationMode());
	//TODO: use last position
}
void MainWindow::addVideoMenu(ToolBar & toolbar) {
	MenuButton btnMenu(this);
	{
		Action actFilm(this, QIcon(":/images/forge/24/film.png"),
			rtr(L"Начать/остановить захват экрана", "Start/stop video screen capture"),
			rtr(L"Непрерывно захватывать изображение всего экрана, чтобы было видно изменения в реальном времени.\n"
				L"Переместите окно так, чтобы оно занимало только половину экрана вверху/внизу/слева/справа, и прокрутите полосу прокрутки, чтобы видеть происходящее на оставшейся части экрана.\n"
				L"Эта функция доступна только в зарегистрированной версии",

				"Start video screen capture, allowing to see changes at real time.\n"
				"Resize and move window to the left/right/top/bottom of the screen, to see both source image and transformation result.\n"
				"This function is available only in registered version"));
		actFilm.shortcutContext = Qt::ApplicationShortcut;
		actFilm.checkable = true;
		actFilm.addShortcutToTooltip();
		actFilm.connectClicks(this, SLOT(startVideo()));
		m_actShowAnimation = actFilm;
		connect(m_scene, SIGNAL(animationModeChanged(bool)), m_actShowAnimation, SLOT(setChecked(bool)));
		btnMenu << actFilm;

		Action actR(this, rtr(L"Начать и поместить справа", "Start at right side"),
			QIcon(":/qt-project.org/styles/commonstyle/images/right-16.png"));
		actR.connectClicks(this, SLOT(videoR()));
		btnMenu << actR;

		Action actL(this, rtr(L"Начать и поместить слева", "Start at left side"),
			QIcon(":/qt-project.org/styles/commonstyle/images/left-16.png"));
		actL.connectClicks(this, SLOT(videoL()));
		btnMenu << actL;

		Action actT(this, rtr(L"Начать и поместить вверх", "Start at top side"),
			QIcon(":/qt-project.org/styles/commonstyle/images/up-16.png"));
		actT.connectClicks(this, SLOT(videoT()));

		Action actB(this, rtr(L"Начать и поместить вниз", "Start at bottom side"),
			QIcon(":/qt-project.org/styles/commonstyle/images/down-16.png"));
		actB.connectClicks(this, SLOT(videoB()));
		btnMenu << actB;

		btnMenu << actT;
	}
	toolbar << btnMenu;
}
void MainWindow::addOpenMenu(ToolBar & toolbar) {
	MenuButton btnMenu(this);
	{
		Action actOpenFile(this,
			QIcon(":/images/forge/24/folder-horizontal-open.png"),
			rtr(L"Открыть файл", "Open file"));
		actOpenFile = QKeySequence("Ctrl+O");
		actOpenFile.shortcutContext = Qt::ApplicationShortcut;
		actOpenFile.addShortcutToTooltip();
		actOpenFile.connectClicks(m_scene, SLOT(loadFileDialog()));
		btnMenu << actOpenFile;

		Action actOpenH(this, rtr(L"Добавить изображение справа", "Add image to the right"));
		actOpenH.connectClicks(m_scene, SLOT(loadFileDialogH()));
		btnMenu << actOpenH;

		Action actOpenV(this, rtr(L"Добавить изображение вниз", "Add image to the bottom"));
		actOpenV.connectClicks(m_scene, SLOT(loadFileDialogV()));
		btnMenu << actOpenV;

		Action a(this, rtr(L"Вставить из кармана", "Load image from clipboard"), QKeySequence("Ctrl+V"));
		a.connectClicks(m_scene, SLOT(loadFromClipboard()));
		btnMenu << a;
	}
	toolbar << btnMenu;
}
void MainWindow::addDemoMenu(ToolBar & toolbar) {
	MenuButton btnMenu(this);
	{
		Action act2(this,
			rtr(L"HSL/HSV ", "HSL/HSV Demo"),
			QIcon(":/images/forge/24/color.png"));
		act2.connectClicks(this, SLOT(showHslHsv()));
		btnMenu << act2;

		Action actHsv(this, rtr(L"HSV", "HSV Demo"));
		actHsv.connectClicks(this, SLOT(showHsv()));
		btnMenu << actHsv;

		Action actHsl(this, rtr(L"HSL", "HSL Demo"));
		actHsl.connectClicks(this, SLOT(showHsl()));
		btnMenu << actHsl;

		Action actDemo(this, rtr(L"Демонстрация режимов", "Modes demonstration"));
		actDemo.connectClicks(m_scene, SLOT(generateDemo()));
		btnMenu << actDemo;
	}
	btnMenu->setToolTip(rtr(L"Примеры для изучения цветовых преобразований",
			"Color examples to learn how different transormations work"));
	toolbar << btnMenu;
}
void MainWindow::addSaveMenu(ToolBar & toolbar) {
	MenuButton btnMenu(this);
	{
		Action actSave(this,
			QIcon(":/images/forge/24/disk-black.png"),
			rtr(L"Сохранить изображение", "Save image"));
		actSave = QKeySequence("Ctrl+S");
		actSave.shortcutContext = Qt::ApplicationShortcut;
		actSave.addShortcutToTooltip();
		actSave.connectClicks(m_scene, SLOT(saveImageDialog()));
		btnMenu << actSave;

		Action actSave2(this, rtr(L"Сохранить c оригиналом горизонтально", "Save with original horizontally"));
		actSave2.connectClicks(m_scene, SLOT(saveSrcAndDstImagesH()));
		btnMenu << actSave2;

		Action actSave3(this, rtr(L"Сохранить c оригиналом вертикально", "Save with original vertically"));
		actSave3.connectClicks(m_scene, SLOT(saveSrcAndDstImagesV()));
		btnMenu << actSave3;
	}
	toolbar << btnMenu;
}
void MainWindow::addHelpMenu(ToolBar & toolbar) {
	Menu menu(toolbar);
	{
		Action actHelp(this, rtr(L"Справка", "&Help"));
		actHelp.connectClicks(this, SLOT(showHelp()));
		actHelp.shortcut = QKeySequence("F1");
		menu << actHelp;

		Action actAbout(this, rtr(L"О программе", "&About"),
			QIcon(":/qt-project.org/styles/commonstyle/images/fileinfo-16.png"));
		actAbout.connectClicks(this, SLOT(showAbout()));
		menu << actAbout;

		Action actAboutQt(this, rtr(L"Про Qt", "About &Qt"),
			QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
		actAboutQt.connectClicks(qApp, SLOT(aboutQt()));
		menu << actAboutQt;

		Action actFeedback(this, rtr(L"Обратная связь", "Send feedback"),
			QIcon(":/qt-project.org/dialogs/qprintpreviewdialog/images/go-next-24.png"));
		actFeedback.connectClicks(this, SLOT(sendFeedback()));
		menu << actFeedback;

		//Action actCheckVersion(this, rtr(L"Проверить доступность обновлений", "Check for updates"),
		//	QIcon(":/qt-project.org/styles/commonstyle/images/refresh-24.png"));
		//actCheckVersion.connectClicks(m_newVersionChecker, SLOT(checkWithDialog()));
		//menu << actCheckVersion;
	}
	ToolButton btnMenu(this);
	btnMenu.icon = QIcon(":/images/questionRect.png");
	btnMenu.toolTip = rtr(L"Показать меню помощи", "Show help menu");
	btnMenu->setMenu(menu);
	btnMenu->setPopupMode(QToolButton::InstantPopup);
	toolbar << btnMenu;
}
void MainWindow::updateShortcutTooltips() {
	QString str = rtr(L"Сделать снимок экрана", "Take screenshot");
	QString key = m_shortcutShow->shortcut().toString();
	if(!key.isEmpty()) {
		str += rtr(L"\nКлавиатура: %1", "\nShortcut: %1").arg(key);
	}
	m_actShoot->setToolTip(str);
}
void MainWindow::readSettings() {
	SettingsHash sett;
	sett.useSettingsFile();

	readPositionSettings(sett);
	m_shortcutShow->setShortcut(sett.value("MainWindow.shortcut", "Ctrl+F12").toString());
	updateShortcutTooltips();
	m_bFirstLaunch = sett.value("firstLaunch", true).toBool();
	m_bFirstClose = sett.value("firstClose", true).toBool();
	m_nScreenshotDelay = sett.value("screenshotDelay", m_nScreenshotDelay).toInt();
	if(m_nScreenshotDelay<1 && m_nScreenshotDelay>1000)
		m_nScreenshotDelay = 500;

	m_scene->readSettings(sett);
	synchSceneToUI();
	m_spectroToolbars->readSettings(sett);
	//m_newVersionChecker->readSettings(sett);
}
void MainWindow::readPositionSettings(SettingsHash & _sett) {
	auto sett = _sett.sub("QMainWindow");
	QByteArray state = sett.value("state").toByteArray();
	restoreState(state);

	QPoint pos = sett.value("pos", QPoint(100, 100)).toPoint();
	QSize size = sett.value("size").toSize();
	if(size.isValid()) {
		move(pos);
		resize(size);
	} else {
		showMaximized();
	}
	_sett.setSub("QMainWindow", sett);
}
void MainWindow::writePositionSettings(SettingsHash & _sett) {
	auto sett = _sett.sub("QMainWindow");
	sett.setValue("state", saveState());
	sett.setValue("pos", pos());
	sett.setValue("size", size());
	_sett.setSub("QMainWindow", sett);
}
void MainWindow::quit() {
	m_bQuit = true;
	close();
}
void MainWindow::saveSettings() {
	SettingsHash sett;
	sett.useSettingsFile();
	writePositionSettings(sett);
	m_scene->saveSettings(sett);
	sett.setValue("firstLaunch", false);
	sett.setValue("firstClose", m_bFirstClose);
	sett.setValue("screenshotDelay", m_nScreenshotDelay);
	sett.setValue("MainWindow.shortcut", m_shortcutShow->shortcut());
	m_spectroToolbars->saveSettings(sett);
	//m_newVersionChecker->saveSettings(sett);
}
void MainWindow::showHsl() {
	m_scene->setInAnimationMode(false);
	m_scene->setImage(DemoScene::hslScene(256, 50));
}
void MainWindow::showHsv() {
	m_scene->setInAnimationMode(false);
	m_scene->setImage(DemoScene::hsvScene(256, 50));
}
void MainWindow::showHslHsv() {
	m_scene->setInAnimationMode(false);
	m_scene->setImage(DemoScene::hslHsvCircles(256, 50));
}
void MainWindow::maybeShowFirstLaunchWindow() {
	if(!m_bFirstLaunch)
		return;
	QString str =
		rtr(L"<b>Первый запуск. Краткий набор советов:</b><ol>"
		L"<li>посмотрите %1</li>"
		L"<li>подсказки: у кнопок и вкладок на панелях инструметов при наведении мыши показываются подсказки, в которых указываются и клавиатурные сокращения</li>"
		L"<li>shift+прокрутка колесика мыши по изображению меняют 'Точность цвета'</li>"
		L"<li>выберите цветовой профиль, а затем используйте доступные режимы отображения внутри этого профиля</li>"
		L"<li>Чтобы свернуть окно вместо закрытия, используйте клавишу 'Esc'</li>"
		L"<li>больше информации в документации</li>"
		L"</ol>"
		,
		"<b>First program launch. Short list of advices:</b><ol>"
		"<li>Watch %1</li>"
		"<li>To minimize window to tray instead of closing it, use the 'Esc' key</li>"
		"<li>Tooltips: putting the cursor onto the buttons or tabs makes their descriptions to appear; description often includes keyboard shortcuts which launch the given command.</li>"
		"<li>Holding shift while scrolling mouse wheel changes 'Color range' slider</li>"
		"<li>Choose one of color profiles, and then use modes available under that profile</li>"
		"<li>See more in full documentation</li>"
		"</ol>")
		.arg(toHref("https://sourceforge.net/p/spectrohelp/wiki/Home/", "slideshow"));
	msgBox(str);
	//TODO: use QErrorMessage to 1) increase size 2) 'show at startup' check box
}
void MainWindow::messageQueueStarted() {
	updateVisibilityButtons();
	maybeShowFirstLaunchWindow();
	//m_scene->createGifFromWave();
}
QString MainWindow::feedEmailUrl() {
	QString ret = "mailto:spectrohelp@gmail.com?subject=SpectroHelp%20";
	ret += rtr(L"обратная%20связь&body=", "Feedback&body=");
	ret += rtr(L"Я использую SpectroHelp версии ", "I am using SpectroHelp version ") + m_strProgramVersion;
	return ret;
}
void MainWindow::sendFeedback() {
	QDesktopServices::openUrl(feedEmailUrl());
}
void MainWindow::handleAppMessage(const QString& message) {
	shootAgain();
}
void MainWindow::updateVisibilityButtons() {
	m_actShowColorScheme->setChecked(m_dockDemo->isVisible());
	m_actShowNames->setChecked(m_dockNames->isVisible());
}
void MainWindow::synchSceneToUI() {
	m_spectroToolbars->synchSceneToUI();
	m_spectroToolbars->m_bIgnoreTabSlots = true;
	m_actShowAnimation->setChecked(m_scene->inAnimationMode());
	m_spectroToolbars->m_bIgnoreTabSlots = false;
}