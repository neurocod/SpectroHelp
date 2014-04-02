//WaveMask.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "WaveMask.h"

WaveMask::WaveMask() {
}
void WaveMask::setPhase(qreal _p) {
	_p *= m_waveLength;
	setPhase((int)_p);
}
void WaveMask::setPhase(int p) {
	p = qAbs(p);
	p %= m_waveLength;
	m_wavePhase = p;
}
void WaveMask::offsetPhase(int off) {
	setPhase(m_wavePhase + off);
}
void WaveMask::set(UColor color, int phase, UColor result) {
	phase = qAbs(phase);
	phase += m_wavePhase;
	phase %= m_waveLength;
	m_colors[phase * n256 + color] = result;
}
WaveMask::UColor WaveMask::get(UColor color, int phase)const {
	phase = qAbs(phase);
	phase += m_wavePhase;
	phase %= m_waveLength;
	return m_colors[phase * n256 + color];
}
void WaveMask::init(int waveLength) {
	ASSERT(1<waveLength
		&& waveLength<1000//for memory consumption
		);
	m_waveLength = waveLength;
	m_wavePhase = 0;
	m_colors.resize(m_waveLength * n256);
	for(int wave = 0; wave<m_waveLength; wave++) {
		qreal phase = wave;
		phase /= (qreal) m_waveLength;//[0 .. 1]
		phase *= PI;//0 .. PI
		qreal value = qSin(phase);//[0 .. 1]
		for(int color=0; color<n256; color++) {
			qreal v = value*(qreal)color;
			set(color, wave, v);
		}
	}
}