//PreviewTabBar.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "PreviewTabBar.h"

PreviewTabBar::PreviewTabBar() {
	setMouseTracking(true);
}
void PreviewTabBar::mouseMoveEvent(QMouseEvent * event) {
	__super::mouseMoveEvent(event);
	/*if(event->buttons()==0) {
		int index = tabAt(event->pos());
		if(-1!=index) {
			setCurrentIndex(index);
		}
	}*/
}