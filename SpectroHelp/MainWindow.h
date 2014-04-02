//MainWindow.h by Kostya Kozachuck as neurocod
#pragma once
#include "DockWidget.h"
#include "SpectroToolbarsManager.h"
class EGView;
class DemoScene;
class QxtGlobalShortcut;
//class NewVersionChecker;

class MainWindow: public QMainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~MainWindow();
		static MainWindow* instance();
		void showTab(ColorProfile cp, Mode mode) { m_spectroToolbars->showTab(cp, mode); }
	public slots:
		void updateVisibilityButtons();
		void handleAppMessage(const QString& message);

		void launch(QSystemTrayIcon::ActivationReason);
		void shootAgain();
		void continueShoot();
		void toggleVisibility();

		void showDemo();
		void showNames();
		void showHelp();
		void showAbout();
		void showHsl();
		void showHsv();
		void showHslHsv();
		void showSettings();
		void quit();
		void messageQueueStarted();
		void sendFeedback();

		void video();
		void videoR();
		void videoL();
		void videoT();//top
		void videoB();//bottom
	protected:
		static MainWindow* s_instance;
		SpectroToolbarsManager*m_spectroToolbars;
		QxtGlobalShortcut* m_shortcutShow;
		QString m_strProgramName;
		QString m_strProgramVersion;
		QString m_strProgramVersionPrev;
		EGView* m_view;
		New<DemoScene> m_demoScene;
		EGScene* m_scene;
		QTimer m_timer;
		Qt::WindowStates m_stateBeforeMinimize;
		DockWidget* m_dockDemo;
		DockWidget* m_dockNames;
		//NewVersionChecker* m_newVersionChecker;
		bool m_bQuit;
		bool m_bFirstLaunch;
		bool m_bFirstClose;
		int m_nScreenshotDelay;
		QAction* m_actShoot;
		QAction* m_actShowNames;
		QAction* m_actShowColorScheme;
		QAction* m_actShowAnimation;

		void video(Qt::Alignment side);
		virtual void keyPressEvent(QKeyEvent * event);
		virtual void closeEvent(QCloseEvent * event);
		virtual void changeEvent(QEvent * event);
		void showHelpTab();
		void showTestTab();
		void createTrayIcon();
		void createToolbars();
		static void setSliderRelative(QSlider*s, int n/*0 to 9*/);
		void addHelpMenu(ToolBar & toolbar);
		void addOpenMenu(ToolBar & toolbar);
		void addSaveMenu(ToolBar & toolbar);
		void addDemoMenu(ToolBar & toolbar);
		void addVideoMenu(ToolBar & toolbar);
		bool maybeSetTabIndexByChar(QKeyEvent * ev, const QString & keys, QTabBar*tab);
		void maybeShowFirstLaunchWindow();

		void readSettings();
		void saveSettings();
		void writePositionSettings(SettingsHash& settings);
		void readPositionSettings(SettingsHash & settings);
		void updateShortcutTooltips();
		virtual void synchSceneToUI();
		QString feedEmailUrl();
};