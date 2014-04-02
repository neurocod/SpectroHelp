//ImageModifier.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ImageModifier.h"
#include "ColorTransformationTable.h"

#define FOR_Y_IMG const int y_end = m_rect.bottom(); \
	for(int y=m_rect.top(); y<=y_end; ++y) {			\
		QRgb *dst = (QRgb *)m_dst.scanLine(y);	\
		QRgb *src = (QRgb *)m_src.scanLine(y);	\
		const int x_end = m_rect.right();
#define END_FOR_Y }
#define FOR_X_IMG for(int x = m_rect.left(); x<=x_end; ++x)

ImageModifier::ImageModifier() {
	m_bDebugGray = false;
	m_bAddModeDesc = false;
	m_tool = ToolNone;
	m_nColorDif = 20;
	m_colorProfile = ProfileR;
	m_mode = ColorProfileDesc::get(m_colorProfile)->modes[0].mode;
	m_phase = 0;
	m_mask.init(30);
	m_timeStart.start();
}
void ImageModifier::setMode(Mode mode) {
	if(!checkRange(mode)) {
		ASSERT(0);
		return;
	}
	m_mode = mode;
	m_timeStart.start();
	requestInvalidate();
}
void ImageModifier::grayImage() {
	const int y_end = m_rect.bottom();
	for(int y=m_rect.top(); y<=y_end; ++y) {
		QRgb *dst = (QRgb *)m_dst.scanLine(y);
		const int x_end = m_rect.right();
		FOR_X_IMG {
			QRgb p = dst[x];
			//int g = qGray(p);
			int g = gray(p);
			dst[x] = qRgb(g, g, g);
		}
	}
}
void ImageModifier::toolLight() {
	if(!colorSelected())
		return;
	QRgb selected = m_clrSelected.rgb();
	int diffToApply;
	if(ToolDark==m_tool)
		diffToApply = -60;
	else
		diffToApply = 60;
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			QRgb cNew = dst[x];
			if(diffAbs(p, selected)>m_nColorDif) {
				int g = gray(dst[x]);
				g += diffToApply;
				g = qMin(g, 255);
				g = qMax(g, 0);
				cNew = qRgb(g, g, g);
			}
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showLuminance() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			//int gr = (105*qRed(p) + 151*qGreen(p))/256;
			int gr = qGray(p);
			QRgb cNew = qRgb(gr, gr, gr);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showRG2() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int diff = qRed(p) - qGreen(p)/2;
			if(diff<0)
				diff = 0;
			QRgb cNew = qRgb(diff, diff, diff);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showL_M() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int diff = qRed(p) - qGreen(p);
			if(diff<0)
				diff = 0;
			QRgb cNew = qRgb(diff, diff, diff);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showLM() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int sum = qRed(p) + qGreen(p);
			QRgb cNew = qRgb(sum/2, sum/2, 0);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showLM_S() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int sum = (qRed(p) + qGreen(p))/2-qBlue(p);
			if(sum<0)sum=0;
			QRgb cNew = qRgb(sum, sum, sum);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showRG_B() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int sum = (qRed(p) + qGreen(p))/2-qBlue(p);
			if(sum<0)sum=0;
			QRgb cNew = qRgb(sum, sum, 0);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showCone() {
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int ch = channelByProfile(p);
			QRgb cNew = qRgb(ch, ch, ch);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showStrip() {
	const int stripSize = 64;
	int stripOffset = 2*m_phase*stripSize;
	//if(ProfileAll==m_colorProfile) {
		showStripFull(stripSize);
		return;
	//}
	/*auto channel = missedChannel();
	FOR_Y_IMG
		int stripLine = (y + stripOffset)/stripSize;
		if(stripLine % 2) {
			FOR_X_IMG {
				QRgb p = src[x];
				int ch = channelByProfile(p);
				QRgb cNew = qRgb(ch, ch, ch);
				dst[x] = cNew;
			}
		} else {
			FOR_X_IMG {
				QRgb p = src[x];
				QRgb cNew = ABColor::imitate(channel, p);
				dst[x] = cNew;
			}
		}
	END_FOR_Y*/
	FOR_Y_IMG
		int stripLine = (y + stripOffset)/stripSize;
		ColorProfile cp = (ColorProfile)(stripLine % 2);
		FOR_X_IMG {
			QRgb p = src[x];
			int ch = channelByProfile(p, cp);
			QRgb cNew = qRgb(ch, ch, qBlue(p));
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showStripFull(int stripSize) {
	int stripOffset = 3*m_phase*stripSize;
	FOR_Y_IMG
		int stripLine = (y + stripOffset)/stripSize;
		ColorProfile cp = (ColorProfile)(stripLine % 3);
		FOR_X_IMG {
			QRgb p = src[x];
			int ch = channelByProfile(p, cp);
			QRgb cNew = qRgb(ch, ch, ch);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showChannel() {
	int mask = 0;
	switch(m_mode) {
	case ModeR: mask = qRgb(255, 0, 0); break;
	case ModeG: mask = qRgb(0, 255, 0); break;
	case ModeB: mask = qRgb(0, 0, 255); break;
	default:
		ASSERT(0);
	}
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			QRgb cNew = p & mask;
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showImitated() {
	if(m_mode==ModeImitateAll) {
		showLuminance();
		return;
	}
	auto channel = missedChannel();
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			QRgb cNew = ABColor::imitate(channel, p);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showTuned() {
	auto channel = missedChannel();
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			ABColor ab = p;
			QRgb cNew = ab.toRgb(channel);
			dst[x] = cNew;
		}
	END_FOR_Y
}
int degreeDiffAbs(int d0, int d1) {
	int diff = d0 - d1;
	while (diff < -180) diff += 360;
	while (diff > 180) diff -= 360;
	if(diff<0)
		diff = -diff;
	return diff;
}
void ImageModifier::showFilter() {
	int angleCenter;
	int angleRange;
	const int saturMin = 50;
	switch(m_colorProfile) {
	case ProfileR: angleCenter = 0; angleRange = 35; break;
	case ProfileG: angleCenter = 120; angleRange = 60; break;
	case ProfileB: angleCenter = 240; angleRange = 40; break;
	default:
		ASSERT(0);
		return;
	}
	auto & rgb2hsv = RgbToHsv::instance();
	const qreal transmittance = 0.3;
	qreal terminatorZone = 5;//длина зоны размытия (в градусах) - плавного перехода к более темной области
	qreal kMap = (1 - transmittance)/terminatorZone;
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int h,s,v;
			rgb2hsv.get(p, h,s,v);
			qreal k = 1;
			do {
				if(-1==h) {
					k = transmittance;
					break;
				}
				int maxAngleDif = angleRange/2 + 20*s/255;
				qreal angleDiff = degreeDiffAbs(angleCenter, h) - maxAngleDif;
				if(angleDiff>terminatorZone) {
					k = transmittance;
					break;
				}
				if(angleDiff>0) {
					//map k to [1 .. transmittance] <- angleDiff [0 .. terminatorZone]
					//k = 1 - 0*kMap
					//k = 1 - angleDif*kMap, and if angleDif==terminatorZone->
					//k = transmittance = 1 - terminatorZone*kMap
					//terminatorZone*kMap  = 1 - transmittance
					//kMap = (1 - transmittance)/terminatorZone
					//1 - angleDiff * kMap
					k = 1 - angleDiff * kMap;
				}
				if(s<saturMin) {
					//map k [transmittance .. 1] as s [0 .. saturMin]
					qreal k2 = s;
					translate<qreal>(0, saturMin, transmittance, 1, k2);
					k = qMin(k, k2);
					break;
				}
			} while(false);
			if(k!=1) {
				v *= k;
				//QRgb cNew = HsvToRgb::s_instance.get(h,s,v);
				QRgb cNew = qRgb(k*qRed(p), k*qGreen(p), k*qBlue(p));
				dst[x] = cNew;
			}
		}
	END_FOR_Y
}
void ImageModifier::rotateHsv() {
	qreal karr[360];
	{
		qreal angle = m_phase * 360;
		for(int i=0; i<_countof(karr); i++) {
			qreal k;
			int diff = (int)angle-i;
			diff = qAbs(diff);
			if(diff>180)
				diff = 360-diff;
			const int distance = 60;
			if(diff>distance)
				k = 0.3;
			else {
				k = 1 - ((qreal)diff/(qreal)distance)/2;
				//k = qSqrt(k);
			}
			karr[i] = k;
		}
	}
	auto & rgbToHsv = RgbToHsv::instance();
	auto & hsvToRgb = HsvToRgb::instance();
	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			int h,s,v;
			rgbToHsv.get(p, h,s,v);
			qreal k;
			if(-1==h)
				k = 1;
			else {
				ASSERT(0<=h && h<360);
				k = karr[h];
			}
			v *= k;
			QRgb rbgNew = hsvToRgb.get(h,s,v);
			int ch = qGray(rbgNew);
			QRgb cNew = qRgb(ch, ch, ch);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::showAll() {
	qreal qSubMode = m_phase;
	enum Channel {CR=0, CG, CB } channel;
	if(qSubMode<0.3333) {
		channel = CR;
	} else if(qSubMode<0.6666) {
		channel = CG;
		qSubMode -= 0.3333;
	} else {
		channel = CB;
		qSubMode -= 0.6666;
	}
	qSubMode *= 3;//0 .. 1
	qSubMode *= PI/2;
	qreal k = qSin(qSubMode);//k for main channel
	//k *= k;
	qreal k2 = 1 - k;//k2 for other channels
	qreal arrk[] = {0, 0, 0};
	arrk[channel] = k;
	if(channel==CR)
		arrk[2] = k2;
	else
		arrk[channel-1] = k2;

	FOR_Y_IMG
		FOR_X_IMG {
			QRgb p = src[x];
			qreal arr[] = {qRed(p), qGreen(p), qBlue(p)};
			arr[0] *= arrk[0];
			arr[1] *= arrk[1];
			arr[2] *= arrk[2];
			QRgb cNew = qRgb(arr[0], arr[1], arr[2]);
			dst[x] = cNew;
		}
	END_FOR_Y
}
void ImageModifier::toolWave() {
	if(!colorSelected())
		return;
	QRgb selected = m_clrSelected.rgb();

	FOR_Y_IMG
		int clr = m_mask.get(255, y);
		QRgb cNew = qRgb(clr, clr, clr);
		FOR_X_IMG {
			QRgb p = src[x];
			if(diffAbs(p, selected)<m_nColorDif) {
				dst[x] = cNew;
			}
		}
	END_FOR_Y
}
void ImageModifier::copyToDest() {
	FOR_Y_IMG
		FOR_X_IMG {
			dst[x] = src[x];
		}
	END_FOR_Y
}
//static
int ImageModifier::diffAbs(QRgb c0, QRgb c1) {
	int arr[3] = {
		qRed(c0)-qRed(c1),
		qGreen(c0) - qGreen(c1),
		qBlue(c0) - qBlue(c1),
	};
	int diff = 0;
	for(int i = 0; i<3; i++) {
		int v = arr[i];
		if(v<0)
			v=-v;
		diff += v;
	}
	return diff;
}
ABColor::MissedChannel ImageModifier::missedChannel()const {
	switch(m_colorProfile) {
		case ProfileR: return ABColor::MissedR;
		case ProfileG: return ABColor::MissedG;
		case ProfileB: return ABColor::MissedB;
	}
	switch(m_mode) {
		case ModeImitateR: return ABColor::MissedR;
		case ModeImitateG: return ABColor::MissedG;
		case ModeImitateB: return ABColor::MissedB;
	}
	ASSERT(0);
	return ABColor::MissedR;
}
int ImageModifier::channelByProfile(QRgb rgb)const {
	return channelByProfile(rgb, m_colorProfile);
}
//static
int ImageModifier::channelByProfile(QRgb rgb, ColorProfile cp) {
	switch(cp) {
	case ProfileR: return qRed(rgb);
	case ProfileG: return qGreen(rgb);
	case ProfileB: return qBlue(rgb);
	default:
		ASSERT(0);
		return 0;
	}
}
//static
bool ImageModifier::checkRange(Tool t) {
	return ToolStart<t && t<ToolEnd;
}
//static
bool ImageModifier::checkRange(Mode m) {
	return ModeFirst<m && m<ModeEnd;
}
//static
bool ImageModifier::checkRange(ColorProfile cp) {
	return ProfileFirst<cp && cp<ProfileEnd;
}
QList<ImageModifier::ToolDesc> ImageModifier::allTools()const {
	if(m_allTools.isEmpty()) {
		m_allTools
			<< ToolDesc(ToolNone, rtr(L" - ", " - "),
				rtr(L"Пусто - применяется для просмотра оригинального изображения без изменений, а также для ускорения перерисовки",
				"None - used to view original image without changes and to speedup repaints"))
			<< ToolDesc(ToolWave, rtr(L"Волна", "Wave"),
				rtr(L"Показывает анимированную волну вместо цвета, который выбран мышей",
				"Shows waves on selected color. Select color with mouse"))
			<< ToolDesc(ToolDark, rtr(L"Тень", "Dark"),
				rtr(L"Оставляет выбранный цвет неизменным, а остальные делает серыми и более темными",
				"Leaves selected color unchanged, and other colors makes gray and more dark"))
			<< ToolDesc(ToolLight, rtr(L"Свет", "Light"),
				rtr(L"Оставляет выбранный цвет неизменным, а остальные делает серыми и более светлыми",
				"Leaves selected color unchanged, and other colors makes gray and more light"))
			;
	}
	return m_allTools;
}
void ImageModifier::setTool(int tool) {
	Tool t = (Tool)tool;
	if(checkRange(t)) {
		if(m_tool!=t) {
			m_tool = t;
			emitToolChanged();
			requestInvalidate();
		}
	} else {
		ASSERT(0);
	}
}
void ImageModifier::setColorRange(int dif) {
	if(1<= dif && dif <255*2) {
		m_nColorDif = dif;
		requestInvalidate();
	}
}
void ImageModifier::setImage(const QImage & _img) {
	auto img = _img;
	if(img.size().isEmpty())
		img = QImage(1, 1, QImage::Format_RGB32);
	m_src = img.convertToFormat(QImage::Format_RGB32);
	m_dst = m_src;
}
void ImageModifier::draw(const QRect & rect) {
	m_rect = rect;
	copyToDest();
	switch(m_mode) {
	case ModeOriginal: break;
	case ModeR:
	case ModeG:
	case ModeB:
		showChannel(); break;
	case ModeTuned: showTuned(); break;
	case ModeImitateAll:
	case ModeImitateR:
	case ModeImitateG:
	case ModeImitateB:
		showImitated(); break;
	case ModeShowAll: showAll(); break;
	case ModeRotateHsv: rotateHsv(); break;
	case ModeCone: showCone(); break;
	case ModeL_M: showL_M(); break;
	case ModeFilter: showFilter(); break;
	case ModeLM: showLM(); break;
	case ModeLM_S: showLM_S(); break;
	case ModeRG_B: showRG_B(); break;
	case ModeRG2: showRG2(); break;
	case ModeStrip: showStrip(); break;
	default:
		ASSERT(0);
	}
	switch(m_tool) {
	case ToolNone: break;
	case ToolWave: toolWave(); break;
	case ToolDark://fall
	case ToolLight:
		toolLight();
		break;
	default:
		ASSERT(0);
	}
	if(m_bAddModeDesc) {
		auto cpDesc = ColorProfileDesc::get(m_colorProfile);
		QString str = cpDesc->mode(m_mode).name;
		if(ModeFilter==m_mode || ModeTuned==m_mode) {
			str.prepend(cpDesc->name + " ");
		}
		QPainter painter(&m_dst);
		QFont font;
		font.setPixelSize(24);
		painter.setFont(font);
		auto rect = painter.boundingRect(m_dst.rect(), str);
		painter.fillRect(rect, QColor(0xffffffff));
		painter.drawText(rect, str);
	}
	if(m_bDebugGray)
		grayImage();
}
void ImageModifier::set(ColorProfile cp, Mode m) {
	if(!checkRange(cp) || !checkRange(m) || -1==ColorProfileDesc::get(cp)->indexOfMode(m)) {
		ASSERT(0);
		return;
	}
	m_colorProfile = cp;
	m_mode = m;
	//TODo: check compatibility
	requestInvalidate();
}