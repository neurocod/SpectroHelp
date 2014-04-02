//ColorProfileDesc.h by Kostya Kozachuck as neurocod
#pragma once

enum Mode {
	ModeFirst = -1,

	ModeOriginal,
	ModeTuned,
	ModeShowAll,
	ModeRotateHsv,
	ModeCone,//R->RRR etc
	ModeFilter,
	ModeL_M,//L-M
	ModeRG2,
	ModeLM,
	ModeLM_S,
	ModeRG_B,
	ModeStrip,
	//ModeChannel //R|G|B
	ModeR,
	ModeG,
	ModeB,
	//ModeImitateIll
	ModeImitateR,
	ModeImitateG,
	ModeImitateB,
	ModeImitateAll,

	ModeEnd//always last
};
struct ModeDesc {
	Mode mode;
	QString name;
	QString tooltip;

	ModeDesc() { mode = ModeEnd; }
	ModeDesc(Mode m, QString _name, QString _tool=""): mode(m), name(_name), tooltip(_tool) {}
};
enum ColorProfile {
	ProfileFirst=-1,

	ProfileR,//red is invisible for person
	ProfileG,
	ProfileB,
	ProfileDemo,
	//ProfileAll,//fully color blind

	ProfileEnd//always last
};
class ColorProfileDesc: public QObject {
	public:
		typedef QMap<ColorProfile, ColorProfileDesc*> ColorProfileMap;
		static ColorProfileDesc*get(ColorProfile cp);
		static ColorProfileDesc*get(int index);
		static QString modeDesc(ColorProfile cp, Mode m);
		static QList<ColorProfileDesc*> allProfiles();

		ColorProfile cp;
		QString name;
		QString toolTip;
		QList<ModeDesc> modes;
		ModeDesc mode(Mode m)const {
			for(ModeDesc mo: modes)
				if(mo.mode==m)
					return mo;
			ASSERT(0);
			return modes[0];
		}
		Mode modeLast;
		int index;
		ColorProfileDesc(int _index, ColorProfile _cp, QString _name, QString _toolTip, QList<ModeDesc> _modes):
				cp(_cp), name(_name), toolTip(_toolTip), modes(_modes), index(_index) {
			modeLast = modes[0].mode;
		}
		int indexOfMode(Mode m)const;
		/*ColorProfileDesc(const ColorProfileDesc & other) {
			cp	= other.cp	;
			name	= other.name	;
			toolTip	= other.toolTip ;
			modes	= other.modes	;
			modeLast= other.modeLast;
			index	= other.index	;
		}*/
};