//DemoVisionDiagram.h by Kostya Kozachuck as neurocod
#pragma once
#include "GridItemsLayout.h"
#include "TextWithBorderGraphicsItem.h"
#include "EGScene.h"
#include "DemoScene.h"

class DemoVisionDiagram: public QGraphicsItemGroup {
	public:
		DemoVisionDiagram();
		virtual ~DemoVisionDiagram() {}
		void createArrow(QGraphicsItem*i0, QGraphicsItem*i1);

		struct Item: public TextWithBorderGraphicsItem {
			Item(ColorProfile cp, Mode mode, const QString & text, DemoVisionDiagram*parent):
					TextWithBorderGraphicsItem(text, parent) {
				setData(DemoScene::s_nKeyProfile, (int)cp);
				setData(DemoScene::s_nKeyMode, (int)mode);
				QString tooltip = ColorProfileDesc::modeDesc(cp, mode);
				auto desc = ColorProfileDesc::get(cp);
				tooltip += rtr(L"\nПрофиль: %1 Режим: %2", "\nProfile: %1 mode: %2")
					.arg(desc->name)
					.arg(desc->mode(mode).name);
				setToolTip(tooltip);
			}
		};
		GridItemsLayout m_grid;
		New<Item> Pixel,
			R, G, B,
			scone, mcone, lcone,
			lms, lm, l_m,
			blueYellow, luminance, redGreen;
};