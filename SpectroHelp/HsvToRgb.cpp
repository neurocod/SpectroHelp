//HsvToRgb.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "HsvToRgb.h"

const HsvToRgb & HsvToRgb::instance() {
	static HsvToRgb s;
	return s;
}