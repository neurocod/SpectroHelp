//EGView.h by Kostya Kozachuck as neurocod
#pragma once
class EGScene;
class SpectroToolbarsManager;

class EGView: public QGraphicsView {
	public:
		EGView(EGScene* scene, SpectroToolbarsManager*stm, QWidget * parent = 0);
		virtual ~EGView() {}
	protected:
		virtual void wheelEvent(QWheelEvent *ev);

		SpectroToolbarsManager*m_toolbars;
};