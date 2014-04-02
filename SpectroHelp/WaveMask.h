//WaveMask.h by Kostya Kozachuck as neurocod
#pragma once

class WaveMask {
	public:
		WaveMask();
		virtual ~WaveMask() {}
		void init(int waveLength);

		typedef unsigned char UColor;
		static const int n256 = 256;//colors per chanel
		UColor get(UColor color, int phase)const;
		void setPhase(qreal p);//[0 .. 1]
		void setPhase(int p);//[0 .. m_waveLength]
		void offsetPhase(int off);
	protected:
		int m_waveLength;//in pixels
		int m_wavePhase;//[0 .. m_waveLength]
		QVector<char> m_colors;
		void set(UColor color, int phase, UColor result);
};