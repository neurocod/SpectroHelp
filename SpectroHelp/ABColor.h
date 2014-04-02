//ABColor.h by Kostya Kozachuck as neurocod
#pragma once

class ABColor {
	public:
		enum MissedChannel {
			MissedR,
			MissedG,
			MissedB,
		};

		ABColor() { m_a = m_b = 0; }
		ABColor(QRgb c);

		static QRgb imitate(MissedChannel ch, QRgb c);

		QRgb toRgb(MissedChannel m)const;
		quint16 m_a;
		quint16 m_b;
		quint16 m_g;//grey - for imitation of illness
};