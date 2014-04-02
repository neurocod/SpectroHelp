//EGScene.h by Kostya Kozachuck as neurocod
#pragma once
#include "ImageModifier.h"
#include "ColorToolTipItem.h"
class QGraphicsItem;

class EGScene: public QGraphicsScene, public ImageModifier {
	Q_OBJECT
	public:
		EGScene(QObject * parent = 0);
		virtual ~EGScene() {}
		void setWidgetToUpdate(QWidget*w) { m_widToUpdate = w;}
		void readSettings(SettingsHash & sett);
		void saveSettings(SettingsHash & sett);

		virtual void setImage(const QImage & );
		void shootScreen();
		void fillDst() { drawBackground(m_dst.rect()); }
		void loadImage(QString fileName);
		void saveImage(const QImage & img, QString file);
		void saveImageDialog(const QImage & img);
		bool inAnimationMode()const { return m_bInAnimation; }
		void setInAnimationMode(bool b);
		bool isShowingToolTip()const { return m_tooltipColor->m_bShowToolTip; }
		//to create presentations for web-site
		void createGifFromWave();
		void invalidate(bool bRequestOtherWindow);
	public slots:
		void animate();
		void invalidate();
		void selectColor(QColor color);
		void loadFileDialog();
		void loadFileDialogH();
		void loadFileDialogV();
		void loadFromClipboard();
		void saveImageDialog();
		void saveSrcAndDstImagesV();
		void saveSrcAndDstImagesH();
		void saveSrcAndDstImages(Qt::Orientation or);
		void toggleAnimation();
		void toggleShowToolTip();
		void generateDemo();
	signals:
		void toolChanged();
		void animationModeChanged(bool bInAnimation);
	protected:
		QPointer<QWidget> m_widToUpdate;
		bool m_bInAnimation;
		bool m_bSkipPrepareAnimation;

		New<ColorToolTipItem> m_tooltipColor;

		bool loadFileDialog(OUT QImage & img);
		bool loadImage(QString fileName, OUT QImage & img);

		void drawBackground(const QRect & r);
		virtual void drawBackground(QPainter * painter, const QRectF & r)override;
		//virtual void drawForeground(QPainter * painter, const QRectF & r)override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * ev)override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * ev)override;
		//virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * ev)override;
		virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * ev)override;

		void maybeSelectColor(QGraphicsSceneMouseEvent * ev);
		void prepareAnimation();

		void test();
		void test(int r, int g, int b);
		void updateColorTip(QGraphicsSceneMouseEvent *ev);
		void updateColorTip(QPointF pos);
		void requestInvalidate() { invalidate(); }
		void emitToolChanged() { emit toolChanged(); }
};