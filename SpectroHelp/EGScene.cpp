//EGScene.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "EGScene.h"
#include "Image.h"

int ms_draw = 0;

class TestQGraphicsProxyWidget: public QGraphicsProxyWidget {
	public:
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
			Q_UNUSED(widget)
			//__super::paint(painter, option, widget);
			QImage img(option->exposedRect.size().toSize(), QImage::Format_ARGB32);
			{
				QPainter p2(&img);
				__super::paint(&p2, option, 0);
			}
			img.invertPixels();
			painter->drawImage(scenePos(), img);
		}
};
EGScene::EGScene(QObject * parent): QGraphicsScene(parent) {
	m_bInAnimation = false;
	m_bSkipPrepareAnimation = false;
	setBackgroundBrush(Qt::black);

	{
		New<QTimer> m_timerAnimation;
		m_timerAnimation->start(1);
		connect(m_timerAnimation, SIGNAL(timeout()), this, SLOT(animate()));
	}
	addItem(m_tooltipColor);
	m_tooltipColor->hide();
	setImage(QImage());
#ifdef _DEBUG
	/*loadImage("E:\\neuro img\\EyeWave\\demo\\"
		"testImage.png"
		//"PieChart.gif"
		);*/
#endif
	shootScreen();
	ASSERT(QImage::Format_RGB32==m_src.format()
		|| QImage::Format_ARGB32==m_src.format());

	//addWidget(new TestQLabel("Text in label"));
	/*QWebView *view = new QWebView();
	view->load(QUrl("http://qt.nokia.com/"));
	QGraphicsProxyWidget *proxy = new TestQGraphicsProxyWidget();
	proxy->setWidget(view
		//new TestQLabel("Text in label")
		);
	proxy->setPos(0, 100);
	addItem(proxy);*/
	test();
}
void EGScene::setImage(const QImage & img) {
	m_tooltipColor->setPlainText("");
	__super::setImage(img);
	setSceneRect(m_src.rect());
	invalidate();
}
//virtual
void EGScene::drawBackground(QPainter * painter, const QRectF & _rect) {
	//__super::drawBackground(painter, _rect);
	QRect rect = m_src.rect().intersected(_rect.toRect());
	drawBackground(rect);
	painter->drawImage(rect.topLeft(), m_dst, rect);
}
void EGScene::drawBackground(const QRect & rect) {
	prepareAnimation();
	QTime tm = QTime::currentTime();
	__super::draw(rect);
	ms_draw += tm.elapsed();
}
void EGScene::prepareAnimation() {
	if(m_bSkipPrepareAnimation)
		return;
	int period = s_msWavePeriod;
	if(ModeRotateHsv==m_mode)
		period = s_msChannelPeriod;
	//else if(ModeStrip==m_mode && m_colorProfile==ProfileAll)
		;//period = 3*period/2;
	//calculate phase
	int ms = m_timeStart.elapsed();
	ms %= period;
	m_phase = ms;
	m_phase /= (qreal)period;
	m_mask.setPhase(m_phase);
}
void EGScene::mouseMoveEvent(QGraphicsSceneMouseEvent * ev) {
	maybeSelectColor(ev);
}
void EGScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * ev) {
	mousePressEvent(ev);
}
void EGScene::mousePressEvent(QGraphicsSceneMouseEvent * ev) {
	const auto btn = ev->button();
	bool bToggleColorSelection = true;
	if(Qt::RightButton==btn) {
		//next tool
		Tool t = (Tool)(m_tool+1);
		if(!checkRange(t))
			t = (Tool)(ToolStart + 1);
		setTool(t);
		bToggleColorSelection = false;
		if(m_tool==ToolNone)
			return;
	}
	if(Qt::LeftButton==btn || Qt::RightButton==btn) {
		if(bToggleColorSelection && colorSelected()) {
			if(ToolDark==m_tool || ToolLight==m_tool) {
				m_clrSelected = QColor();
				invalidate();
				return;
			}
		}
		maybeSelectColor(ev);
		return;
	}
}
void EGScene::selectColor(QColor color) {
	m_clrSelected = color;
	invalidate();
	if(ToolNone==m_tool) {
		setTool(ToolWave);
	}
}
void EGScene::maybeSelectColor(QGraphicsSceneMouseEvent * ev) {
	updateColorTip(ev);
	if(ev->buttons() & (Qt::LeftButton|Qt::RightButton)) {
		auto scenePos = ev->scenePos().toPoint();
		if(m_src.rect().contains(scenePos)) {
			QRgb rgb = m_src.pixel(scenePos);
			selectColor(rgb);
		}
	}
}
void EGScene::animate() {
	if(m_bInAnimation) {
		shootScreen();
		return;
	}
	if(!QApplication::focusWidget())
		return;
	if(ModeStrip==m_mode
	|| ModeRotateHsv==m_mode
	|| ModeShowAll==m_mode
	|| ToolWave==m_tool)
		invalidate();
}
void EGScene::invalidate(bool bRequestOtherWindow) {
	if(bRequestOtherWindow && m_widToUpdate) {
		m_widToUpdate->update();
		return;
	}
	updateColorTip(m_tooltipColor->m_lastColorPos);
	for(QGraphicsView *view: views()) {
		view->setCacheMode(QGraphicsView::CacheBackground);
		view->setCacheMode(QGraphicsView::CacheNone);
	}
	//update();
	__super::invalidate();
}
void EGScene::invalidate() {
	invalidate(true);
}
QImage QImage_grabWindow(WId winId, int x=0, int y=0, int w=-1, int h=-1);
void EGScene::shootScreen() {
	auto winid = QApplication::desktop()->winId();
	//clear image for low memory situations on embedded devices
	//QPixmap p = QPixmap::grabWindow(winid);
	auto img = QImage_grabWindow(winid);
	setImage(img);
}
void EGScene::loadFileDialogH() {
	QImage img;
	if(loadFileDialog(img))
		setImage(Image::concat(m_src, img, Qt::Horizontal));
}
void EGScene::loadFileDialogV() {
	QImage img;
	if(loadFileDialog(img))
		setImage(Image::concat(m_src, img, Qt::Vertical));
}
void EGScene::loadFromClipboard() {
	auto img = QApplication::clipboard()->image();
	if(img.isNull())
		msgBox(rtr(L"Буфер обмена не содержит изображение", "Clipboard does not contains image"));
	else
		setImage(img);
}
void EGScene::loadFileDialog() {
	QImage img;
	if(loadFileDialog(img))
		setImage(img);
}
bool EGScene::loadFileDialog(OUT QImage & img) {
	QList<QByteArray> li = QImageReader::supportedImageFormats ();
	QString fileName = QFileDialog::getOpenFileName(0,
		rtr(L"Открыть файл", "Open file"), "",//TODO: last used dir?
		FileFacility::imageExtensionsForFileDialog<QImageReader>()
		//, QString * selectedFilter = 0, Options options = 0
		);
	return loadImage(fileName, img);
}
bool EGScene::loadImage(QString fileName, OUT QImage & img) {
	if(fileName.isEmpty())
		return false;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	bool ok = img.load(fileName);
	if(ok) {
		setInAnimationMode(false);
		QApplication::restoreOverrideCursor();
		return true;
	} else {
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(0,
			rtr(L"Ошибка чтения файла", "Error reading file"),
			rtr(L"Не удалось загрузить изображение из файла %1", "Could not load image from file %1")
				.arg(fileName),
			QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
}
void EGScene::loadImage(QString fileName) {
	QImage img;
	if(loadImage(fileName, img))
		setImage(img);
}
void EGScene::saveSrcAndDstImages(Qt::Orientation or) {
	fillDst();
	auto img = Image::concat(m_src, m_dst, or);
	saveImageDialog(img);
}
void EGScene::saveSrcAndDstImagesV() {
	saveSrcAndDstImages(Qt::Vertical);
}
void EGScene::saveSrcAndDstImagesH() {
	saveSrcAndDstImages(Qt::Horizontal);
}
void EGScene::saveImageDialog() {
	fillDst();
	saveImageDialog(m_dst);
}
void EGScene::saveImageDialog(const QImage & img) {
	bool old = m_bInAnimation;
	setInAnimationMode(false);
	QString format = "png";
	QString initialPath = rtr(L"Безымянный.", "untitled.") + format;
	QString fileName = QFileDialog::getSaveFileName(0, rtr(L"Сохранить изображение", "Save image"),
		initialPath,//dir
		FileFacility::imageExtensionsForFileDialog<QImageWriter>()
		//,QString * selectedFilter = 0, Options options = 0
		);
	saveImage(img, fileName);
	setInAnimationMode(old);
}
void EGScene::saveImage(const QImage & img, QString fileName) {
	if(fileName.isEmpty())
		return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	bool ok = img.save(fileName);

	QApplication::restoreOverrideCursor();
	if(!ok) {
		QMessageBox::warning(0,
			rtr(L"Ошибка записи файла", "Error writing file"),
			rtr(L"Не удалось сохранить изображение в файл %1", "Could not save image to file %1")
				.arg(fileName),
			QMessageBox::Ok, QMessageBox::Ok);
	}
}
void EGScene::test() {
#ifndef _DEBUG
	return;
#endif
	return;
	QStringList li;
	li
		<< "click on any color from image to activate 'wave' tool"
		/*<< "take screenshot"
		<< "start video screen capture"
		<< "load image from disk"
		<< "1) choose color profile"
		<< "2) list of modes will change"
		<< "select any mode to modify colors"*/
		;
	QFont font;
	font.setPixelSize(24);
	QFontMetrics fm(font);

	QColor clrBk = QColor(0x4F81BD);
	QColor clrText = QColor(0xffffff);
	QColor clrBk2 = QColor(0x888888);
	QImage ret;
	for(QString str: li) {
		auto rcText = fm.boundingRect(str);
		rcText.adjust(0,0,2,0);
		rcText.moveTo(0,0);
		int expansion = 5;
		auto rcImg = rcText.adjusted(-expansion, -expansion, expansion, expansion);
		rcImg.moveTo(0,0);
		QImage img(rcImg.size(), QImage::Format_ARGB32);
		img.fill(clrBk2);
		{
			QPainter painter(&img);
			painter.setFont(font);
			painter.setPen(clrText);
			painter.setBrush(clrBk);
			painter.drawRoundedRect(rcImg.adjusted(0,0,-1,-1), expansion, expansion);
			painter.setPen(clrText);
			rcText.moveTo(expansion, expansion);
			painter.drawText(rcText, str);
		}
		Image::addBorder(img, expansion, QColor(clrBk2));
		ret = Image::concat(ret, img, Qt::Vertical, clrBk2);
	}
	Image::replaceAll(ret, clrBk2.rgb(), qRgba(0,0,0,0));
	setImage(ret);
	/*test(0, 1, 120);
	for(int r=0;r<256;r++)
		for(int g=0;g<256;g++)
			for(int b=0;b<256;b++) {
				test(r,g,b);
			}
	int g=3;*/

	//QString file = QString("E:\\neuro img\\EyeWae\\gif\\%1.png");
	//saveImage(m_dst, file);
}
void EGScene::test(int r, int g, int b) {
	QRgb rgb = qRgb(r, g, b);
	int h,s,v;
	int H,S,V;
	auto & rgbToHsv = RgbToHsv::instance();
	rgbToHsv.get(rgb, H,S,V);
	QColor(r,g,b).getHsv(&h, &s, &v);
	ASSERT(h==H && s==S && v==V);
	auto & hsvToRgb = HsvToRgb::instance();
	QRgb rgb2 = hsvToRgb.get(h,s,v);
	int R = qAbs(qRed(rgb2) - r);
	int G = qAbs(qGreen(rgb2) - g);
	int B = qAbs(qBlue(rgb2) - b);
	ASSERT(R<6 && G<6 && B<6);
}
void EGScene::readSettings(SettingsHash & _sett) {
	auto sett = _sett.sub("EGScene");

	m_tooltipColor->readSettings(sett);
	Tool tool = (Tool)sett.value("tool", m_tool).toInt();
	if(checkRange(tool))
		setTool(tool);

	ColorProfile cp = (ColorProfile)sett.value("colorProfile", m_colorProfile).toInt();
	Mode mode = (Mode)sett.value("mode", m_mode).toInt();
	set(cp, mode);

	m_clrSelected = sett.value("clrSelected", m_clrSelected).value<QColor>();
	m_nColorDif = sett.value("colorRange", m_nColorDif).toInt();
}
void EGScene::saveSettings(SettingsHash & _sett) {
	auto sett = _sett.sub("EGScene");

	m_tooltipColor->saveSettings(sett);
	sett.setValue("tool", (int)m_tool);
	sett.setValue("colorProfile", (int)m_colorProfile);
	sett.setValue("mode", (int)m_mode);
	sett.setValue("clrSelected", m_clrSelected);
	sett.setValue("colorRange", m_nColorDif);

	_sett.setSub("EGScene", sett);
}
void EGScene::updateColorTip(QPointF posf) {
	QPoint pos = posf.toPoint();
	QColor clr, clr2;
	if(m_src.rect().contains(pos)) {
		clr = m_src.pixel(pos);
		drawBackground(QRect(pos, QSize(1, 1)));
		clr2 = m_dst.pixel(pos);
	}
	m_tooltipColor->show(clr, clr2);
	m_tooltipColor->calculatePos(posf, sceneRect());
}
void EGScene::updateColorTip(QGraphicsSceneMouseEvent *ev) {
	updateColorTip(ev->scenePos());
}
void EGScene::setInAnimationMode(bool b) {
	m_bInAnimation = b;
	emit animationModeChanged(b);
	invalidate();
}
void EGScene::toggleAnimation() {
	setInAnimationMode(!m_bInAnimation);
}
void EGScene::toggleShowToolTip() {
	m_tooltipColor->m_bShowToolTip = !m_tooltipColor->m_bShowToolTip;
	invalidate();
}
void EGScene::generateDemo() {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	auto oldProfile = m_colorProfile;
	auto oldMode = m_mode;
	m_bAddModeDesc = true;

	QImage ret;
	{
		//original, R-G, R-G/2, LM-S,
		m_colorProfile = ProfileR;

		m_mode = ModeL_M;
		fillDst();
		ret = Image::concat(m_src, m_dst, Qt::Horizontal);

		m_mode = ModeRG2;
		fillDst();
		ret = Image::concat(ret, m_dst, Qt::Horizontal);

		m_mode = ModeLM_S;
		fillDst();
		ret = Image::concat(ret, m_dst, Qt::Horizontal);
	}
	QList<Mode> modes; modes /* << ModeChannel */ << ModeCone << ModeFilter << ModeTuned;
	QList<ColorProfile> profiles; profiles << ProfileR << ProfileG << ProfileB;
	for(ColorProfile cp: profiles) {
		m_colorProfile = cp;
		QImage sub;
		for(Mode m: modes) {
			m_mode = m;
			fillDst();
			sub = Image::concat(sub, m_dst, Qt::Horizontal);
		}
		ret = Image::concat(ret, sub);
	}
	setImage(ret);

	m_colorProfile = oldProfile;
	m_mode = oldMode;
	m_bAddModeDesc = false;

	QApplication::restoreOverrideCursor();
	invalidate();
}
void EGScene::createGifFromWave() {
	loadImage("E:\\neuro img\\EyeWave\\slide\\en\\tokyo.png");
	QString folder = "E:\\neuro img\\EyeWave\\gif\\subway\\2\\";
	if(!QDir().exists(folder)) {
		msgBox("!exists " + folder);
		return;
	}
	m_bSkipPrepareAnimation = true;
	int count = 10;
	for(int i=0; i<10; i++) {
		prepareAnimation();
		qreal phase = i;
		phase /= count;
		m_mask.setPhase(phase);
		fillDst();
		saveImage(m_dst, QString(folder + "%1.png").arg(i));
	}
	m_bSkipPrepareAnimation = false;
	msgBox("saved");
}