//PreviewTabBar.h by Kostya Kozachuck as neurocod
#pragma once

//TODO: show thumbnail with that mode, or maybe preview that mode like tab was clicked
class PreviewTabBar: public QTabBar {
	public:
		PreviewTabBar();
		virtual ~PreviewTabBar() {}
	protected:
		virtual void mouseMoveEvent(QMouseEvent * event);
};