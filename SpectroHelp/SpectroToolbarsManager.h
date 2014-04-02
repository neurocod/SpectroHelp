//SpectroToolbarsManager.h by Kostya Kozachuck as neurocod
#pragma once
#include "EGScene.h"
#include "PreviewTabBar.h"

//Synch all toolbars
class SpectroToolbarsManager: public QObject {
	Q_OBJECT
	public:
		SpectroToolbarsManager(QWidget *parent);
		virtual ~SpectroToolbarsManager() {}
		void createToolbars();
		void readSettings(SettingsHash & sett);
		void saveSettings(SettingsHash & sett);

		void showTab(ColorProfile cp, Mode mode);
		void changeColorRange(int amount);
		void set(EGScene*scene);
		QComboBox* boxProfiles() { return m_boxProfiles; }
		QTabBar* tabTools() { return m_tabTools; }
		QTabBar* tabModes() { return m_tabModes; }
		QSlider* sliderColorRange() { return m_sliderColorRange; }

		QToolBar*toolbarModes() { return m_toolbarModes; }
		QToolBar*toolbarTools() { return m_toolbarTools; }
		QToolBar*toolbarRange() { return m_toolbarRange; }
		QAction* createActionTooltipVisibility();

		void setModeOriginal(bool orig);
		bool isModeOriginal()const;
		void toggleOriginalMode();
		void altPressed(bool bPressedXorReleased);
	public slots:
		void tabModeChanged(int t);
		void tabProfileChanged(int t);
		void tabToolChanged(int t);
		void colorRangeValueChanged(int value);
		void synchUIToScene();
		virtual void synchSceneToUI();
	protected:
		QPointer<QWidget> m_parent;
		QPointer<EGScene> m_scene;
		ColorProfile m_profile;
		Mode m_modeLast;
		ComboBox m_boxProfiles;
		New<QTabBar> m_tabTools;
		New<PreviewTabBar> m_tabModes;
		QPointer<QSlider> m_sliderColorRange;

		QPointer<QToolBar> m_toolbarModes;
		QPointer<QToolBar> m_toolbarTools;
		QPointer<QToolBar> m_toolbarRange;
		QPointer<QAction> m_actShowTooltip;

		friend class MainWindow;
		bool m_bIgnoreTabSlots;

		void setShortcutDescription(QTabBar*tabs, const QString & keys);
		static void addTab(QTabBar*tabs, QString text, QString tooltip="", QVariant data=QVariant());
		//void setDifferentStyle(QTabBar*bar);

		void fillModesTabs();
		ColorProfileDesc* currentProfile();
		void selectModeByCP();
		void setLastMode(Mode m);

		void addToolBar(QToolBar*t);
};