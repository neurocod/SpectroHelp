//main.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "MainWindow.h"
#include "Application.h"

QtMessageHandler g_oldMessageHandler;
void localQMsgHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg) {
	(*g_oldMessageHandler)(type, context, msg);
	QString str;
	switch (type) {
		case QtDebugMsg: str = "Qt Debug"; break;
		case QtWarningMsg: str = "Qt Warning"; break;
		case QtCriticalMsg: str = "Qt Critical"; break;
		case QtFatalMsg: str = "Qt Fatal"; break;
	}
	str += QString(" function %1 file %2 line %3: %4").arg(context.function).arg(context.file).arg(context.line).arg(msg);
	//str.prepend(QDateTime::currentDateTime());
	bool prevent = msg.contains("Unable to set geometry");
	if(!prevent && QCoreApplication::instance()->thread() == QThread::currentThread()) {
		msgBox("Qt message", str);
	}
}
int main(int argc, char *argv[]) {
	Application app(argc, argv);
	//after app will be QSinglaApplication again, uncomment this:
#ifdef SINGLE_APPLICATION
	if (app.isRunning()) {
		app.sendMessage("newInstance");
		return 0;
	}
#endif
	g_oldMessageHandler = qInstallMessageHandler(localQMsgHandler);

	MainWindow w;
#ifdef SINGLE_APPLICATION
	app.setActivationWindow(&w);
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &w, SLOT(handleAppMessage(const QString&)));
#endif
	w.showMaximized();
	return app.exec();
}