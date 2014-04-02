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
	//LogFile::debug() << QDateTime::currentDateTime() << str << endl;
	bool prevent = msg.contains("Unable to set geometry");
	if(!prevent && QCoreApplication::instance()->thread() == QThread::currentThread()) {
		msgBox("Qt message", str);
	}
}
int main(int argc, char *argv[]) {
	Application app(argc, argv);
	//if (app.isRunning()) {
	//	app.sendMessage("newInstance");
	//	return 0;
	//}
	g_oldMessageHandler = qInstallMessageHandler(localQMsgHandler);

	MainWindow w;
	//app.setActivationWindow(&w);
	//QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
	//				  &w, SLOT(handleAppMessage(const QString&)));
	w.showMaximized();
	//w.showExample();
	return app.exec();
}