//HslToRgb.h by Kostya Kozachuck as neurocod
#pragma once
#include "HsxToRgb.h"

class HslToRgb: public HsxToRgb<QColor::fromHsl> {
	public:
		virtual ~HslToRgb() {}
		static const HslToRgb& instance();
	protected:
		HslToRgb() {}
};