//HsvToRgb.h by Kostya Kozachuck as neurocod
#pragma once
#include "HsxToRgb.h"

class HsvToRgb: public HsxToRgb<QColor::fromHsv> {
	public:
		virtual ~HsvToRgb() {}
		static const HsvToRgb& instance();
	protected:
		HsvToRgb(){}
};