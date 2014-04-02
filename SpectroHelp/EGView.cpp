//EGView.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "EGView.h"
#include "EGScene.h"
#include "MainWindow.h"

EGView::EGView(EGScene* scene, SpectroToolbarsManager*stm, QWidget* parent): QGraphicsView(scene, parent) {
	m_toolbars = stm;
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//setAttribute(Qt::WA_OpaquePaintEvent);
}
void EGView::wheelEvent(QWheelEvent *ev) {
	if(ev->buttons() & Qt::RightButton) {
		//if(m_bUseRMBScrolling)
		{
			//scene()->ignoreNextContextMenu();
			QWheelEvent event(ev->pos(), ev->globalPos(), ev->delta(), ev->buttons(), 0, Qt::Horizontal);
			__super::wheelEvent(&event);
			return;
		}
	}
	if(ev->modifiers() & Qt::ShiftModifier) {
		m_toolbars->changeColorRange(ev->delta()/10);
		return;
	}
	/*if(ev->modifiers() & Qt::ControlModifier) {
		//scaleView(pow((double)2, event->delta() / 240.0));
		scaleView(pow((double)2, ev->delta() / 720.0));
		return;
	}*/

	__super::wheelEvent(ev);
}