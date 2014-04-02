//ImageModifier.h by Kostya Kozachuck as neurocod
#pragma once
#include "WaveMask.h"
#include "Vector2d.h"
#include "HsvToRgb.h"
#include "RgbToHsv.h"
#include "ABColor.h"
#include "ColorProfileDesc.h"

class ImageModifier {
	public:
		ImageModifier();
		virtual ~ImageModifier() {}
		virtual void requestInvalidate()=0;
		virtual void setImage(const QImage & img);
		QImage dst()const { return m_dst; }
		void draw(const QRect & rect);

		enum Tool {
			ToolStart=-1,

			ToolNone,
			ToolWave,
			ToolDark,
			ToolLight,

			ToolEnd
		};
		struct ToolDesc {
			Tool m_tool;
			QString m_name;
			QString m_toolTip;
			ToolDesc(Tool tool, QString name, QString toolTip=""):
				m_tool(tool), m_name(name), m_toolTip(toolTip) {}
		};
		QList<ToolDesc> allTools()const;
		void setTool(int tool);
		Tool tool()const { return m_tool; }
		Mode mode()const { return m_mode; }
		void setMode(Mode mode);

		void set(ColorProfile ñp, Mode m);
		ColorProfile colorProfile()const { return m_colorProfile; }
		static bool checkRange(Mode m);
		static bool checkRange(Tool t);
		static bool checkRange(ColorProfile cp);
		int colorRange()const { return m_nColorDif; }
		void setDebugGray(bool b) {
			m_bDebugGray = b;
			requestInvalidate();
		}
		void setColorRange(int dif);
	protected:
		QImage m_src;
		QImage m_dst;//on screen
		QRect m_rect;//on which need process m_src/m_dst

		Tool m_tool;
		Mode m_mode;
		QColor m_clrSelected;
		int m_nColorDif;
		bool m_bDebugGray;
		bool m_bAddModeDesc;
		bool m_bAllowTools;
		int m_nMaxGrayColorDiff;
		QTime m_timeStart;
		WaveMask m_mask;
		mutable QList<ToolDesc> m_allTools;

		qreal m_phase;//[0 .. 1]
		static const int s_msWavePeriod = 1500;
		static const int s_msChannelPeriod = 5000;

		ColorProfile m_colorProfile;
		int channelByProfile(QRgb rgb)const;
		static int channelByProfile(QRgb rgb, ColorProfile cp);
		ABColor::MissedChannel missedChannel()const;

		bool colorSelected()const { return m_clrSelected.isValid(); }
		static int diffAbs(QRgb c0, QRgb c1);

		void copyToDest();
		static int gray(QRgb p) {
			int tmp = qRed(p) + qGreen(p) + qBlue(p);
			return tmp/3;
		}
		void grayImage();
		void toolLight();
		void toolWave();
		void showChannel();
		void showTuned();
		void showImitated();
		void showCone();
		void showRG2();
		void showL_M();
		void showLM();
		void showLM_S();
		void showRG_B();
		void showStrip();
		void showStripFull(int stripSize);
		void showLuminance();
		void showAll();
		void showFilter();
		void rotateHsv();

		virtual void emitToolChanged()=0;
};