//win.cpp by Kostya Kozachuck as neurocod
#include <QPixmap.h>
#include <QTime>
#include "win.h"
//#include "disphelper/disphelper.h
#include <qwindowdefs.h>

bool EMainWindow_raise(WId _wid) {
	HWND wid = (HWND)_wid;
	//check if any visible window is above vlc in the z-order,
	//but ignore the ones always on top
	//and the ones which can't be activated
	WINDOWINFO wi;
	HWND hwnd;
	wi.cbSize = sizeof( WINDOWINFO );
	for(hwnd = GetNextWindow( wid, GW_HWNDPREV );
		hwnd && ( !IsWindowVisible( hwnd ) ||
				( GetWindowInfo( hwnd, &wi ) &&
				(wi.dwExStyle&WS_EX_NOACTIVATE) ) );
		hwnd = GetNextWindow( hwnd, GW_HWNDPREV ) )
	{
	}
	if( !hwnd || !GetWindowInfo( hwnd, &wi ) ||
		(wi.dwExStyle&WS_EX_TOPMOST) )
	{
		return true;
	} else {
		return false;
	}
}
//was QPixmap QPixmap::fromWinHBITMAP(HBITMAP bitmap, HBitmapFormat format = QPixmap::NoAlpha)
QImage QImage_fromWinHBITMAP(HBITMAP bitmap//format == QPixmap::NoAlpha
	)
{
    // Verify size
    BITMAP bitmap_info;
    memset(&bitmap_info, 0, sizeof(BITMAP));

    int res = GetObject(bitmap, sizeof(BITMAP), &bitmap_info);
    if (!res) {
        qErrnoWarning("QPixmap::fromWinHBITMAP(), failed to get bitmap info");
        return QImage();
    }
    int w = bitmap_info.bmWidth;
    int h = bitmap_info.bmHeight;

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = w;
    bmi.bmiHeader.biHeight      = -h;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage   = w * h * 4;

    QImage result;
    // Get bitmap bits
	static int s_imageSize = 0;
    static uchar *data = 0;
	int neededSize = bmi.bmiHeader.biSizeImage;
	if(s_imageSize!=neededSize) {
		delete[] data;
		data = 0;
	}
	if(!data) {
		s_imageSize = neededSize;
		data = new uchar[s_imageSize];
	}
    HDC display_dc = GetDC(0);
    if (GetDIBits(display_dc, bitmap, 0, h, data, &bmi, DIB_RGB_COLORS)) {
        QImage::Format imageFormat = QImage::Format_ARGB32_Premultiplied;
        uint mask = 0;
        //if (אשךüפו == QPixmap::NoAlpha == QPixmap::NoAlpha) {
            imageFormat = QImage::Format_RGB32;
            mask = 0xff000000;
        //}
        // Create image and copy data into image.
        QImage image(w, h, imageFormat);
        if (!image.isNull()) { // failed to alloc?
            int bytes_per_line = w * sizeof(QRgb);
            for (int y=0; y<h; ++y) {
                QRgb *dest = (QRgb *) image.scanLine(y);
                const QRgb *src = (const QRgb *) (data + y * bytes_per_line);
                for (int x=0; x<w; ++x) {
                    const uint pixel = src[x];
					dest[x] = pixel | 0xff000000;
                    /*if ((pixel & 0xff000000) == 0 && (pixel & 0x00ffffff) != 0)
                        dest[x] = pixel | 0xff000000;
                    else
                        dest[x] = pixel | mask;*/
                }
            }
        }
        result = image;
    } else {
        qWarning("QImage_fromWinHBITMAP(), failed to get bitmap bits");
    }
    ReleaseDC(0, display_dc);
    return result;
}
int ms_screen = 0;
int ms_screenFull = 0;
//was QPixmap QPixmap::grabWindow(WId winId, int x, int y, int w, int h)
QImage QImage_grabWindow(WId winId, int x, int y, int w, int h)
{
	QTime tm = QTime::currentTime();
    RECT r;
    GetClientRect((HWND)winId, &r);

    if (w < 0) w = r.right - r.left;
    if (h < 0) h = r.bottom - r.top;

#ifdef Q_WS_WINCE_WM
    if (qt_wince_is_pocket_pc()) {
        QWidget *widget = QWidget::find(winId);
        if (qobject_cast<QDesktopWidget *>(widget)) {
            RECT rect = {0,0,0,0};
            AdjustWindowRectEx(&rect, WS_BORDER | WS_CAPTION, FALSE, 0);
            int magicNumber = qt_wince_is_high_dpi() ? 4 : 2;
            y += rect.top - magicNumber;
        }
    }
#endif

    // Create and setup bitmap
    HDC display_dc = GetDC(0);
    HDC bitmap_dc = CreateCompatibleDC(display_dc);
    HBITMAP bitmap = CreateCompatibleBitmap(display_dc, w, h);
    HGDIOBJ null_bitmap = SelectObject(bitmap_dc, bitmap);

    // copy data
    HDC window_dc = GetDC((HWND)winId);
    BitBlt(bitmap_dc, 0, 0, w, h, window_dc, x, y, SRCCOPY
#ifndef Q_WS_WINCE
                                    | CAPTUREBLT
#endif
            );
    // clean up all but bitmap
    ReleaseDC((HWND)winId, window_dc);
    SelectObject(bitmap_dc, null_bitmap);
    DeleteDC(bitmap_dc);
    auto img = QImage_fromWinHBITMAP(bitmap);
	ms_screen += tm.elapsed();
    DeleteObject(bitmap);
    ReleaseDC(0, display_dc);
	ms_screenFull += tm.elapsed();
    return img;
}
int volumeInformation() {
	//That's fine, if you want the volume serial number. However, the volume serial number is not the hard disk serial number
	//and can change, either by formatting or with a simple utility http://www.codeproject.com/KB/system/change_drive_sn.aspx
	static DWORD dwVolSerial = 0;
	BOOL bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
	return dwVolSerial;
}
/*int volumeInformation2() {
	int ret = 0;
	DISPATCH_OBJ(wmiDisk);
	char* pszSerial = NULL;
	dhInitialize(TRUE);
	dhToggleExceptions(TRUE);
	dhGetObject(L"winmgmts:{impersonationLevel=impersonate}!\\\\.\\root\\cimv2:Win32_LogicalDisk='C:'",
		NULL, &wmiDisk);
	dhGetValue(L"%s", &pszSerial, wmiDisk, L".VolumeSerialNumber");
	ret = atoi(pszSerial);
	dhFreeString(pszSerial);
	SAFE_RELEASE(wmiDisk);
	dhUninitialize(TRUE);
	return ret;
}*/