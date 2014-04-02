//HslToRgb.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "HslToRgb.h"

//static
const HslToRgb & HslToRgb::instance() {
	static HslToRgb s;
	return s;
}