//SpectroToolbarsManager.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "SpectroToolbarsManager.h"
#include "qxt/gui/qxtlabel.h"

SpectroToolbarsManager::SpectroToolbarsManager(QWidget *parent): QObject(parent) {
	m_parent = parent;
	m_bIgnoreTabSlots = false;
	m_modeLast = ModeOriginal;
}
void SpectroToolbarsManager::createToolbars() {
	{
		ToolBar toolbar(m_parent);
		toolbar.objectName = "toolbar.tool";
		m_toolbarTools = toolbar;
		addToolBar(toolbar);

		//NLProxyStyle::icoClose()

		/*{ Action act(this, QIcon(":/qt-project.org/styles/commonstyle/images/standardbutton-apply-16.png"));
		toolbar << act; }
		{ Action act(this, QIcon(":/qt-project.org/styles/commonstyle/images/newdirectory-32.png"));
		toolbar << act; }
		{ Action act(this, QIcon(":/qt-project.org/styles/commonstyle/images/newdirectory-16.png"));
		toolbar << act; }*/

		/*{ Action act(this, QIcon(":/qt-project.org/styles/commonstyle/images/standardbutton-open-32.png"));
		toolbar << act; }
		{ Action act(this, QIcon(":/qt-project.org/styles/commonstyle/images/standardbutton-open-16.png"));
		toolbar << act; }*/

	/*#ifdef _DEBUG
		CheckBox checkGray("DEBUG GRAY");
		toolbar << checkGray;
		checkGray.connectClicks(this, SLOT(toggleGray(bool)));
	#endif*/
		toolbar << new QxtLabel(rtr(L"Инструмент: ", "Tool: "));
		for(EGScene::ToolDesc d: m_scene->allTools()) {
			addTab(m_tabTools, d.m_name, d.m_toolTip);
		}
		setShortcutDescription(m_tabTools, "ZXCV");
		connect(m_tabTools, SIGNAL(currentChanged(int)), this, SLOT(tabToolChanged(int)));
		toolbar << m_tabTools;
	}
	/*
		:/images/forge/24/folder-horizontal-open.png
:/images/forge/24/film.png
:/images/forge/24/disk-black.png
:/qt-project.org/styles/commonstyle/images/standardbutton-open-32.png
:/qt-project.org/styles/commonstyle/images/standardbutton-open-16.png
		*/
	{
		ToolBar toolbar(m_parent);
		toolbar.objectName = "toolbar.mode";
		m_toolbarModes = toolbar;
		addToolBar(toolbar);

		for(ColorProfileDesc* d: ColorProfileDesc::allProfiles()) {
			QString name = rtr(L"Профиль %1", "%1 Profile").arg(d->name);
			m_boxProfiles.add(name, QVariant(d->cp));
			//d->toolTip
		}
		//TODO setShortcutDescription(m_boxProfiles, "ASDF");
		connect(m_boxProfiles, SIGNAL(currentIndexChanged(int)), this, SLOT(tabProfileChanged(int)));
		toolbar << m_boxProfiles;

		toolbar << new QxtLabel(" " + rtr(L"Режим: ", "Mode: "));
		connect(m_tabModes, SIGNAL(currentChanged(int)), this, SLOT(tabModeChanged(int)));
		fillModesTabs();
		toolbar << m_tabModes;
	}
	{
		ToolBar toolbar(m_parent);
		toolbar.objectName = "toolbar.sliders";
		m_toolbarRange = toolbar;
		addToolBar(toolbar);

		QString tooltip = rtr(
			L"'Точность цвета' определяет, сколько похожих цветов захватывается инструментами 'волна' и т. п.",
			"'Color range' defines how many similar colors are used by 'wave' tool and other tools.");
		toolbar->setToolTip(tooltip);
		toolbar << new QxtLabel(rtr(L"Точность цвета: ", "Color range: "));

		m_sliderColorRange = new QSlider(Qt::Horizontal, m_parent);
		m_sliderColorRange->setRange(1, 256);
		toolbar << m_sliderColorRange;
		m_sliderColorRange->setTickPosition(QSlider::TicksBelow);
		connect(m_sliderColorRange, SIGNAL(valueChanged(int)), SLOT(colorRangeValueChanged(int)));
	}
}
void SpectroToolbarsManager::addTab(QTabBar*tabs, QString text, QString tooltip, QVariant data) {
	int index = tabs->addTab(text);
	tabs->setTabToolTip(index, tooltip);
	tabs->setTabData(index, data);
}
void SpectroToolbarsManager::setShortcutDescription(QTabBar*tabs, const QString & keys) {
	for(int index = 0; index<tabs->count(); ++index) {
		if(index>=keys.length())
			break;
		QString str = tabs->tabToolTip(index);
		if(!str.isEmpty())
			str += "\n";
		str += rtr(L"Клавиатурное сокращение: ", "Shortcut: ") + keys.at(index);
		tabs->setTabToolTip(index, str);
	}
}
/*void SpectroToolbarsManager::setDifferentStyle(QTabBar*bar) {
	bar->setStyleSheet(
		"QTabWidget::pane { "//The tab widget frame
		"	border-top: 2px solid #C2C7CB;"
		"}"
		"QTabWidget::tab-bar {"
		"	left: 5px;"//move to the right by 5px
		"}"
		//Style the tab using the tab sub-control. Note that"
		//it reads QTabBar _not_ QTabWidget
		"QTabBar::tab {"
		"	background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
		"								stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
		"								stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
		"	border: 2px solid #C4C4C3;"
		"	border-bottom-color: #C2C7CB;"//same as the pane color
		"	border-top-left-radius: 4px;"
		"	border-top-right-radius: 4px;"
		"	min-width: 8ex;"
		"	padding: 2px;"
		"}"
		"QTabBar::tab:selected, QTabBar::tab:hover {"
		"	background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
		"								stop: 0 #FAFAFA, stop: 0.4 #f4f4f4,"
		"								stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
		"}"
		"QTabBar::tab:selected {"
		//"	border-color: #9B9B9B;"
		"	border-color: #0B9B0B;"
		"	border-bottom-color: #C2C7CB;"//same as pane color
		"}"
		"QTabBar::tab:!selected {"
		"	margin-top: 2px;"//make non-selected tabs look smaller
		"}"
			);
}*/
void SpectroToolbarsManager::fillModesTabs() {
	QScopedValueRollback<bool> pushIgnore(m_bIgnoreTabSlots);
	m_bIgnoreTabSlots = true;
	m_tabModes->hide();
	while(0!=m_tabModes->count())
		m_tabModes->removeTab(0);
	auto profile = currentProfile();
	for(ModeDesc m: profile->modes) {
		addTab(m_tabModes, m.name, m.tooltip, m.mode);
	}
	setShortcutDescription(m_tabModes, "QWERTYUIOP[]");
	m_tabModes->show();
}
void SpectroToolbarsManager::tabProfileChanged(int t) {
	Q_UNUSED(t)
	if(m_bIgnoreTabSlots)
		return;
	fillModesTabs();
	selectModeByCP();
}
void SpectroToolbarsManager::tabToolChanged(int t) {
	if(m_bIgnoreTabSlots)
		return;
	m_scene->setTool(t);
}
void SpectroToolbarsManager::tabModeChanged(int t) {
	if(m_bIgnoreTabSlots)
		return;
	QVariant d = m_tabModes->tabData(t);
	ASSERT(d.type()==QVariant::Int);
	Mode modeToSet = (Mode)d.toInt();
	m_scene->setMode(modeToSet);
	setLastMode(modeToSet);
}
void SpectroToolbarsManager::selectModeByCP() {
	auto profile = currentProfile();
	bool bUsePrevProfileMode = (m_modeLast!=ModeOriginal)
		&& -1!=profile->indexOfMode(m_modeLast);
	Mode modeToSet = bUsePrevProfileMode ? m_modeLast : profile->modeLast;
	m_scene->set(profile->cp, modeToSet);
	synchSceneToUI();
}
ColorProfileDesc* SpectroToolbarsManager::currentProfile() {
	return ColorProfileDesc::get(m_boxProfiles.currentIndex);
}
void SpectroToolbarsManager::synchUIToScene() {
	int indexM = m_tabModes->currentIndex();
	QVariant d = m_tabModes->tabData(indexM);
	ASSERT(d.type()==QVariant::Int);

	auto cpDesc = ColorProfileDesc::get(m_boxProfiles.currentIndex);
	m_scene->set(cpDesc->cp, (Mode)d.toInt());
}
void SpectroToolbarsManager::synchSceneToUI() {
	m_bIgnoreTabSlots = true;

	auto cp = m_scene->colorProfile();
	auto mode = m_scene->mode();
	ColorProfileDesc* cpDesc = ColorProfileDesc::get(cp);

	m_actShowTooltip->setChecked(m_scene->isShowingToolTip());
	m_boxProfiles.currentIndex = cpDesc->index;
	fillModesTabs();
	int index = cpDesc->indexOfMode(mode);
	ASSERT(-1!=index);
	m_tabModes->setCurrentIndex(index);
	setLastMode(mode);
	m_tabTools->setCurrentIndex(
		INDEX_OF_EXPR(m_scene->allTools(), m, m.m_tool==m_scene->tool())
		);
	m_bIgnoreTabSlots = false;
}
void SpectroToolbarsManager::setLastMode(Mode mode) {
	auto profile = currentProfile();
	profile->modeLast = m_scene->mode();
	if(profile->modeLast!=ModeOriginal)
		m_modeLast = profile->modeLast;
	int index = profile->indexOfMode(mode);
	ASSERT(-1!=index);
}
void SpectroToolbarsManager::addToolBar(QToolBar*t) {
	if(QMainWindow*mw = qobject_cast<QMainWindow*>(m_parent)) {
		mw->addToolBar(t);
		return;
	}
	m_parent->layout()->addWidget(t);
}
void SpectroToolbarsManager::colorRangeValueChanged(int value) {
	m_scene->setColorRange(value);
	m_sliderColorRange->setToolTip(toString(value));
	QPoint pos = m_sliderColorRange->rect().topLeft();
	pos = m_sliderColorRange->mapTo(m_parent, pos);
	QToolTip::showText(pos, toString(value));
}
void SpectroToolbarsManager::showTab(ColorProfile cp, Mode mode) {
	m_scene->set(cp, mode);
	synchSceneToUI();
}
void SpectroToolbarsManager::changeColorRange(int amount) {
	int value = m_sliderColorRange->value();
	value += amount;
	value = qMax(value, m_sliderColorRange->minimum());
	value = qMin(value, m_sliderColorRange->maximum());
	m_sliderColorRange->setValue(value);
}
void SpectroToolbarsManager::readSettings(SettingsHash & sett) {
	Mode mode = (Mode)sett.value("modeLast", m_modeLast).toInt();
	if(EGScene::checkRange(mode))
		m_modeLast = mode;
	m_sliderColorRange->setValue(m_scene->colorRange());
}
void SpectroToolbarsManager::saveSettings(SettingsHash & sett) {
	sett.setValue("modeLast", m_modeLast);
}
QAction* SpectroToolbarsManager::createActionTooltipVisibility() {
	Action actToolTip(0, rtr(L"Показать/скрыть цветовую подсказку", "Show/hide color tooltip"),
			QIcon(":/images/tooltip24.png"));
	actToolTip.addShortcutToTooltip();
	actToolTip.checkable = true;
	actToolTip.connectClicks(m_scene, SLOT(toggleShowToolTip()));
	m_actShowTooltip = actToolTip;
	return m_actShowTooltip;
}
void SpectroToolbarsManager::set(EGScene*scene) {
	ASSERT(!m_scene && scene);
	m_scene = scene;
	connect(m_scene, SIGNAL(toolChanged()), SLOT(synchSceneToUI()) );
}
bool SpectroToolbarsManager::isModeOriginal()const {
	return m_scene->mode()==ModeOriginal;
}
void SpectroToolbarsManager::toggleOriginalMode() {
	setModeOriginal(!isModeOriginal());
}
void SpectroToolbarsManager::setModeOriginal(bool orig) {
	if(orig==isModeOriginal())
		return;
	if(orig) {
		m_scene->setMode(ModeOriginal);
		synchSceneToUI();
		return;
	}
	auto profile = currentProfile();
	Mode modeNext;
	if(-1==profile->indexOfMode(m_modeLast)) {
		//choose mode next to the original
		ASSERT(profile->modes.size()>1);
		modeNext = profile->modes[1].mode;
	} else {
		modeNext = m_modeLast;
	}
	m_scene->setMode(modeNext);
	synchSceneToUI();
}
void SpectroToolbarsManager::altPressed(bool bPressedXorReleased) {
	setModeOriginal(!bPressedXorReleased);
}